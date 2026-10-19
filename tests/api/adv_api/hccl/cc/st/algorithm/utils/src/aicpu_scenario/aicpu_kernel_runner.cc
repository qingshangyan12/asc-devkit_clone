/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "aicpu_kernel_runner.h"

#include "alg_data_trans_wrapper.h"
#include "coll_alg_v2_exec_registry.h"
#include "hccl_res.h"
#include "hcomm_primitives.h"
#include "op_common.h"
#include "securec.h"

namespace HcclSim {
namespace AicpuSt {
namespace {
// 收编常量（原 op_common.cc:108 / kernel_launch.cc 系）
constexpr u32 HOST_WAIT_AICPU_NOTIFYIDX = 0;         // host 主流 wait aicpu 流的 notify idx
constexpr u32 ENTRANCE_NOTIFY_WAIT_TIME = 27U * 68U; // 原 HcclAicpuKernelEntranceLaunch 尾部
constexpr u32 DEFAULT_NOTIFY_IDX = 0;                // kernel run 完成通知 idx（原 kernel_launch.cc:371）

// ═══ RestoreVarData* 收编（原 kernel_launch.cc:501-581，产品文件废弃待删，连带收编保完整）═══
// 作用：把随 OpParam 序列化传递的变长数组（sendCounts/displs 等）还原为指针。
// 当前 AG/RS 场景 opType 不经过这些分支（varMemSize=0），收编为 AllToAllV 系场景预留。
HcclResult RestoreVarDataBatchSendRecv(mc2_ops_hccl::OpParam& param)
{
    const uint64_t sendRecvItemSize = static_cast<uint64_t>(sizeof(HcclSendRecvItem));
    const uint64_t itemNum = static_cast<uint64_t>(param.batchSendRecvDataDes.itemNum);
    if (param.varMemSize != itemNum * sendRecvItemSize) {
        HCCL_ERROR(
            "[AicpuStKernelRunner] varMemSize[%llu] != itemNum[%llu] * itemSize[%llu] for BatchSendRecv.",
            param.varMemSize, itemNum, sendRecvItemSize);
        return HCCL_E_PARA;
    }
    param.batchSendRecvDataDes.sendRecvItemsPtr = reinterpret_cast<HcclSendRecvItem*>(param.varData);
    return HCCL_SUCCESS;
}

HcclResult RestoreVarDataAlltoAllV(mc2_ops_hccl::OpParam& param, const mc2_ops_hccl::AlgResourceCtxSerializable& resCtx)
{
    const uint64_t rankSize = resCtx.topoInfo.userRankSize;
    if (param.varMemSize != mc2_ops_hccl::ALL_TO_ALL_V_VECTOR_NUM * rankSize * sizeof(uint64_t)) {
        HCCL_ERROR(
            "[AicpuStKernelRunner] varMemSize[%llu] invalid for AlltoAllV, rankSize[%llu].", param.varMemSize,
            rankSize);
        return HCCL_E_PARA;
    }
    constexpr uint64_t OFFSET_RECV_COUNTS = 1;
    constexpr uint64_t OFFSET_SDISPLS = 2;
    constexpr uint64_t OFFSET_RDISPLS = 3;
    uint64_t* data = reinterpret_cast<uint64_t*>(param.varData);
    param.all2AllVDataDes.sendCounts = data;
    param.all2AllVDataDes.recvCounts = data + OFFSET_RECV_COUNTS * rankSize;
    param.all2AllVDataDes.sdispls = data + OFFSET_SDISPLS * rankSize;
    param.all2AllVDataDes.rdispls = data + OFFSET_RDISPLS * rankSize;
    return HCCL_SUCCESS;
}

HcclResult RestoreVarDataReduceScatterV(
    mc2_ops_hccl::OpParam& param, const mc2_ops_hccl::AlgResourceCtxSerializable& resCtx)
{
    const uint64_t rankSize = resCtx.topoInfo.userRankSize;
    if (param.varMemSize != mc2_ops_hccl::REDUCE_SCATTER_V_VECTOR_NUM * rankSize * sizeof(uint64_t)) {
        HCCL_ERROR(
            "[AicpuStKernelRunner] varMemSize[%llu] invalid for ReduceScatterV, rankSize[%llu].", param.varMemSize,
            rankSize);
        return HCCL_E_PARA;
    }
    uint64_t* data = reinterpret_cast<uint64_t*>(param.varData);
    param.vDataDes.counts = data;
    param.vDataDes.displs = data + rankSize;
    return HCCL_SUCCESS;
}

HcclResult RestoreVarDataAllGatherV(
    mc2_ops_hccl::OpParam& param, const mc2_ops_hccl::AlgResourceCtxSerializable& resCtx)
{
    const uint64_t rankSize = resCtx.topoInfo.userRankSize;
    if (param.varMemSize != mc2_ops_hccl::ALL_GATHER_V_VECTOR_NUM * rankSize * sizeof(uint64_t)) {
        HCCL_ERROR(
            "[AicpuStKernelRunner] varMemSize[%llu] invalid for AllGatherV, rankSize[%llu].", param.varMemSize,
            rankSize);
        return HCCL_E_PARA;
    }
    uint64_t* data = reinterpret_cast<uint64_t*>(param.varData);
    param.vDataDes.counts = data;
    param.vDataDes.displs = data + rankSize;
    return HCCL_SUCCESS;
}

HcclResult RestoreVarData(mc2_ops_hccl::OpParam& param, const mc2_ops_hccl::AlgResourceCtxSerializable& resCtx)
{
    // 分发表与原 kernel_launch.cc:305-315 一致
    switch (param.opType) {
        case HcclCMDType::HCCL_CMD_BATCH_SEND_RECV:
            return RestoreVarDataBatchSendRecv(param);
        case HcclCMDType::HCCL_CMD_ALLTOALLV:
        case HcclCMDType::HCCL_CMD_ALLTOALLVC:
        case HcclCMDType::HCCL_CMD_ALLTOALL:
            return RestoreVarDataAlltoAllV(param, resCtx);
        case HcclCMDType::HCCL_CMD_REDUCE_SCATTER_V:
            return RestoreVarDataReduceScatterV(param, resCtx);
        case HcclCMDType::HCCL_CMD_ALLGATHER_V:
            return RestoreVarDataAllGatherV(param, resCtx);
        default:
            return HCCL_SUCCESS; // 非变长算子无还原需求（AG/RS 等）
    }
}
} // namespace

HcclResult AicpuStKernelRunner::Run(
    HcclComm comm, mc2_ops_hccl::OpParam& param, std::unique_ptr<mc2_ops_hccl::TopoInfoWithNetLayerDetails>& topoInfo,
    const std::string& algName)
{
    using namespace mc2_ops_hccl;
    // ═══ ① 执行包装段（收编自 HcclExecOp op_common.cc:429-551，AICPU_TS 分支）═══
    param.hcclComm = comm;
    const int tagRet = sprintf_s(param.commModeTag, sizeof(param.commModeTag), "%s_%s", param.commName, "_opbase");
    CHK_PRT_RET(tagRet <= 0, HCCL_ERROR("[AicpuStKernelRunner] fill commModeTag failed"), HCCL_E_INTERNAL);
    const int algRet = sprintf_s(param.algName, sizeof(param.algName), "%s", algName.c_str());
    CHK_PRT_RET(algRet <= 0, HCCL_ERROR("[AicpuStKernelRunner] fill algName failed"), HCCL_E_INTERNAL);

    // AICPU 引擎无 CannBridge（UseCannBridge 仅劫持 ALLTOALLV/ALLREDUCE）——本地 registry
    std::unique_ptr<InsCollAlgBase> executor = CollAlgExecRegistryV2::Instance().GetAlgExec(param.opType, algName);
    CHK_PRT_RET(
        executor.get() == nullptr, HCCL_ERROR("Fail to find executor for algName[%s]", algName.c_str()), HCCL_E_PARA);

    std::unique_ptr<AlgResourceCtxSerializable> resCtxHost = std::make_unique<AlgResourceCtxSerializable>();
    resCtxHost->isHcommBatchTransferOnThreadSupported = HcommIsSupportHcommBatchTransferOnThread();
    void* resCtxSequence = nullptr;
    bool isResourceReused = false;

    ThreadHandle cpuTsThread{0};
    ThreadHandle exportedAicpuTsThread{0};
    CHK_RET(HcclThreadAcquireWithStream(comm, COMM_ENGINE_CPU_TS, param.stream, 1, &cpuTsThread));
    CHK_RET(HcclThreadExportToCommEngine(comm, 1, &cpuTsThread, COMM_ENGINE_AICPU_TS, &exportedAicpuTsThread));

    // 资源计算/复用（fixture 的 GetOpParamResCtx 已分配过——此处命中复用路径，与原 HcclExecOp 行为一致）
    CHK_RET(HcclGetAlgRes(comm, param, executor, topoInfo.get(), resCtxHost, &resCtxSequence, isResourceReused));
    if (isResourceReused) {
        CHK_PTR_NULL(resCtxSequence);
        CHK_PRT_RET(param.ctxSize == 0U, HCCL_ERROR("[AicpuStKernelRunner] reused ctx size is zero."), HCCL_E_PARA);
    }
    // （原 :507-534 HcclDfxRegOpInfo：DFX 桩空实现、无任务图足迹——省略）

    ThreadHandle mainThread;
    u32 notifyNumOnMainThread = 0;
    CHK_RET(GetMainThreadInfo(comm, param, mainThread, notifyNumOnMainThread));
    ThreadHandle exportedCpuTsThread;
    CHK_RET(HcclThreadExportToCommEngine(comm, 1, &mainThread, COMM_ENGINE_CPU_TS, &exportedCpuTsThread));
    param.opThread = exportedAicpuTsThread;

    ThreadHandle unfoldThread;
    CHK_RET(GetUnfoldThreadInfo(comm, param, unfoldThread));
    CHK_RET(CaptureSlaveStreams(comm, param.stream, {mainThread, unfoldThread}));

    // ═══ ② host→device 通知（收编自 HcclAicpuKernelEntranceLaunch op_common.cc:632）═══
    // 主流上 idx 最大的 notify（handshake 1/4）
    CHK_RET(static_cast<HcclResult>(
        HcommThreadNotifyRecordOnThread(cpuTsThread, exportedCpuTsThread, notifyNumOnMainThread - 1)));
    // （原 :636-648 AicpuKernelLaunch[aclrt launch]+HcclReportAicpuKernel：launch 语义在 ST 无
    //  对应物（同步模型下发与执行合一），DFX 桩空——由下方 kernel run 内联替代）

    // ═══ ③ kernel run 段（收编自 HcclLaunchAicpuKernel 950 路径 kernel_launch.cc:238-389）═══
    CHK_PRT_RET(
        HcommAcquireComm(param.commName) != 0, HCCL_ERROR("[AicpuStKernelRunner] HcommAcquireComm fail"),
        HCCL_E_INTERNAL);

    // 反序列化（原 :276-301；省 g_cacheManager 缓存——ST 每用例独立世界，直接反序列化等价）
    AlgResourceCtxSerializable resCtx;
    char* ctx = static_cast<char*>(param.resCtx);
    std::vector<char> seq(ctx, ctx + param.ctxSize);
    resCtx.DeSerialize(seq);

    CHK_RET(RestoreVarData(param, resCtx));
    CHK_RET(InitHcommBatchTransferOnThreadSupported(resCtx.isHcommBatchTransferOnThreadSupported));

    ThreadHandle thread = resCtx.threads[0];
    CHK_PRT_RET(
        HcommBatchModeStart(param.algTag) != 0, HCCL_ERROR("[AicpuStKernelRunner] HcommBatchModeStart fail"),
        HCCL_E_INTERNAL);
    // （原 :333-339 HcommProfilingReportKernelStartTask：DFX 桩空——省略）

    // 主 thread 等待 host stream 通知（handshake 2/4；原 :341-352）
    u32 maxNotifyNum = resCtx.notifyNumOnMainThread;
    for (u32 notifyNum : resCtx.notifyNumPerThread) {
        if (notifyNum > maxNotifyNum) {
            maxNotifyNum = notifyNum;
        }
    }
    CHK_RET(static_cast<HcclResult>(HcommThreadNotifyWaitOnThread(thread, maxNotifyNum, CUSTOM_TIMEOUT)));

    // ★ 算法编排（被测物；与活跃链最后一跳 cann_dispatch_bridge.cc:136 调用形态一致）
    std::unique_ptr<InsCollAlgBase> runExecutor = CollAlgExecRegistryV2::Instance().GetAlgExec(param.opType, algName);
    CHK_PRT_RET(
        runExecutor.get() == nullptr, HCCL_ERROR("Fail to find executor for algName[%s]", algName.c_str()),
        HCCL_E_PARA);
    HcclResult orchestrateRet = runExecutor->Orchestrate(param, resCtx);
    CHK_PRT_RET(
        orchestrateRet != HCCL_SUCCESS,
        HCCL_ERROR("[AicpuStKernelRunner] orchestrate failed for alg[%s], ret[%d]", param.algName, orchestrateRet),
        orchestrateRet);
    // （原 :366-369 HcommProfilingReportDeviceOp：DFX 桩空——省略）

    // 主 thread 通知 host stream（handshake 3/4；原 :371-376）
    CHK_RET(
        static_cast<HcclResult>(HcommThreadNotifyRecordOnThread(thread, exportedAicpuTsThread, DEFAULT_NOTIFY_IDX)));
    // （原 :378-384 HcommProfilingReportKernelEndTask：DFX 桩空——省略）
    CHK_PRT_RET(
        HcommBatchModeEnd(param.algTag) != 0, HCCL_ERROR("[AicpuStKernelRunner] HcommBatchModeEnd fail"),
        HCCL_E_INTERNAL);
    CHK_PRT_RET(
        HcommReleaseComm(param.commName) != 0, HCCL_ERROR("[AicpuStKernelRunner] HcommReleaseComm fail"),
        HCCL_E_INTERNAL);

    // ═══ ④ host 等待 device 完成（收编自 HcclAicpuKernelEntranceLaunch 尾部 op_common.cc:652-654，handshake 4/4）═══
    CHK_RET(static_cast<HcclResult>(
        HcommThreadNotifyWaitOnThread(cpuTsThread, HOST_WAIT_AICPU_NOTIFYIDX, ENTRANCE_NOTIFY_WAIT_TIME)));
    return HCCL_SUCCESS;
}

} // namespace AicpuSt
} // namespace HcclSim
