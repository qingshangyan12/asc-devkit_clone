/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * 本程序是自由软件，您可以根据 CANN 开源软件许可证 2.0 版的条款和条件对其进行再分发和/或修改。
 * 详情请参阅许可证。如不符合许可证要求，不得使用本软件。
 */
#include "channel.h"
#include "hccl_ccu_res.h"
#include "ccu_assist_pub.h"
#include "kernel/ccu_kernel_half_all_to_all_v_mesh1d.h"
#include "ccu_temp_half_all_to_all_v_mesh_1D.h"

namespace mc2_ops_hccl {

/* 双 Mission 数量 */
constexpr u32 MISSION_NUM = 2;
/* 最大支持的 rank 数（受源端保序方案限制） */
constexpr u32 MAX_SUPPORTED_RANK_SIZE = 8;

/*
 * 构造函数：从子通信域 rank 列表初始化
 *
 * 对应 hccl 仓 CcuTempHalfAllToAllVMesh1D 构造函数
 */
CcuTempHalfAllToAllVMesh1D::CcuTempHalfAllToAllVMesh1D(
    const OpParam& param, const u32 rankId, const std::vector<std::vector<u32>>& subCommRanks)
    : CcuAlgTemplateBase(param, rankId, subCommRanks)
{
    if (subCommRanks.empty()) {
        return;
    }
    tempRankSize_ = subCommRanks[0].size();
    // 查找本 rank 在子通信域中的位置
    auto it = std::find(subCommRanks[0].begin(), subCommRanks[0].end(), rankId);
    if (it != subCommRanks[0].end()) {
        mySubCommRank_ = std::distance(subCommRanks[0].begin(), it);
    }
}

CcuTempHalfAllToAllVMesh1D::~CcuTempHalfAllToAllVMesh1D() {}

/*
 * CalcRes：计算资源需求，构造双 Mission 的 CcuKernelInfo
 *
 * 对应 hccl 仓 CcuTempHalfAllToAllVMesh1D::CalcRes()
 * 关键差异：
 *   - hccl: kernelInfo.kernelFunc = reinterpret_cast<void*>(CcuHalfAllToAllVMesh1DKernel)
 *     → asc-devkit: kernelInfo.creator = [](arg) { return make_unique<CcuKernelHalfAllToAllVMesh1D>(arg); }
 *   - hccl: kernelInfo.setKernelArg(kernelArg)
 *     → asc-devkit: kernelInfo.kernelArg = std::make_shared<...>(...)
 */
HcclResult CcuTempHalfAllToAllVMesh1D::CalcRes(
    HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
    AlgResourceRequest& resourceRequest)
{
    // 校验：仅支持 2-8 rank 的 1D Mesh
    CHK_PRT_RET(subCommRanks_.size() != 1 || subCommRanks_[0].size() < MISSION_NUM ||
                    subCommRanks_[0].size() > MAX_SUPPORTED_RANK_SIZE,
                HCCL_ERROR("[CcuTempHalfAllToAllVMesh1D::CalcRes] Only 2-8 ranks in one 1D mesh are supported."),
                HCCL_E_NOT_SUPPORT);

    // 资源请求：1 个主线程 notify + 1 个从线程
    resourceRequest.notifyNumOnMainThread = 1;
    resourceRequest.slaveThreadNum = 1;
    resourceRequest.notifyNumPerThread.assign(resourceRequest.slaveThreadNum, 1);
    // ★ 双 Mission：ccuKernelNum = 2
    resourceRequest.ccuKernelNum.push_back(MISSION_NUM);

    // 计算 1D Mesh channel
    std::vector<HcclChannelDesc> channelDescs;
    CHK_RET(CalcChannelRequestMesh1D(comm, param, topoInfo, subCommRanks_, channelDescs));
    CHK_PRT_RET(channelDescs.size() != tempRankSize_ - 1,
                HCCL_ERROR("[CcuTempHalfAllToAllVMesh1D::CalcRes] Channel count[%zu] != rankSize-1[%u].",
                           channelDescs.size(), tempRankSize_ - 1),
                HCCL_E_INTERNAL);

    // 为每个 Mission 创建 CcuKernelInfo
    for (u32 missionId = 0; missionId < MISSION_NUM; ++missionId) {
        CcuKernelInfo kernelInfo;
        // ★ 关键差异：asc-devkit 用 creator lambda 创建 Kernel 实例
        //   hccl 用 kernelInfo.kernelFunc = reinterpret_cast<void*>(func)
        kernelInfo.creator = [](const hcomm::CcuKernelArg& arg) {
            return std::make_unique<CcuKernelHalfAllToAllVMesh1D>(arg);
        };
        // 构造 Kernel 参数
        kernelInfo.kernelArg = std::make_shared<CcuKernelArgHalfAllToAllVMesh1D>(
            tempRankSize_, mySubCommRank_, missionId, param, subCommRanks_);
        kernelInfo.channels = channelDescs;
        resourceRequest.ccuKernelInfos.push_back(kernelInfo);
    }

    HCCL_INFO("[CcuTempHalfAllToAllVMesh1D::CalcRes] rankSize[%u], rankId[%u], channels[%zu], kernels[%u].",
              tempRankSize_, mySubCommRank_, channelDescs.size(), MISSION_NUM);
    return HCCL_SUCCESS;
}

/*
 * KernelRun：启动双 Mission 的 CCU Kernel
 *
 * 对应 hccl 仓 CcuTempHalfAllToAllVMesh1D::KernelRun()
 * 关键差异：
 *   - hccl: BuildTaskArgs() 返回 vector<uint64_t> → HcommCcuKernelLaunch(thread, kernel, args.data(), size)
 *     → asc-devkit: 构造 CcuTaskArg 对象 → HcclCcuKernelLaunch(comm, thread, kernel, taskArgPtr)
 */
HcclResult CcuTempHalfAllToAllVMesh1D::KernelRun(
    const OpParam& param, const TemplateDataParams& templateDataParams, TemplateResource& templateResource)
{
    // 校验资源充足
    CHK_PRT_RET(templateResource.threads.size() < MISSION_NUM || templateResource.ccuKernels.size() < MISSION_NUM,
                HCCL_ERROR("[CcuTempHalfAllToAllVMesh1D::KernelRun] Insufficient thread[%zu] or kernel[%zu] resources.",
                           templateResource.threads.size(), templateResource.ccuKernels.size()),
                HCCL_E_INTERNAL);

    buffInfo_ = templateDataParams.buffInfo;

    // 获取输入/输出地址
    uint64_t inputAddr = PointerToAddr(buffInfo_.inputPtr) + buffInfo_.inBuffBaseOff;
    uint64_t cclBufferAddr = PointerToAddr(buffInfo_.hcclBuff.addr) + buffInfo_.hcclBuffBaseOff;
    uint64_t token = 0;
    CHK_RET(GetToken(buffInfo_, token));

    // 构造 TaskArg
    // 两个 Mission 使用相同的 TaskArg，Kernel 内部根据 missionId 选择 A/B 分片
    auto taskArg = std::make_unique<CcuTaskArgHalfAllToAllVMesh1D>(
        inputAddr,                          // 用户输入地址
        cclBufferAddr,                      // CCL buffer 地址
        token,                              // 内存 token
        reinterpret_cast<uint64_t>(param.all2AllVDataDes.sendCounts),  // 分片大小数组地址
        reinterpret_cast<uint64_t>(param.all2AllVDataDes.sdispls),     // 分片偏移数组地址
        0,                                  // recvOffset（由 remoteWinOffset 决定）
        param.inputSize);                   // 本端数据量

    void* taskArgPtr = static_cast<void*>(taskArg.get());

    // 启动双 Mission
    for (u32 missionId = 0; missionId < MISSION_NUM; ++missionId) {
        CHK_RET(HcclCcuKernelLaunch(param.hcclComm, templateResource.threads[missionId],
                                     templateResource.ccuKernels[missionId], taskArgPtr));
    }

    HCCL_INFO("[CcuTempHalfAllToAllVMesh1D::KernelRun] success, inputAddr[%lu], cclBufferAddr[%lu].",
              inputAddr, cclBufferAddr);
    return HCCL_SUCCESS;
}

void CcuTempHalfAllToAllVMesh1D::SetA2ASendRecvInfo(const A2ASendRecvInfo& sendRecvInfo)
{
    localSendRecvInfo_ = sendRecvInfo;
}

u64 CcuTempHalfAllToAllVMesh1D::CalcScratchMultiple(BufferType inBuffType, BufferType outBuffType)
{
    (void)inBuffType;
    (void)outBuffType;
    // Scratch buffer 需要 rankSize 倍空间
    return tempRankSize_;
}

u64 CcuTempHalfAllToAllVMesh1D::GetThreadNum() const
{
    return MISSION_NUM;
}

HcclResult CcuTempHalfAllToAllVMesh1D::GetRes(AlgResourceRequest& resourceRequest) const
{
    resourceRequest.slaveThreadNum = 1;
    resourceRequest.notifyNumPerThread.assign(resourceRequest.slaveThreadNum, 1);
    resourceRequest.notifyNumOnMainThread = 1;
    return HCCL_SUCCESS;
}

} // namespace mc2_ops_hccl
