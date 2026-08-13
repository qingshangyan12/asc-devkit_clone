/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef HCCL_CCU_TEMP_KFC_HALF_ALL_TO_ALL_V_MESH_1D_H
#define HCCL_CCU_TEMP_KFC_HALF_ALL_TO_ALL_V_MESH_1D_H

#include "utils.h"
#include "ccu_temp_kfc_mesh_1D_mem2mem_base.h"

namespace mc2_ops_hccl {

/*
 面向 MC2 场景的变长 AllToAll。源实现拆为两个 context，每个搬运一半数据，以预留：
 1. 扩展为 64P 双 die 通过 unions 全互联的算法；
 2. 两个 context 可放在同一或不同 die 上执行。

 阶段一实现：单 mission 承担全部数据。目标仓尚无跨 kernel 变量/信号交换原语
 （ExportVariable/ImportVariable/LocalCtxPost 等），KFC Server 的 MissionPreSync/
 MissionPostSync 仍是空桩，故先走源实现注释已认可的退化路径 —— 通过调整
 sendSize & sendOffset 让单 context 搬运全部数据，等同常规 1D Fullmesh 算法。
 A/B 双片内存布局保持不变，阶段二恢复双 mission 时无需改动 host 侧参数打包。

 此外，后同步采用源端保序方式，当前最大支持 8P 通信域。
 */
class CcuTempKfcHalfAlltoAllVMesh1D : public CcuTempKfcMesh1DMem2MemBase {
public:
    explicit CcuTempKfcHalfAlltoAllVMesh1D(
        const OpParam& param, const u32 rankId, const std::vector<std::vector<u32>>& subCommRanks);

    ~CcuTempKfcHalfAlltoAllVMesh1D() override;

    std::string Describe() const override
    {
        return StringFormat(
            "Template of KFC HalfAlltoAllV Mesh1D with tempRankSize [%u].", subCommRanks_[0].size());
    }

    HcclResult CalcRes(
        HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
        AlgResourceRequest& resourceRequest) override;
    HcclResult KernelRun(
        const OpParam& param, const TemplateDataParams& templateDataParams,
        TemplateResource& templateResource) override;
    u64 CalcScratchMultiple(BufferType inBuffType, BufferType outBuffType) override;
};

} // namespace mc2_ops_hccl

#endif // HCCL_CCU_TEMP_KFC_HALF_ALL_TO_ALL_V_MESH_1D_H
