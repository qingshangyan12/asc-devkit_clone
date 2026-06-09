/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#ifndef HCCLV2_ALL_GATHER_SOLE_EXECUTOR
#define HCCLV2_ALL_GATHER_SOLE_EXECUTOR

#include <string>
#include <vector>
#include <map>

#include "../../../../../../../../../../../../../../include/adv_api/hccl/internal/hcomm/hccl/hccl_types.h"
#include "../../../../../../../../../../../../../../include/adv_api/hccl/internal/hcomm/pkg_inc/hccl/base.h"
#include "impl/adv_api/detail/hccl/cc/src/aicpu_kfc/legacy/common/types/types.h"
#include "impl/adv_api/detail/hccl/cc/src/aicpu_kfc/legacy/service/collective/coll_alg_params.h"
#include "impl/adv_api/detail/hccl/cc/src/aicpu_kfc/legacy/framework/resource_manager/transport/aicpu/connected_link_mgr.h"
#include "template_utils.h"
#include "impl/adv_api/detail/hccl/cc/src/aicpu_kfc/legacy/service/collective/alg/coll_alg_factory/alg_template/prim_alg_template/alg_template_base_v2.h"
#include "impl/adv_api/detail/hccl/cc/src/aicpu_kfc/legacy/include/hccl_params_pub.h"
#include "coll_alg_base.h"
#include "impl/adv_api/detail/hccl/cc/src/aicpu_kfc/legacy/framework/topo/new_topo_builder/rank_graph/rank_gph.h"
#include "impl/adv_api/detail/hccl/cc/src/aicpu_kfc/legacy/service/collective/coll_operator.h"

namespace Hccl {

template <typename AlgTopoMatch, typename AlgTemplate> class AllGatherSoleExecutor : public CollAlgBase {
public:
    explicit AllGatherSoleExecutor();
    ~AllGatherSoleExecutor() override;

    std::string Describe() const override
    {
        return "All Gather Sole Executor.";
    }

    HcclResult GenPrimQues(const RankGraph *rankGraph, const CollAlgOperator &op, const CollAlgParams &params,
                           PrimQuePtr primQue) override;
    HcclResult CalcResOffload(const RankGraph *rankGraph, const u64 &dataSize,
                              CollOffloadOpResReq &resReq) override;
    HcclResult CalcRes(const RankGraph *rankGraph, CollAlgResReq &algResReq) override;

    HcclResult GenPrimQuesAIC(const AlgTopoInfo &topoInfo, const CollAlgOperator &op, const CollAlgParams &params,
                              ConnectedLinkMgr *linkMgr, PrimQuePtr primQue) override;

private:
    HcclResult GenPrimQues4Offload(AlgTemplateBase &tempAlg);
    HcclResult GenPrimQues4Opbase(const u32 dataSizePerVolume, AlgTemplateBase &tempAlg);

    std::vector<RankId>              virtRanks_;
    std::map<RankId, u32>            virtRankMap_; // map<virtRank, virtRankOrder>
    std::vector<std::vector<RankId>> vTopo_;

    std::vector<PrimQuePtr> requiredQue_;
    ResLinks                tempResLinks_;
};

} // namespace Hccl

#endif // HCCLV2_ALL_GATHER_SOLE_EXECUTOR
