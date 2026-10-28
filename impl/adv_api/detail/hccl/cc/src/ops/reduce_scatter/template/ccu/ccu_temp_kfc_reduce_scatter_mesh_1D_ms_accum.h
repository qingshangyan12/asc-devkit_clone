/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef HCCL_CCU_TEMP_KFC_REDUCE_SCATTER_MESH_1D_MS_ACCUM_H
#define HCCL_CCU_TEMP_KFC_REDUCE_SCATTER_MESH_1D_MS_ACCUM_H

#include "utils.h"
#include "ccu_temp_kfc_mesh_1D_mem2mem_base.h"

namespace mc2_ops_hccl {

// KFC 直调捕获（ST）与 KFC server 继承共用的注册期元数据；InheritKfcServerKernelArg 的
// generic 分支按 subCommRanks 重建 rankSize/rankId，此处填充不影响生产 KFC 路径。
struct CcuKernelArgKfcReduceScatterMesh1DMsAccum : CcuKernelArgBase {
    uint64_t rankSize = 0;
    uint32_t rankId = 0;
    OpParam opParam;
    std::vector<std::vector<uint32_t>> subCommRanks;
};

// SoleMeshChunkTwoShot（CcuSchedReduceScatterSoleMeshChunkTwoShot）的 KFC template，
// 移植自 hccl rs-sched-perf 的 CcuTempReduceScatterMesh1DMsAccum。
// kernelFuncName 占位符由 CcuTempKfcServer::CalcRes 替换为 CcuKernelKfcServer 并复用 Mesh1D 通道；
// 每轮参数由 AIV 侧 CcuPrepareForReduceScatterTwoShotM2M 写入 HBM 单帧。
class CcuTempKfcReduceScatterMesh1DMsAccum : public CcuTempKfcMesh1DMem2MemBase {
public:
    CcuTempKfcReduceScatterMesh1DMsAccum() = default;
    explicit CcuTempKfcReduceScatterMesh1DMsAccum(
        const OpParam& param, const u32 rankId, const std::vector<std::vector<u32>>& subCommRanks);

    ~CcuTempKfcReduceScatterMesh1DMsAccum() override;

    std::string Describe() const override
    {
        return StringFormat(
            "Template of KFC ReduceScatter Mesh1D MsAccum with tempRankSize [%u].", subCommRanks_[0].size());
    }

    HcclResult CalcRes(
        HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
        AlgResourceRequest& resourceRequest) override;
    u64 CalcScratchMultiple(BufferType inBuffType, BufferType outBuffType) override;
    HcclResult KernelRun(
        const OpParam& param, const TemplateDataParams& templateDataParams,
        TemplateResource& templateResource) override;
};

} // namespace mc2_ops_hccl

#endif // HCCL_CCU_TEMP_KFC_REDUCE_SCATTER_MESH_1D_MS_ACCUM_H
