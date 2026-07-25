/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * 本程序是自由软件，您可以根据 CANN 开源软件许可证 2.0 版的条款和条件对其进行再分发和/或修改。
 * 详情请参阅许可证。如不符合许可证要求，不得使用本软件。
 */
#ifndef HCCLV2_ALLTOALLV_AUTO_SELECTOR
#define HCCLV2_ALLTOALLV_AUTO_SELECTOR

#include "auto_selector_base.h"

namespace mc2_ops_hccl {

/*
 * AlltoAllV 算法选择器
 *
 * 对应 hccl 仓的 AlltoAllVAutoSelector
 *
 * 当前实现 HALF_ALLTOALLV 分支：
 *   - opType == HCCL_CMD_HALF_ALLTOALLV
 *   - 单级 1D Mesh 拓扑
 *   - 2-8 rank
 *   → 选择 "CcuHalfAll2AllVMesh1D" 算法
 */
class AlltoAllVAutoSelector : public AutoSelectorBase {
private:
    /*
     * CCU Schedule 模式算法选择
     * 优先判断 HALF_ALLTOALLV，其他 ALLTOALLV 算法可后续补充
     */
    SelectorStatus SelectCcuScheduleAlgo(
        const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
        const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap,
        std::string& selectAlgName) const override;

    SelectorStatus SelectAicpuAlgo(
        const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
        const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap,
        std::string& selectAlgName) const override;

    SelectorStatus SelectAivAlgo(
        const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
        const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap,
        std::string& selectAlgName) const override;

    SelectorStatus SelectCcuMsAlgo(
        const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
        const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap,
        std::string& selectAlgName) const override;
};

} // namespace mc2_ops_hccl

#endif // HCCLV2_ALLTOALLV_AUTO_SELECTOR
