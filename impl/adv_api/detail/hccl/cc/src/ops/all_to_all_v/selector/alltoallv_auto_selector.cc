/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * 本程序是自由软件，您可以根据 CANN 开源软件许可证 2.0 版的条款和条件对其进行再分发和/或修改。
 * 详情请参阅许可证。如不符合许可证要求，不得使用本软件。
 */
#include "alltoallv_auto_selector.h"
#include "selector_registry.h"

namespace mc2_ops_hccl {

/*
 * CCU Schedule 模式算法选择
 *
 * 对应 hccl 仓 AlltoAllVAutoSelector::SelectCcuScheduleAlgo()
 *
 * HALF_ALLTOALLV 选择条件：
 *   1. opType == HCCL_CMD_HALF_ALLTOALLV
 *   2. 单级拓扑（topoLevelNums == 1）
 *   3. 1D Mesh 拓扑（level0Topo == MESH_1D）
 *   4. rank 数量 2-8（受源端保序方案限制）
 */
SelectorStatus AlltoAllVAutoSelector::SelectCcuScheduleAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)configAlgMap;

    // === HALF_ALLTOALLV 分支 ===
    if (opParam.opType == HcclCMDType::HCCL_CMD_HALF_ALLTOALLV) {
        constexpr uint32_t minRankSize = 2;
        constexpr uint32_t maxRankSize = 8;
        // 校验拓扑和 rank 数量
        if (topoInfo->topoLevelNums != 1 || topoInfo->level0Topo != Level0Shape::MESH_1D ||
            topoInfo->userRankSize < minRankSize || topoInfo->userRankSize > maxRankSize) {
            HCCL_WARNING("[AlltoAllVAutoSelector] Half alltoallv only supports 2-8 ranks in one 1D mesh. "
                         "topoLevelNums[%u], level0Topo[%u], userRankSize[%u].",
                         topoInfo->topoLevelNums, static_cast<u32>(topoInfo->level0Topo),
                         topoInfo->userRankSize);
            return SelectorStatus::NOT_MATCH;
        }
        // 选择 CcuHalfAll2AllVMesh1D 算法
        // ★ 算法名必须与 REGISTER_EXEC_V2 第二参数一致
        selectAlgName = "CcuHalfAll2AllVMesh1D";
        HCCL_INFO("[AlltoAllVAutoSelector] Half alltoallv matched, selectAlgName[%s].", selectAlgName.c_str());
        return SelectorStatus::MATCH;
    }

    // 其他 ALLTOALLV 算法可后续补充
    HCCL_WARNING("[AlltoAllVAutoSelector] opType[%u] is not supported yet for ccu schedule mode.",
                 static_cast<u32>(opParam.opType));
    return SelectorStatus::NOT_MATCH;
}

SelectorStatus AlltoAllVAutoSelector::SelectAicpuAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)topoInfo;
    (void)opParam;
    (void)configAlgMap;
    (void)selectAlgName;
    return SelectorStatus::NOT_MATCH;
}

SelectorStatus AlltoAllVAutoSelector::SelectAivAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)topoInfo;
    (void)opParam;
    (void)configAlgMap;
    (void)selectAlgName;
    return SelectorStatus::NOT_MATCH;
}

SelectorStatus AlltoAllVAutoSelector::SelectCcuMsAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)topoInfo;
    (void)opParam;
    (void)configAlgMap;
    (void)selectAlgName;
    return SelectorStatus::NOT_MATCH;
}

/*
 * 注册 Selector：
 *   - opType: HCCL_CMD_HALF_ALLTOALLV
 *   - priority: 18（与 all_gather/reduce_scatter 一致）
 *
 * ★ 同时注册 HCCL_CMD_ALLTOALLV（后续可扩展普通 AlltoAllV 算法）
 */
REGISTER_SELECTOR_BY_OPTYPE(HcclCMDType::HCCL_CMD_HALF_ALLTOALLV, 18, AlltoAllVAutoSelector);

} // namespace mc2_ops_hccl
