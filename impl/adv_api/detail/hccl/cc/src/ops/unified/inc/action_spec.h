/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * 重构 PoC · L3 语义动作层的值对象定义（引擎无关，不含任何引擎头）
 *
 * 对应现状：TemplateDataParams（布局）+ subCommRanks/channels（域）+ 各 template
 * 私有成员（channelsPerRank_ / supportSymmetricMemory_ / dataType_ …）散落在
 * 65 份 AICPU template 与 46 份 CCU kernel 里，每份各自解释。
 */
#ifndef REFACTOR_POC_ACTION_SPEC_H
#define REFACTOR_POC_ACTION_SPEC_H

#include <algorithm>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "alg_param.h"   // devkit 真实类型 u32/u64/HcclDataType…

namespace mc2_ops_hccl::refactor {

// u32/u64 复用 devkit 定义（base.h：unsigned int / unsigned long long），
// 不自定义以免与真实接口签名不匹配

// ---------------------------------------------------------------- 公共组件 A
// DataLayout：全部 offset 公式的唯一实现点（方案 §6.7）
// 字段承接现有 TemplateDataParams，语义不变；差别是"公式只写一遍"。
struct DataLayout {
    u64 sliceSize{0};
    u64 tailSize{0};                 // count 不被 N 整除时末 rank 的短片
    u64 inputSliceStride{0};
    u64 outputSliceStride{0};        // strideCount 的落点（喂计算的布局敏感需求）
    u64 repeatNum{1};
    u64 inputRepeatStride{0};
    u64 outputRepeatStride{0};
    u64 inBaseOff{0};
    u64 outBaseOff{0};
    u64 scratchBaseOff{0};
    u32 dataTypeSize{2};

    // 现状：这三个式子在 ins_temp_all_gather_mesh_1D.cc 的 RunAllGatherMesh /
    // LocalDataCopy / PostLocalCopy 中各写一遍（第 163/164/172/174/240/241/306/307 行），
    // 同一套式子在 AG/RS/AR/A2A 四族 template 里再各抄一遍。
    u64 InOffset(u32 algRank, u32 rpt = 0) const
    {
        return inBaseOff + rpt * inputRepeatStride + inputSliceStride * algRank;
    }
    u64 OutOffset(u32 algRank, u32 rpt = 0) const
    {
        return outBaseOff + rpt * outputRepeatStride + outputSliceStride * algRank;
    }
    u64 ScratchOffset(u32 algRank, u32 rankNum, u32 rpt = 0) const
    {
        return scratchBaseOff + rpt * (sliceSize * rankNum) + sliceSize * algRank;
    }
    // tail 判定的唯一入口——现状是每处 `if (tailSize != 0 && algRank == N-1)` 手写
    u64 SliceSizeOf(u32 algRank, u32 rankNum) const
    {
        return (tailSize != 0 && algRank == rankNum - 1) ? tailSize : sliceSize;
    }
    u64 CountOf(u32 algRank, u32 rankNum) const { return SliceSizeOf(algRank, rankNum) / dataTypeSize; }

    // two_shot：RS 段的 output 即 AG 段的 input（配方声明数据流边时调用）
    DataLayout ForSecondStage() const
    {
        DataLayout next = *this;
        next.inputSliceStride = outputSliceStride;
        next.inBaseOff = outBaseOff;
        return next;
    }
    // two_shot 段间衔接（§12-9 B2 决策，2026-09-20）：对齐现状 hcclBuff 中转语义——
    // RS 段把各 rank 的片归并到 scratch[0]（reduceDstSlice offset = hcclBuffBaseOffset，
    // ins_temp_all_reduce_mesh_1D_two_shot.cc:297），AG 段对任意 peer 的取数源恒为
    // scratch[0]（sendSrcSlice/recvSrcSlice 均 hcclBuffBaseOffset，:363/:367）——即
    // 本函数产出的布局 InOffset(peer) 对所有 peer 等值（scratch 基址 + 0 号槽）。
    // ForSecondStage 保留给 sequence 配方的域间衔接（前域 output 真的就是后域 input）。
    DataLayout ForSecondStageViaScratch() const
    {
        DataLayout next = *this;
        // input 半边改指 scratch 0 号槽：基址 = scratch 基址，步长 = 0
        //（AG 段逐 peer 取的是同一份归并结果，非逐 peer 槽位——§12-9"不改流水实现"）
        next.inBaseOff = scratchBaseOff;
        next.inputSliceStride = 0;
        // output 半边不变：AG 段仍写真实 output（outBaseOff/outputSliceStride 语义不动）
        return next;
    }
    // ChunkPipeline：切出第 i 个子段（段形态限定词 .chunk 的落点）
    DataLayout ForChunk(u32 i, u32 nSeg) const
    {
        DataLayout seg = *this;
        seg.sliceSize = sliceSize / nSeg;
        seg.inBaseOff = inBaseOff + i * seg.sliceSize;
        seg.outBaseOff = outBaseOff + i * seg.sliceSize;
        seg.tailSize = (i == nSeg - 1) ? (sliceSize - seg.sliceSize * (nSeg - 1)) : 0;
        return seg;
    }
    // 切轮（§10.2 A2）：OrchestrateLoop 逐轮推进 baseOff 的唯一实现点。
    // 与 ForChunk 的差别：轮内 sliceSize = 本轮实际字节（末轮为余量），不带 tail 语义
    // （tail 是 rank 维的尾块，轮是数据维的切分，两者正交）；repeat 不变。
    // roundStride = 满轮步长（末轮 slice 是余量，但下一轮起点仍按满轮推进——
    // 现状 OrchestrateLoop 的 processedDataCount 语义）。
    // tailSize 分支注：tailSize ≥ roundSlice（尾 rank 块大于尾轮字节）时当前置 0 是
    // 保守简化，P2 落真后端前补专项断言或判非法。
    DataLayout ForRound(u32 round, u64 roundSlice, u64 roundStride) const
    {
        DataLayout r = *this;
        r.inBaseOff = inBaseOff + round * roundStride;
        r.outBaseOff = outBaseOff + round * roundStride;
        r.sliceSize = roundSlice;
        r.tailSize = (tailSize != 0 && tailSize < roundSlice) ? tailSize : 0; // 尾轮不足整轮时
        return r;
    }
};

// ---------------------------------------------------------------- 动作与域
enum class ActionKind { CollectAll, ReduceSlice, ExchangeAll, LocalCopy, LocalReduce };

// pattern：域内遍历方式（不是链路类型，方案 §6.8）
enum class CommPattern { AUTO, MESH, NHR, RING };
// 段形态限定词
enum class SegShape { NONE, ONE_SHOT, TWO_SHOT };
// 内存路径
enum class TransportPolicy { AUTO, MEM2MEM, VIA_CCUBUF, SYMMEM };

struct DomainSpec {
    std::vector<u32> ranks;          // 现状 subCommRanks_[0]
    u32 myRank{0};
    u32 MyAlgRank() const            // 现状 GetAlgRank()，每 template 调一次
    {
        for (u32 i = 0; i < ranks.size(); ++i) {
            if (ranks[i] == myRank) return i;
        }
        return 0;
    }
    u32 Size() const { return static_cast<u32>(ranks.size()); }
};

struct ReduceSpec {
    int reduceOp{0};
    int dataType{0};
    int outputType{0};
};

struct SyncSpec {
    bool preBarrier{true};
    bool postBarrier{true};
};

// ActionSpec：Plan.segments 的元素。引擎无关，可序列化。
struct ActionSpec {
    ActionKind kind{ActionKind::CollectAll};
    DataLayout layout;
    DomainSpec domain;
    ReduceSpec reduce;
    CommPattern pattern{CommPattern::AUTO};
    SegShape shape{SegShape::NONE};
    u32 channelsPerRank{1};          // single/multi-channel 限定词的落点
    u64 token{0};                    // 内存 token（devkit GetToken 产出，进消息槽）
    TransportPolicy transport{TransportPolicy::AUTO};
    SyncSpec sync;
    std::vector<u32> inputRanks;     // 数据所有权契约（段间数据流边）
    std::vector<u32> outputRanks;
};

// ---------------------------------------------------------------- 资源申报
struct ResCounters {                 // 七类 CCU 硬件资源
    u32 loop{0}, ccuBuf{0}, variable{0}, address{0}, event{0}, ccuThread{0}, instruction{0};
    ResCounters& operator+=(const ResCounters& o)
    {
        loop += o.loop; ccuBuf += o.ccuBuf; variable += o.variable; address += o.address;
        event += o.event; ccuThread += o.ccuThread; instruction += o.instruction;
        return *this;
    }
};

struct ChannelReq {                  // 通道申请（引擎无关，两引擎共用）
    u32 remoteRank{0};
    u32 count{1};                    // channelsPerRank
};

// kernel 注册计划（§10.2 C9）：真实链路每个 CCU kernel 一条 CcuKernelInfo
// {kernelFuncName, channels} 进 resourceRequest（现状用法见 ccu_temp_all_gather_2dies_mesh_1D.cc:88，
// 双 die 两 kernel 各带各的通道组）——"kernelFunc 跟动作走"的申报载体
struct CcuKernelPlan {
    std::string kernelFuncName;      // 动作 kernel 名（每动作一个，非算法名）
    std::vector<ChannelReq> channels;
};

struct ResourceClaim {
    std::map<u32, std::map<u32, ResCounters>> ccu;   // [dieId][resGroup]
    u32 slaveThreadNum{0};
    u32 notifyNumOnMainThread{0};
    std::vector<u32> notifyNumPerThread;
    std::vector<ChannelReq> channels;
    std::vector<CcuKernelPlan> kernels;              // kernel 注册条目（逐条追加，不合并）

    ResourceClaim& operator+=(const ResourceClaim& o)
    {
        // C++14：不用结构化绑定（devkit -std=c++14 -Werror 硬约束）
        for (auto itDie = o.ccu.begin(); itDie != o.ccu.end(); ++itDie) {
            const u32 die = itDie->first;
            for (auto itGrp = itDie->second.begin(); itGrp != itDie->second.end(); ++itGrp) {
                const u32 grp = itGrp->first;
                ccu[die][grp] += itGrp->second;
            }
        }
        slaveThreadNum = std::max(slaveThreadNum, o.slaveThreadNum);   // 线程可复用，取峰值
        notifyNumOnMainThread += o.notifyNumOnMainThread;
        notifyNumPerThread.insert(notifyNumPerThread.end(), o.notifyNumPerThread.begin(),
                                  o.notifyNumPerThread.end());
        channels.insert(channels.end(), o.channels.begin(), o.channels.end());
        kernels.insert(kernels.end(), o.kernels.begin(), o.kernels.end());
        return *this;
    }
};

// Plan：配方展开的产物，纯数据（可序列化 / 可缓存 / 可 dump）
struct Plan {
    // 序列化格式 ABI 版本（§6.3 version 字段升版守门）：v3 = KFC 消息槽定长格式
    // （真序列化，第四轮 A4 交付，实现见 plan_serializer.h 的 SerializePlanV3；
    // 本文件内 SerializePlanForCtx 是 v1 文本摘要的沿用，仅日志/维测用）
    u32 version{3};
    std::vector<ActionSpec> segments;
    ResourceClaim claim;
};

// ---------------------------------------------------------------------------
// Plan 序列化摘要（v1 文本格式，日志/维测专用）：
//   ★第四轮（A4，2026-09-21）已交付真序列化 v3（plan_serializer.h 的
//   SerializePlanV3，定长槽位二进制格式，可重建执行参数；三轮 MAJOR 降级声明
//   至此兑现，Plan.version 已升 3）。本摘要格式保留作日志 dump——ctx 旁路
//   （HcclSerializeUnifiedPlan）按 planBlob.size() 分配，真注册链接线（下一轮）
//   切换到 SerializePlanV3 的字节视图（vector<u64> → string 视图搬运），
//   此函数届时退役或降级为 HCCL_DEBUG 专用。
// ---------------------------------------------------------------------------
inline std::string SerializePlanForCtx(const Plan& plan)
{
    std::string s = "PLAN v" + std::to_string(plan.version) +
                    " segs:" + std::to_string(plan.segments.size()) + "\n";
    for (const ActionSpec& a : plan.segments) {
        s += "seg kind:" + std::to_string(static_cast<int>(a.kind)) +
             " pattern:" + std::to_string(static_cast<int>(a.pattern)) +
             " shape:" + std::to_string(static_cast<int>(a.shape)) +
             " transport:" + std::to_string(static_cast<int>(a.transport)) +
             " chan:" + std::to_string(a.channelsPerRank) +
             " slice:" + std::to_string(a.layout.sliceSize) + "\n";
    }
    s += "claim threads:" + std::to_string(plan.claim.slaveThreadNum) +
         " channels:" + std::to_string(plan.claim.channels.size()) +
         " kernels:" + std::to_string(plan.claim.kernels.size()) + "\n";
    return s;
}

// L1 产出的通信订单
struct PrimitiveRequest {
    int op{0};
    DataLayout layout;
    ReduceSpec reduce;
    DomainSpec domain;               // 由 DomainPlan 推导（用户不感知，方案 §6.8）
    bool allConnected{true};         // LinkProfile：域内是否全互联 → AUTO 选 pattern
    bool supportSymmetricMemory{false};
    u64 inputVa{0};                  // 用户 input 设备虚地址（§10.2 B8：token 计算源，0=未知）
    u64 outputVa{0};                 // 用户 output 设备虚地址（同上）
};

} // namespace mc2_ops_hccl::refactor

// v3 序列化实现在 plan_serializer.h（依赖 backend_registry.h 的 LoopSplitter 完整
// 类型；backend_registry.h 又 include 本文件——单独成头破循环，值对象文件保持零
// 引擎依赖的增长面）
#include "plan_serializer.h"

#endif
