/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * 本程序是自由软件，您可以根据 CANN 开源软件许可证 2.0 版的条款和条件对其进行再分发和/或修改。
 * 详情请参阅许可证。如不符合许可证要求，不得使用本软件。
 */
#include "ins_v2_all_to_all_v_sole_executor.h"
#include "topo_match_1d.h"
#if !defined(AICPU_COMPILE) && MC2_CLIENT_ENABLE_CCU
#include "../template/ccu/ccu_temp_half_all_to_all_v_mesh_1D.h"
#endif

namespace mc2_ops_hccl {

template <typename AlgTopoMatch, typename InsAlgTemplate>
InsV2AlltoAllVSoleExecutor<AlgTopoMatch, InsAlgTemplate>::InsV2AlltoAllVSoleExecutor()
{}

/*
 * 拓扑匹配：使用 AlgTopoMatch 计算 AlgHierarchyInfoForAllLevel
 */
template <typename AlgTopoMatch, typename InsAlgTemplate>
HcclResult InsV2AlltoAllVSoleExecutor<AlgTopoMatch, InsAlgTemplate>::CalcAlgHierarchyInfo(
    HcclComm comm, TopoInfoWithNetLayerDetails* topoInfo, AlgHierarchyInfoForAllLevel& algHierarchyInfo)
{
    AlgTopoMatch topoMatch;
    CHK_RET(topoMatch.MatchTopo(comm, topoInfo, algHierarchyInfo));
    return HCCL_SUCCESS;
}

/*
 * 资源计算：构建 Template 实例，调用 CalcRes
 */
template <typename AlgTopoMatch, typename InsAlgTemplate>
HcclResult InsV2AlltoAllVSoleExecutor<AlgTopoMatch, InsAlgTemplate>::CalcRes(
    HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
    const AlgHierarchyInfoForAllLevel& algHierarchyInfo, AlgResourceRequest& resourceRequest)
{
    // 构建 Template 实例
    std::shared_ptr<InsAlgTemplate> algTemplate =
        std::make_shared<InsAlgTemplate>(param, topoInfo->userRank, algHierarchyInfo.infos[0]);
    // 调用 Template 的资源计算
    CHK_RET(algTemplate->CalcRes(comm, param, topoInfo, resourceRequest));
    return HCCL_SUCCESS;
}

/*
 * Orchestrate：编排执行
 *
 * 对应 hccl 仓 InsAlltoAllVSoleExecutor::Orchestrate()
 * 流程：InitCommInfo → RestoreChannelMap → OrchestrateLoop
 */
template <typename AlgTopoMatch, typename InsAlgTemplate>
HcclResult InsV2AlltoAllVSoleExecutor<AlgTopoMatch, InsAlgTemplate>::Orchestrate(
    const OpParam& param, const AlgResourceCtxSerializable& resCtx)
{
    HCCL_INFO("[InsV2AlltoAllVSoleExecutor][Orchestrate] Start, opType[%u].", static_cast<u32>(param.opType));

    // 初始化通信信息
    myRank_ = resCtx.topoInfo.userRank;
    threads_ = resCtx.threads;

    // 恢复 channel 映射（非 AIV/CCU 引擎需要）
    if (param.engine != CommEngine::COMM_ENGINE_AIV && param.engine != CommEngine::COMM_ENGINE_CCU) {
        CHK_RET(RestoreChannelMap(resCtx, remoteRankToChannelInfo_));
    }

    // 执行编排循环
    HcclResult ret = OrchestrateLoop(param, resCtx);
    CHK_PRT_RET(ret != HCCL_SUCCESS,
                HCCL_ERROR("[InsV2AlltoAllVSoleExecutor][Orchestrate] errNo[0x%016llx] kernel run failed",
                           HCCL_ERROR_CODE(ret)),
                ret);
    return HCCL_SUCCESS;
}

/*
 * OrchestrateLoop：编排循环
 *
 * 对应 hccl 仓 InsAlltoAllVSoleExecutor::OrchestrateLoop()
 * 流程：准备资源 → 构建Template → KernelRun
 */
template <typename AlgTopoMatch, typename InsAlgTemplate>
HcclResult InsV2AlltoAllVSoleExecutor<AlgTopoMatch, InsAlgTemplate>::OrchestrateLoop(
    const OpParam& param, const AlgResourceCtxSerializable& resCtx)
{
    HCCL_INFO("[InsV2AlltoAllVSoleExecutor][OrchestrateLoop] Start");

    // 准备资源
    TemplateResource templateAlgRes;
    if (param.engine == COMM_ENGINE_CCU) {
        templateAlgRes.ccuKernels = resCtx.ccuKernels;
    }
    if (param.engine != CommEngine::COMM_ENGINE_AIV && remoteRankToChannelInfo_.size() > 0) {
        templateAlgRes.channels = remoteRankToChannelInfo_[0];
    }
    templateAlgRes.threads = resCtx.threads;
    templateAlgRes.aivCommInfoPtr = resCtx.aivCommInfoPtr;

    // 准备数据参数
    TemplateDataParams tempAlgParams;
    tempAlgParams.buffInfo.inputPtr = param.inputPtr;
    tempAlgParams.buffInfo.outputPtr = param.outputPtr;
    tempAlgParams.buffInfo.hcclBuff = resCtx.cclMem;
    tempAlgParams.buffInfo.inputSize = param.inputSize;
    tempAlgParams.buffInfo.outputSize = param.outputSize;
    tempAlgParams.buffInfo.inBuffBaseOff = 0;
    tempAlgParams.buffInfo.outBuffBaseOff = 0;
    tempAlgParams.buffInfo.hcclBuffBaseOff = 0;
    tempAlgParams.repeatNum = 1;

    // 构建 Template 实例
    InsAlgTemplate algTemplate(param, resCtx.topoInfo.userRank, resCtx.algHierarchyInfo.infos[0]);

    // 执行 KernelRun
    CHK_RET(algTemplate.KernelRun(param, tempAlgParams, templateAlgRes));

    HCCL_INFO("[InsV2AlltoAllVSoleExecutor][OrchestrateLoop] End.");
    return HCCL_SUCCESS;
}

/*
 * 注册 Executor：
 *   - opType: HCCL_CMD_HALF_ALLTOALLV
 *   - algName: CcuHalfAll2AllVMesh1D（必须与 selector 返回的字符串一致）
 *   - Executor: InsV2AlltoAllVSoleExecutor
 *   - TopoMatch: TopoMatch1D
 *   - Template: CcuTempHalfAllToAllVMesh1D
 *
 * 对应 hccl 仓的 REGISTER_EXEC_V2(HcclCMDType::HCCL_CMD_HALF_ALLTOALLV, CcuHalfAll2AllVMesh1D, ...)
 * 差异：asc-devkit 使用 MC2_CLIENT_ENABLE_CCU 守卫宏
 */
#if !defined(AICPU_COMPILE) && MC2_CLIENT_ENABLE_CCU
REGISTER_EXEC_V2(
    HcclCMDType::HCCL_CMD_HALF_ALLTOALLV, CcuHalfAll2AllVMesh1D, InsV2AlltoAllVSoleExecutor, TopoMatch1D,
    CcuTempHalfAllToAllVMesh1D);
#endif

} // namespace mc2_ops_hccl
