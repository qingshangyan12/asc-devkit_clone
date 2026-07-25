/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * 本程序是自由软件，您可以根据 CANN 开源软件许可证 2.0 版的条款和条件对其进行再分发和/或修改。
 * 详情请参阅许可证。如不符合许可证要求，不得使用本软件。
 */
#ifndef HCCL_CCU_KERNEL_HALF_ALL_TO_ALL_V_MESH1D_H
#define HCCL_CCU_KERNEL_HALF_ALL_TO_ALL_V_MESH1D_H

#include <vector>
#include <ios>
#include "utils.h"
#include "ccu_kernel.h"
#include "ccu_kernel_utils.h"
#include "ccu_kernel_alg_base.h"

namespace mc2_ops_hccl {

/*
 * HalfAllToAllVMesh1D 算法概述（从 hcomm/hccl 仓移植）：
 *
 * 该算法是一个双 Mission 并发的变长 AllToAllV CCU 算法，
 * 将数据分为前后两半（A/B），由两个 CCU Mission 分别搬运。
 *
 * - Mission 0：搬运前半数据（sendSizeA / sendOffsetA）
 * - Mission 1：搬运后半数据（sendSizeB / sendOffsetB），目标地址紧接 A 之后
 *
 * 两个 Mission 并发执行，通过 MissionSync 互相等待完成后，
 * 仅由 Mission 0 执行跨 rank 后同步（PostSync）。
 *
 * 支持场景：MC2 场景，2-8 rank 的 1D Mesh 拓扑。
 */

/*
 * CCU Kernel 参数（host 侧构造，通过 CcuKernelInfo 传入）
 *
 * 对应 hccl 仓的 CcuKernelArgHalfAllToAllVMesh1D : CcuKernelArgBase
 * 差异：asc-devkit 继承 hcomm::CcuKernelArg，需实现 GetKernelSignature()
 */
class CcuKernelArgHalfAllToAllVMesh1D : public hcomm::CcuKernelArg {
public:
    explicit CcuKernelArgHalfAllToAllVMesh1D(
        uint64_t rankSize, uint32_t rankId, uint32_t missionId, const OpParam& opParam,
        const std::vector<std::vector<uint32_t>>& subCommRanks)
        : rankSize_(rankSize), rankId_(rankId), missionId_(missionId), opParam_(opParam), subCommRanks_(subCommRanks)
    {
        HCCL_DEBUG("[CcuKernelArgHalfAllToAllVMesh1D] rankSize: %lu, rankId: %u, missionId: %u",
                   rankSize_, rankId_, missionId_);
    }

    /* 生成 Kernel 签名，用于 CCU 资源复用判断 */
    hcomm::CcuKernelSignature GetKernelSignature() const override
    {
        hcomm::CcuKernelSignature signature;
        GenerateCcuKernelSignature(signature, "CcuKernelArgHalfAllToAllVMesh1D", opParam_, subCommRanks_);
        return signature;
    }

    uint64_t rankSize_;                              // 通信域 rank 数量
    uint32_t rankId_;                                // 本 rank 在子通信域中的 ID
    uint32_t missionId_;                             // Mission 编号：0=搬运前半，1=搬运后半
    OpParam opParam_;                                // 算子参数（含数据类型、缓冲区等）
    std::vector<std::vector<uint32_t>> subCommRanks_; // 子通信域 rank 列表
};

/*
 * CCU 任务参数（每次 KernelRun 时构造，通过 HcclCcuKernelLaunch 传入）
 *
 * 对应 hccl 仓的 BuildTaskArgs() 返回的 vector<uint64_t>
 * 差异：asc-devkit 用 CcuTaskArg 对象封装，GeneArgs() 内部转换为 uint64_t 数组
 */
class CcuTaskArgHalfAllToAllVMesh1D : public hcomm::CcuTaskArg {
public:
    explicit CcuTaskArgHalfAllToAllVMesh1D(
        uint64_t inputAddr, uint64_t cclBufferAddr, uint64_t token, uint64_t sendSizesAddr,
        uint64_t sendOffsetsAddr, uint64_t recvOffset, uint64_t localDataSize)
        : inputAddr_(inputAddr), cclBufferAddr_(cclBufferAddr), token_(token), sendSizesAddr_(sendSizesAddr),
          sendOffsetsAddr_(sendOffsetsAddr), recvOffset_(recvOffset), localDataSize_(localDataSize)
    {
        HCCL_DEBUG("[CcuTaskArgHalfAllToAllVMesh1D] inputAddr: %lu, cclBufferAddr: %lu, "
                   "sendSizesAddr: %lu, sendOffsetsAddr: %lu, recvOffset: %lu, localDataSize: %lu",
                   inputAddr_, cclBufferAddr_, sendSizesAddr_, sendOffsetsAddr_, recvOffset_, localDataSize_);
    }

    uint64_t inputAddr_;       // 用户输入数据地址（对应 AIC 侧 xnData[1]）
    uint64_t cclBufferAddr_;   // 本端 CCL buffer 地址（windowsOut[0]）
    uint64_t token_;           // 内存 token
    uint64_t sendSizesAddr_;   // 分片大小数组地址（对应 AIC 侧 xnData[2]）
    uint64_t sendOffsetsAddr_; // 分片偏移数组地址（对应 AIC 侧 xnData[3]）
    uint64_t recvOffset_;      // 接收偏移（对应 AIC 侧 xnData[8] remoteWinOffset）
    uint64_t localDataSize_;   // 本端数据量（用于计算 GroupOpSize）
};

/*
 * HalfAllToAllVMesh1D CCU Kernel 类
 *
 * 对应 hccl 仓的：
 *   - HalfAllToAllVMesh1DContext（Context 结构体）+ CcuHalfAllToAllVMesh1DKernel（入口函数）
 * 差异：asc-devkit 将 Context 和入口合并为类，继承 CcuKernelAlgBase
 *
 * 算法流程（Algorithm 方法）：
 *   InitResource → LoadArgs → PreSync → DoHalfAllToAll → MissionSync → PostSync
 */
class CcuKernelHalfAllToAllVMesh1D : public CcuKernelAlgBase {
public:
    CcuKernelHalfAllToAllVMesh1D(const hcomm::CcuKernelArg& arg);
    ~CcuKernelHalfAllToAllVMesh1D() override {}

    /* 核心算法入口，由 CCU 框架调度执行 */
    HcclResult Algorithm() override;

    /* 将 TaskArg 转换为 SQE 参数数组，顺序必须与 LoadArgs() 中的 Load() 顺序一致 */
    std::vector<uint64_t> GeneArgs(const hcomm::CcuTaskArg& arg) override;

private:
    /* 初始化 CCU 资源：创建 Variable、LocalAddr、RemoteAddr、CompletedEvent 等 */
    HcclResult InitResource();

    /* 从 SQE 加载基础参数，从 HBM 加载分片参数（sendSizeA/B、sendOffsetA/B） */
    void LoadArgs();

    /* 前同步：通过 NotifyRecord 将本端 cclBuffer 地址和 token 交换给所有对端 */
    void PreSync();

    /* 核心数据搬运：双 Mission 并发，Mi0 搬前半 A，Mi1 搬后半 B */
    void DoHalfAllToAll();

    /* 跨 Mission 同步：Mi0 和 Mi1 互相等待对方搬运完成 */
    void MissionSync();

    /* 跨 rank 后同步：仅 Mi0 执行，通知所有对端并等待确认 */
    void PostSync();

    /* === 基本配置 === */
    uint64_t rankSize_{0};  // 通信域 rank 数量
    uint32_t rankId_{0};    // 本 rank ID
    uint32_t missionId_{0}; // Mission 编号：0 或 1
    uint32_t signalNum_{0}; // 信号分组数（每 16 rank 一组，仅用于逻辑计算）

    /* === 运行时参数（从 SQE 加载） === */
    // 对应 hccl 的 ccu::Variable → asc-devkit 的 CcuRep::Variable
    CcuRep::Variable userInAddr_;       // 用户输入数据地址
    CcuRep::Variable cclBufferAddr_;    // 本端 CCL buffer 地址
    CcuRep::Variable myToken_;          // 本端内存 token
    CcuRep::Variable sendSizesAddr_;    // 分片大小数组地址（HBM 中连续存储）
    CcuRep::Variable sendOffsetsAddr_;  // 分片偏移数组地址（HBM 中连续存储）
    CcuRep::Variable recvOffset_;       // 接收偏移（相对对端 cclBuffer 首地址）
    GroupOpSize groupOpSize_;           // 本端 GroupCopy 参数（分块搬运）

    /* === 数据分片（从 HBM 加载） === */
    // 每个 rank 的数据分为前半 A 和后半 B，各 rankSize 个
    std::vector<CcuRep::Variable> sendSizeA_;     // 前半分片大小（字节）
    std::vector<CcuRep::Variable> sendSizeB_;     // 后半分片大小（字节）
    std::vector<CcuRep::Variable> sendOffsetA_;   // 前半分片偏移（字节）
    std::vector<CcuRep::Variable> sendOffsetB_;   // 后半分片偏移（字节）

    /* === 远端资源（通过 channel 获取） === */
    // 对应 hccl 的 ccu::GetResByChannel<Variable>(channel, xnId)
    // asc-devkit 用 CreateVariable(channel, xnId, &var)
    std::vector<CcuRep::Variable> remoteCclBuffer_; // 远端 CCL buffer 地址（每个 rank 一个）
    std::vector<CcuRep::Variable> remoteToken_;      // 远端 token（每个 rank 一个）

    /* === 地址句柄 === */
    CcuRep::LocalAddr src_;                       // 源地址（本端）
    std::vector<CcuRep::RemoteAddr> dst_;         // 目标地址（每个 rank 一个）

    /*
     * 写完成事件
     *
     * ★ 关键差异：
     *   hccl 用 ccu::Event + bit mask（每 16 rank 共享一个 Event，每个 rank 占 1 bit）
     *   asc-devkit 用 CcuRep::CompletedEvent（无 bit mask，每个 rank 需独立 Event）
     *   因此数组大小从 signalNum 变为 rankSize
     */
    std::vector<CcuRep::CompletedEvent> writeDoneEvents_;
};

} // namespace mc2_ops_hccl

#endif // HCCL_CCU_KERNEL_HALF_ALL_TO_ALL_V_MESH1D_H
