/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef HCCL_CCU_TEMP_ALL_TO_ALL_V_MESH_1D_H
#define HCCL_CCU_TEMP_ALL_TO_ALL_V_MESH_1D_H

#include "utils.h"
#include "ccu_alg_template_base.h"

namespace mc2_ops_hccl {

class CcuTempAlltoAllVMesh1D : public CcuAlgTemplateBase {
public:
    CcuTempAlltoAllVMesh1D() = default;
    explicit CcuTempAlltoAllVMesh1D(const OpParam& param,
                                    const u32 rankId, // 传通信域的 rankId，userRank
                                    const std::vector<std::vector<u32>> &subCommRanks);

    ~CcuTempAlltoAllVMesh1D() override;

    std::string Describe() const override
    {
        return StringFormat("Template of All to All V ccu mesh 1D with tempRankSize [%u].",
                            tempRankSize_);
    }

    HcclResult CalcRes(HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
                       AlgResourceRequest& resourceRequest) override;

    HcclResult KernelRun(const OpParam& param,
                         const TemplateDataParams& templateDataParams,
                         TemplateResource& templateResource) override;

    u64 CalcScratchMultiple(BufferType inBuffType, BufferType outBuffType) override;

    HcclResult FastLaunch(const OpParam& param, const TemplateFastLaunchCtx& tempFastLaunchCtx) override;

private:
    BuffInfo buffInfo_;
    uint32_t mySubCommRank_ = 0;
    uint32_t tempRankSize_ = 0;
};

}// namespace mc2_ops_hccl

#endif// HCCL_CCU_TEMP_ALL_TO_ALL_V_MESH_1D_H
