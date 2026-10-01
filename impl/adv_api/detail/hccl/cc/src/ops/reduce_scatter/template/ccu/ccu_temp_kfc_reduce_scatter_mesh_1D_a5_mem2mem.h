/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef HCCL_CCU_TEMP_KFC_REDUCE_SCATTER_MESH_1D_A5_MEM2MEM_H
#define HCCL_CCU_TEMP_KFC_REDUCE_SCATTER_MESH_1D_A5_MEM2MEM_H

#include "utils.h"
#include "ccu_temp_kfc_mesh_1D_mem2mem_base.h"

namespace mc2_ops_hccl {

// A5(Ascend950) 单server 2~8卡专用 KFC ReduceScatter template（双缓冲流水线版）。
// 自 hccl CcuTempReduceScatterMesh1DA5Mem2Mem 移植：CalcRes 注册 kernelFuncName 供
// CcuTempKfcServer::CalcRes 识别并继承为 KFC server mission；KernelRun 空实现（per-round
// 参数由 AIV CcuPrepareForReduceScatterSoleMeshA5M2M 写入 HBM，KFC server 读取）。
struct CcuKernelArgKfcReduceScatterMesh1DA5Mem2Mem : CcuKernelArgBase {
    uint64_t rankSize = 0;
    uint32_t rankId = 0;
    OpParam opParam;
    std::vector<std::vector<uint32_t>> subCommRanks;
};

class CcuTempKfcReduceScatterMesh1DA5Mem2Mem : public CcuTempKfcMesh1DMem2MemBase {
public:
    CcuTempKfcReduceScatterMesh1DA5Mem2Mem() = default;
    explicit CcuTempKfcReduceScatterMesh1DA5Mem2Mem(
        const OpParam& param, const u32 rankId, const std::vector<std::vector<u32>>& subCommRanks);

    ~CcuTempKfcReduceScatterMesh1DA5Mem2Mem() override;

    std::string Describe() const override
    {
        return StringFormat(
            "Template of KFC ReduceScatter Mesh1D A5 Mem2Mem with tempRankSize [%u].", subCommRanks_[0].size());
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

#endif // HCCL_CCU_TEMP_KFC_REDUCE_SCATTER_MESH_1D_A5_MEM2MEM_H
