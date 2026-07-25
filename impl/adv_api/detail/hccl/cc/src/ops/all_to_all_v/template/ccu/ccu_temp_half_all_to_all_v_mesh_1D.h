/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * 本程序是自由软件，您可以根据 CANN 开源软件许可证 2.0 版的条款和条件对其进行再分发和/或修改。
 * 详情请参阅许可证。如不符合许可证要求，不得使用本软件。
 */
#ifndef HCCL_CCU_TEMP_HALF_ALL_TO_ALL_V_MESH_1D_H
#define HCCL_CCU_TEMP_HALF_ALL_TO_ALL_V_MESH_1D_H

#include "utils.h"
#include "ccu_alg_template_base.h"

namespace mc2_ops_hccl {

/*
 * HalfAllToAllVMesh1D CCU Template 类（host 侧编排）
 *
 * 对应 hccl 仓的 CcuTempHalfAllToAllVMesh1D : CcuAlgTemplateBase
 *
 * 职责：
 *   1. CalcRes()   — 计算资源需求，构造 CcuKernelInfo（双 Mission）
 *   2. KernelRun() — 组装 TaskArg，启动 2 个 Mission 的 CCU Kernel
 *
 * 关键差异（hccl → asc-devkit）：
 *   - kernelInfo.kernelFunc = reinterpret_cast<void*>(func)
 *     → kernelInfo.creator = [](arg) { return make_unique<Kernel>(arg); }
 *   - HcommCcuKernelLaunch(thread, kernel, args.data(), size)
 *     → HcclCcuKernelLaunch(comm, thread, kernel, taskArgPtr)
 *   - BuildTaskArgs() 返回 vector<uint64_t>
 *     → 构造 CcuTaskArg 对象
 */
class CcuTempHalfAllToAllVMesh1D : public CcuAlgTemplateBase {
public:
    CcuTempHalfAllToAllVMesh1D(
        const OpParam& param, const u32 rankId, const std::vector<std::vector<u32>>& subCommRanks);
    ~CcuTempHalfAllToAllVMesh1D() override;

    std::string Describe() const override
    {
        return StringFormat("Template of half all to all v ccu mesh 1D with tempRankSize [%u].", tempRankSize_);
    }

    /* 计算资源需求：双 Mission，2 个 CCU Kernel */
    HcclResult CalcRes(
        HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
        AlgResourceRequest& resourceRequest) override;

    /* 启动 CCU Kernel：构造 TaskArg，launch 2 个 Mission */
    HcclResult KernelRun(
        const OpParam& param, const TemplateDataParams& templateDataParams,
        TemplateResource& templateResource) override;

    /* 资源请求：1 个主线程 notify + 1 个从线程 */
    HcclResult GetRes(AlgResourceRequest& resourceRequest) const override;

    /* 双 Mission 需要 2 个线程 */
    u64 GetThreadNum() const override;

    /* Scratch buffer 倍数：rankSize 倍 */
    u64 CalcScratchMultiple(BufferType inBuffType, BufferType outBuffType) override;

    /* 设置本端收发信息（由 Executor 调用） */
    void SetA2ASendRecvInfo(const A2ASendRecvInfo& sendRecvInfo);

private:
    u32 mySubCommRank_{0};  // 本 rank 在子通信域中的 ID
    u32 tempRankSize_{0};   // 子通信域 rank 数量
    A2ASendRecvInfo localSendRecvInfo_; // 本端收发信息
};

} // namespace mc2_ops_hccl

#endif // HCCL_CCU_TEMP_HALF_ALL_TO_ALL_V_MESH_1D_H
