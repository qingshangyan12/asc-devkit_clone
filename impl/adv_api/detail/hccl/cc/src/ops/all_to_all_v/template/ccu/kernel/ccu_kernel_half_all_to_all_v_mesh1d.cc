/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * 本程序是自由软件，您可以根据 CANN 开源软件许可证 2.0 版的条款和条件对其进行再分发和/或修改。
 * 详情请参阅许可证。如不符合许可证要求，不得使用本软件。
 */
#include "ccu_kernel_half_all_to_all_v_mesh1d.h"
#include "ccu_kernel_alg_base.h"

namespace mc2_ops_hccl {
using namespace hcomm;

/*
 * ==================== 常量定义 ====================
 * 与 hccl 仓保持一致的算法常量
 */
constexpr uint32_t MISSION_NUM = 2;           // 双 Mission 并发
constexpr uint32_t RANK_NUM_PER_CKE = 16;     // 每个 CKE 有 16 个 bit（仅用于逻辑计算）
constexpr uint32_t MAX_SUPPORTED_RANK_SIZE = 8; // 最大支持的 rank 数（受源端保序方案限制）
constexpr uint32_t OUTPUT_XN_ID = 1;          // cclBuffer 地址在 channel 中的 XN 索引
constexpr uint32_t TOKEN_XN_ID = 2;           // token 在 channel 中的 XN 索引
constexpr uint32_t CKE_IDX_0 = 0;             // CKE 索引 0：用于 PreSync
constexpr uint32_t CKE_IDX_1 = 1;             // CKE 索引 1：用于 MissionSync 和 PostSync
constexpr uint32_t XN_LENGTH = 8;             // 每个 XN 参数占 8 字节
constexpr uint16_t OUTPUT_NOTIFY_BIT = 1U << OUTPUT_XN_ID;
constexpr uint16_t TOKEN_NOTIFY_BIT = 1U << TOKEN_XN_ID;

/*
 * ==================== 构造函数 ====================
 * 从 CcuKernelArg 解析参数，初始化基本配置
 *
 * 对应 hccl 仓的 ParseKernelArg() 函数
 */
CcuKernelHalfAllToAllVMesh1D::CcuKernelHalfAllToAllVMesh1D(const CcuKernelArg& arg) : CcuKernelAlgBase(arg)
{
    const CcuKernelArgHalfAllToAllVMesh1D* kernelArg = dynamic_cast<const CcuKernelArgHalfAllToAllVMesh1D*>(&arg);
    rankSize_ = kernelArg->rankSize_;
    rankId_ = kernelArg->rankId_;
    missionId_ = kernelArg->missionId_;
    channels_ = kernelArg->channels;
    // 信号分组数：每 16 个 rank 共享一组信号（仅用于逻辑计算，实际用 per-rank Event）
    signalNum_ = (rankSize_ + RANK_NUM_PER_CKE - 1) / RANK_NUM_PER_CKE;

    HCCL_INFO("[CcuKernelHalfAllToAllVMesh1D] Init, rankId[%u], rankSize[%u], missionId[%u], signalNum[%u]",
              rankId_, rankSize_, missionId_, signalNum_);
}

/*
 * ==================== InitResource ====================
 * 初始化 CCU 资源：创建 Variable、LocalAddr、RemoteAddr、CompletedEvent
 *
 * 对应 hccl 仓的 InitResource() 函数
 * 关键差异：
 *   - hccl: ccu::GetResByChannel<ccu::Variable>(channel, xnId) 获取远端资源
 *   - asc-devkit: CreateVariable(channel, xnId, &var) 获取远端资源
 *   - hccl: writeDoneEvents 大小为 signalNum（共享 + bit mask）
 *   - asc-devkit: writeDoneEvents 大小为 rankSize（每 rank 独立，无 bit mask）
 */
HcclResult CcuKernelHalfAllToAllVMesh1D::InitResource()
{
    // 分配分片参数数组
    sendSizeA_.resize(rankSize_);
    sendSizeB_.resize(rankSize_);
    sendOffsetA_.resize(rankSize_);
    sendOffsetB_.resize(rankSize_);
    remoteCclBuffer_.resize(rankSize_);
    remoteToken_.resize(rankSize_);
    dst_.resize(rankSize_);

    // ★ 关键差异：asc-devkit 的 CompletedEvent 无 bit mask，每个 rank 需要独立 Event
    //   hccl 中 writeDoneEvents_[signalNum]，16 rank 共享一个 Event + bit mask
    //   asc-devkit 中 writeDoneEvents_[rankSize]，每 rank 独立 Event
    writeDoneEvents_ = CreateBlockCompletedEvent(rankSize_);

    // 从 channel 获取远端资源（cclBuffer 地址和 token）
    // 按 rank 号从小到大遍历，跳过本 rank，依次从 channel 获取远端资源
    uint32_t channelIdx = 0;
    if (channels_.size() != rankSize_ - 1) {
        HCCL_ERROR("[CcuKernelHalfAllToAllVMesh1D] channels size[%zu] != rankSize-1[%lu]",
                   channels_.size(), rankSize_ - 1);
        return HcclResult::HCCL_E_INTERNAL;
    }
    for (uint64_t peerId = 0; peerId < rankSize_; peerId++) {
        if (peerId == rankId_) {
            continue; // 本 rank 跳过，不需要远端资源
        }
        // 对应 hccl: ccu::GetResByChannel<ccu::Variable>(arg->channels[channelIdx], OUTPUT_XN_ID)
        CHK_RET(CreateVariable(channels_[channelIdx], OUTPUT_XN_ID, &remoteCclBuffer_[peerId]));
        CHK_RET(CreateVariable(channels_[channelIdx], TOKEN_XN_ID, &remoteToken_[peerId]));
        channelIdx++;
    }

    // 创建本地 Variable 资源
    userInAddr_ = CreateVariable();
    cclBufferAddr_ = CreateVariable();
    myToken_ = CreateVariable();
    sendSizesAddr_ = CreateVariable();
    sendOffsetsAddr_ = CreateVariable();
    recvOffset_ = CreateVariable();
    groupOpSize_ = CreateGroupOpSize();

    // 创建本地地址句柄
    src_ = CreateLocalAddr();

    HCCL_INFO("[CcuKernelHalfAllToAllVMesh1D::InitResource] success, rankSize[%lu], channels[%zu]",
              rankSize_, channels_.size());
    return HcclResult::HCCL_SUCCESS;
}

/*
 * ==================== LoadArgs ====================
 * 从 SQE 加载基础参数，从 HBM 加载分片参数
 *
 * 对应 hccl 仓的 LoadArgs() + LoadHalfInfoFromMem() 函数
 * 关键差异：
 *   - hccl: ccu::LoadArg(var, index) 按索引从 taskArgs 加载
 *   - asc-devkit: Load(var) 按 GeneArgs 返回的顺序加载
 *   - hccl: ccu::Load(addr, var) 从内存加载
 *   - asc-devkit: LoadVariable(addr, var) 从内存加载
 */
void CcuKernelHalfAllToAllVMesh1D::LoadArgs()
{
    // === 加载基础参数（从 SQE，顺序与 GeneArgs 返回值一致） ===
    Load(userInAddr_);       // 参数 0：用户输入地址
    Load(cclBufferAddr_);    // 参数 1：CCL buffer 地址
    Load(myToken_);          // 参数 2：内存 token
    Load(sendSizesAddr_);    // 参数 3：分片大小数组地址
    Load(sendOffsetsAddr_);  // 参数 4：分片偏移数组地址
    Load(recvOffset_);       // 参数 5：接收偏移
    Load(groupOpSize_);      // 参数 6-9：GroupOpSize（4 个值）

    // === 从 HBM 连续加载分片参数 ===
    // HBM 中存储格式：[sendSizeA[0..N-1], sendSizeB[0..N-1],
    //                  sendOffsetA[0..N-1], sendOffsetB[0..N-1]]
    // 每个 XN 占 8 字节
    // 注意：CcuRep::Variable 的 operator+= 只接受 Variable 类型，
    // 所以用 LoadVariable(uint64_t addr, var) 的重载，逐个计算偏移地址
    uint64_t baseAddr = 0; // 占位，实际运行时由框架从 sendSizesAddr_ 解析

    // 加载前半分片大小 sendSizeA[]
    for (uint64_t peerId = 0; peerId < rankSize_; peerId++) {
        // LoadVariable 的第一个参数是 Variable（存放内存地址），第二个是目标 Variable
        // 这里用 sendSizesAddr_ 作为基地址，通过 CreateVariable 获取临时地址变量递增
        LoadVariable(sendSizesAddr_, sendSizeA_[peerId]);
        // 地址递增：sendSizesAddr_ += XN_LENGTH（用 Variable 运算）
        sendSizesAddr_ = sendSizesAddr_ + CreateVariable();
    }
    // 加载后半分片大小 sendSizeB[]
    for (uint64_t peerId = 0; peerId < rankSize_; peerId++) {
        LoadVariable(sendSizesAddr_, sendSizeB_[peerId]);
        sendSizesAddr_ = sendSizesAddr_ + CreateVariable();
    }

    // 加载前半分片偏移 sendOffsetA[]
    for (uint64_t peerId = 0; peerId < rankSize_; peerId++) {
        LoadVariable(sendOffsetsAddr_, sendOffsetA_[peerId]);
        sendOffsetsAddr_ = sendOffsetsAddr_ + CreateVariable();
    }
    // 加载后半分片偏移 sendOffsetB[]
    for (uint64_t peerId = 0; peerId < rankSize_; peerId++) {
        LoadVariable(sendOffsetsAddr_, sendOffsetB_[peerId]);
        sendOffsetsAddr_ = sendOffsetsAddr_ + CreateVariable();
    }

    HCCL_DEBUG("[CcuKernelHalfAllToAllVMesh1D::LoadArgs] success, missionId[%u]", missionId_);
}

/*
 * ==================== PreSync ====================
 * 前同步：将本端 cclBuffer 地址和 token 发送给所有对端
 *
 * 对应 hccl 仓的 PreSync() 函数
 * 关键差异：
 *   - hccl: ccu::WriteVariableWithNotify(channel, var, xnId, missionId, bit)
 *   - asc-devkit: NotifyRecord(channel, notifyIdx, varIdx, var, mask)
 *   参数顺序不同，但语义相同：写变量到对端 channel 并通知
 */
void CcuKernelHalfAllToAllVMesh1D::PreSync()
{
    // 构建本端 cclBuffer + recvOffset 的地址变量，发送给对端
    CcuRep::Variable localDst = cclBufferAddr_;
    localDst += recvOffset_;

    // 向每个对端发送 cclBuffer 地址和 token
    uint32_t channelIdx = 0;
    for (uint64_t peerId = 0; peerId < rankSize_; peerId++) {
        if (peerId == rankId_) {
            continue;
        }
        // 对应 hccl: ccu::WriteVariableWithNotify(ch, localDst, OUTPUT_XN_ID, missionId, OUTPUT_NOTIFY_BIT)
        // asc-devkit: NotifyRecord(channel, ckeIdx, varIdx, var, mask)
        NotifyRecord(channels_[channelIdx], CKE_IDX_0, OUTPUT_XN_ID, localDst, OUTPUT_NOTIFY_BIT);
        NotifyRecord(channels_[channelIdx], CKE_IDX_0, TOKEN_XN_ID, myToken_, TOKEN_NOTIFY_BIT);
        channelIdx++;
    }

    // 等待所有对端的 cclBuffer 地址和 token 到位
    uint16_t allBits = OUTPUT_NOTIFY_BIT | TOKEN_NOTIFY_BIT;
    for (ChannelHandle channel : channels_) {
        NotifyWait(channel, CKE_IDX_0, allBits);
    }

    HCCL_DEBUG("[CcuKernelHalfAllToAllVMesh1D::PreSync] success, missionId[%u]", missionId_);
}

/*
 * ==================== DoHalfAllToAll ====================
 * 核心数据搬运：双 Mission 并发
 *
 * 对应 hccl 仓的 DoHalfAllToAll() 函数
 * 关键差异：
 *   - hccl: ccu::Write(ch, dst, src, size, event, bit) — 有 bit 参数
 *   - asc-devkit: WriteNb(ch, dst, src, len, event) — 无 bit 参数，每 rank 独立 event
 *   - hccl: ccu::EventRecord(event, bit) — 有 bit 参数
 *   - asc-devkit: RecordEvent(event) — 无 bit 参数
 *   - hccl: ccu::EventWait(event, waitBits) — 分组等待
 *   - asc-devkit: WaitEvent(event) — 逐个等待
 *   - hccl: ccu::LocalCopy(dst, src, size, event, bit)
 *   - asc-devkit: GroupCopy(dst, src, goSize) — 复用 all_gather 的 GroupCopy 模式
 */
void CcuKernelHalfAllToAllVMesh1D::DoHalfAllToAll()
{
    uint32_t channelIdx = 0;
    for (uint64_t peerId = 0; peerId < rankSize_; peerId++) {
        // === 根据 missionId 选择前半(A)或后半(B)分片 ===
        // Mi0 搬前半 A，Mi1 搬后半 B
        CcuRep::Variable sendSize = (missionId_ == 0) ? sendSizeA_[peerId] : sendSizeB_[peerId];
        CcuRep::Variable sendOffset = (missionId_ == 0) ? sendOffsetA_[peerId] : sendOffsetB_[peerId];

        // === 构建源地址 ===
        // src = userInAddr + sendOffset
        src_.addr = userInAddr_;
        src_.addr += sendOffset;
        src_.token = myToken_;

        if (peerId == rankId_) {
            // === 本端处理 ===
            if (missionId_ == 0) {
                // Mi0 执行本地 GroupCopy（合并 A+B 大小）
                // 对应 hccl: ccu::LocalCopy(dst, src, localCopySize, event, bit)
                // asc-devkit: GroupCopy(dst, src, goSize) — goSize 在 LoadArgs 中已加载
                CcuRep::LocalAddr localDst;
                localDst.addr = cclBufferAddr_;
                localDst.addr += recvOffset_;
                localDst.token = myToken_;
                // GroupCopy 内部会处理分块搬运逻辑
                GroupCopy(localDst, src_, groupOpSize_);
            }
            // Mi0 和 Mi1 都记录本端完成事件
            // 对应 hccl: ccu::EventRecord(writeDoneEvents[signalId], peerBit)
            // asc-devkit: RecordEvent(writeDoneEvents_[peerId]) — 无 bit 参数
            RecordEvent(writeDoneEvents_[peerId]);
            continue;
        }

        // === 远端写 ===
        // 构建远端目标地址
        dst_[peerId].addr = remoteCclBuffer_[peerId];
        dst_[peerId].addr += recvOffset_;
        if (missionId_ == 1) {
            // Mi1 的目标地址需额外加 sendSizeA[peerId]（B 紧接 A 之后）
            dst_[peerId].addr += sendSizeA_[peerId];
        }
        dst_[peerId].token = remoteToken_[peerId];

        // 条件传输：非空数据用 WriteNb，空数据直接 RecordEvent
        // CCU_IF 宏与 hccl 完全相同
        CCU_IF(sendSize != 0) {
            // 对应 hccl: ccu::Write(ch, dst, src, sendSize, event, peerBit)
            // asc-devkit: WriteNb(ch, dst, src, len, event) — 无 bit 参数
            WriteNb(channels_[channelIdx], dst_[peerId], src_, sendSize, writeDoneEvents_[peerId]);
        }
        CCU_IF(sendSize == 0) {
            // 空数据直接置位完成事件
            RecordEvent(writeDoneEvents_[peerId]);
        }
        channelIdx++;
    }

    // === 等待所有写操作完成 ===
    // 对应 hccl: for (signalId) { ccu::EventWait(writeDoneEvents[signalId], waitBits); }
    // asc-devkit: 逐个等待（因为 CompletedEvent 无 bit mask）
    for (uint64_t peerId = 0; peerId < rankSize_; peerId++) {
        WaitEvent(writeDoneEvents_[peerId]);
    }

    HCCL_DEBUG("[CcuKernelHalfAllToAllVMesh1D::DoHalfAllToAll] success, missionId[%u]", missionId_);
}

/*
 * ==================== MissionSync ====================
 * 跨 Mission 同步：Mi0 和 Mi1 互相等待对方搬运完成
 *
 * 对应 hccl 仓的 MissionSync() 函数
 * 关键差异：
 *   - hccl: ccu::EventRecord("half_alltoallv_mission0_sync", mask) — 命名标签事件
 *   - asc-devkit: 无命名标签机制，改用 channel 的 NotifyRecord/NotifyWait
 *   两个 Mission 共享同一组 channel，用不同 mask bit 区分
 */
void CcuKernelHalfAllToAllVMesh1D::MissionSync()
{
    // Mi0 置位 bit0 并等待 bit1，Mi1 置位 bit1 并等待 bit0
    uint16_t recordMask = static_cast<uint16_t>(1U << missionId_);
    uint16_t waitMask = static_cast<uint16_t>(1U << (1U - missionId_));

    // 通知对方本 Mission 完成
    for (ChannelHandle channel : channels_) {
        NotifyRecord(channel, CKE_IDX_1, recordMask);
    }
    // 等待对方 Mission 完成
    for (ChannelHandle channel : channels_) {
        NotifyWait(channel, CKE_IDX_1, waitMask);
    }

    HCCL_DEBUG("[CcuKernelHalfAllToAllVMesh1D::MissionSync] success, missionId[%u]", missionId_);
}

/*
 * ==================== PostSync ====================
 * 跨 rank 后同步：仅 Mi0 执行
 *
 * 对应 hccl 仓的 PostSync() 函数
 * 采用源端保序方式：Mi0 向所有对端发送完成信号并等待确认
 */
void CcuKernelHalfAllToAllVMesh1D::PostSync()
{
    // 仅 Mission 0 执行后同步
    if (missionId_ != 0) {
        return;
    }

    // 向所有对端发送完成信号
    for (ChannelHandle channel : channels_) {
        NotifyRecord(channel, CKE_IDX_1, 1);
    }
    // 等待所有对端的完成信号
    for (ChannelHandle channel : channels_) {
        NotifyWait(channel, CKE_IDX_1, 1);
    }

    HCCL_DEBUG("[CcuKernelHalfAllToAllVMesh1D::PostSync] success, missionId[%u]", missionId_);
}

/*
 * ==================== Algorithm ====================
 * 核心算法入口，由 CCU 框架调度执行
 *
 * 对应 hccl 仓的 CcuHalfAllToAllVMesh1DKernel() 入口函数
 * 流程：InitResource → LoadArgs → PreSync → DoHalfAllToAll → MissionSync → PostSync
 */
HcclResult CcuKernelHalfAllToAllVMesh1D::Algorithm()
{
    HCCL_INFO("[CcuKernelHalfAllToAllVMesh1D] Algorithm start, missionId[%u].", missionId_);

    CHK_RET(InitResource());
    LoadArgs();
    PreSync();
    DoHalfAllToAll();
    MissionSync();
    PostSync();

    HCCL_INFO("[CcuKernelHalfAllToAllVMesh1D] Algorithm end, missionId[%u].", missionId_);
    return HcclResult::HCCL_SUCCESS;
}

/*
 * ==================== GeneArgs ====================
 * 将 TaskArg 转换为 SQE 参数数组
 *
 * 对应 hccl 仓的 BuildTaskArgs()（在 Template 层构建 vector<uint64_t>）
 * asc-devkit 在 Kernel 层用 GeneArgs 转换
 *
 * ★ 关键约束：返回值顺序必须与 LoadArgs() 中的 Load() 顺序严格一致
 */
std::vector<uint64_t> CcuKernelHalfAllToAllVMesh1D::GeneArgs(const CcuTaskArg& arg)
{
    const CcuTaskArgHalfAllToAllVMesh1D* taskArg =
        dynamic_cast<const CcuTaskArgHalfAllToAllVMesh1D*>(&arg);

    // 基础参数（顺序对应 LoadArgs 中的 Load 调用）
    std::vector<uint64_t> args = {
        taskArg->inputAddr_,       // → Load(userInAddr_)
        taskArg->cclBufferAddr_,   // → Load(cclBufferAddr_)
        taskArg->token_,           // → Load(myToken_)
        taskArg->sendSizesAddr_,   // → Load(sendSizesAddr_)
        taskArg->sendOffsetsAddr_, // → Load(sendOffsetsAddr_)
        taskArg->recvOffset_       // → Load(recvOffset_)
    };

    // GroupOpSize（4 个值，对应 Load(groupOpSize_)）
    // CalGoSize 将数据量切分为 loop 分块参数
    auto goSize = CalGoSize(taskArg->localDataSize_);
    args.insert(args.end(), goSize.begin(), goSize.end());

    HCCL_INFO("[CcuKernelHalfAllToAllVMesh1D::GeneArgs] args size[%zu], missionId[%u]",
              args.size(), missionId_);
    return args;
}

} // namespace mc2_ops_hccl
