/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * 重构 PoC · 动词发射框架（方案 §6.4.1 七动词汇表 / §6.5）
 *
 * 归属说明：词汇表与发射接口是 L4/L5 跨层契约（EmitCtx 需要 sink 类型），故落 inc/
 *（引擎无关值对象居所）；两引擎映射表虽含引擎知识但仅是注记字符串（真引擎头零
 * include）。真原语接线（P2）时映射表落点随 PrimitiveSink 一并评估是否回迁 l5/。
 *
 * 本文件是"引擎知识唯一居所"的落点：L4 后端 Emit 只产出 VerbCall（引擎无关语义动词），
 * 动词 → 物理接口的映射由本层提供（每引擎一张表）。真原语接线在 P2 真机联调时完成，
 * 本轮先落三件事：
 *   ① VerbCall 值对象（结构化记录，§6.13.4 TraceRecord 的前身——seq/segIdx 字段留 P2）
 *   ② IVerbSink 抽象（可注入发射目标：默认 TraceSink 拼 trace 字符串，真机时
 *      PrimitiveSink 直驱 hcomm 原语；ST 语义 checker 也挂此口）
 *   ③ 两引擎动词映射表（kAicpuVerbMap / kCcuVerbMap，每行注明真实目标与现状出处）——
 *      host 侧可查可 dump（§6.13.3 表 1 注册表 dump 的雏形），P2 接线时表行即工单
 */
#ifndef REFACTOR_POC_VERB_SINK_H
#define REFACTOR_POC_VERB_SINK_H

#include "alg_param.h"     // u32/u64（verb_sink 只用标量别名，不依赖 ActionSpec 值
                           // 对象——原 include action_spec.h 会经其尾部的
                           // plan_serializer.h → backend_registry.h 回指本文件，
                           // 在"本头先被 include"的次序下 backend_registry.h 内
                           // EmitCtx::IVerbSink 前向不见（guard 已置位空过）。
                           // 改为最小依赖 alg_param.h，include 环从根上消除）

#include <string>
#include <vector>

namespace mc2_ops_hccl::refactor {

// ---------------------------------------------------------------------------
// VerbCall：一次动词发射的值对象（§6.13.4：结构化记录，不是字符串拼接；
// 真实实现的 seq/segIdx 字段随编排器落地，PoC 阶段由 sink 消费方自行编号）
// ---------------------------------------------------------------------------
struct VerbCall {
    enum Verb {
        COPY,           // 本地拷贝（input→output 归位 / input→scratch 上料）
        POST_COPY,      // 段后回拷（scratch→output）。★现状勘误（§10.2 B4）：ins_temp_all_gather_
                        // mesh_1D.cc 的 PostLocalCopy（:278 定义）无调用点（死代码），真实语义
                        // read 直达 output（:170）；动词保留，P2 需中转形态再启用（且 read 目的
                        // 地须改 scratch）。本后端不发（AICPU AG Emit 无此步）
        READ,           // 从对端读（read 语义：src 在对端、dst 在本地）
        WRITE,          // 写到对端（write 语义：src 在本地、dst 在对端）
        REDUCE,         // 远端规约写（write + reduce，RS 类动作用，P2 落地）
        BROADCAST,      // 一条声明式原语全域广播（CCU GroupBroadcast）
        SYNC_PEER_PRE,  // 段前屏障（rank 间握手，数据动词前）
        SYNC_PEER_POST, // 段后屏障（rank 间握手，数据动词后）
    };
    Verb verb{COPY};
    u64 src{0};
    u64 dst{0};
    u64 size{0};
    u32 peer{0};        // READ/WRITE 的对端 rank；其余为 0
};

// 动词名（dump/trace 渲染用；与第一轮 EmitCtx.trace 的字符串前缀保持一致，
// ST 语义 checker 的 ParseTrace 按这些名字解析——改名须同步 checker）
inline const char* VerbName(VerbCall::Verb v)
{
    switch (v) {
        case VerbCall::COPY:          return "copy";
        case VerbCall::POST_COPY:     return "postCopy";
        case VerbCall::READ:          return "read";
        case VerbCall::WRITE:         return "write";
        case VerbCall::REDUCE:        return "reduce";
        case VerbCall::BROADCAST:     return "broadcast";
        case VerbCall::SYNC_PEER_PRE: return "syncPeer:pre";
        case VerbCall::SYNC_PEER_POST:return "syncPeer:post";
    }
    return "?";
}

// ---------------------------------------------------------------------------
// IVerbSink：动词发射目标（可注入）。L4 后端只管调 sink->Emit(call)，
// 不关心动词最终去了哪——trace / ST checker / 真原语，由注入方决定。
// ---------------------------------------------------------------------------
struct IVerbSink {
    virtual ~IVerbSink() = default;
    virtual void Emit(const VerbCall& call) = 0;
};

// 默认 sink：拼 EmitCtx.trace 字符串（第一轮行为原样保留，60 项断言的口径）。
// 格式：verb(src->dst,size[,peerN]) —— 与第一轮逐字兼容（postCopy 是新增动词，无旧断言依赖）
struct TraceSink : public IVerbSink {
    std::string* trace{nullptr};
    explicit TraceSink(std::string* t) : trace(t) {}
    void Emit(const VerbCall& call) override
    {
        if (trace == nullptr) return;
        *trace += std::string(VerbName(call.verb)) + "(" + std::to_string(call.src) + "->" +
                  std::to_string(call.dst) + "," + std::to_string(call.size) +
                  (call.peer ? ",peer" + std::to_string(call.peer) : "") + ") ";
    }
};

// ---------------------------------------------------------------------------
// 动词映射表（§6.5"动词映射表是 L4 唯一依赖的引擎知识"；本轮注记真实目标，P2 接线）
// ---------------------------------------------------------------------------
struct VerbMapEntry {
    VerbCall::Verb verb;
    const char* target;   // 真实物理接口（P2 接线工单）
    const char* note;     // 现状出处 / 附加语义
};

// AICPU 表（命令式：逐动词下发 stream 任务，KFC server 串行驱动）
// 现状原语声明：include/adv_api/hccl/internal/hcomm/hcomm_primitives.h
static const VerbMapEntry kAicpuVerbMap[] = {
    {VerbCall::COPY, "HcommLocalCopyOnThread(thread, dst, src, len)",
     "hcomm_primitives.h:87；现状包装 LocalCopy（alg_data_trans_wrapper.cc:719）"},
    {VerbCall::POST_COPY, "HcommLocalCopyOnThread(thread, dst, src, len)",
     "★现状勘误（§10.2 B4）：PostLocalCopy（ins_temp_all_gather_mesh_1D.cc:278）无调用点，"
     "read 直达 output（:170 rxDstPtr=outputPtr）。表行保留——P2 中转形态的接线工单，"
     "启用时 read 目的地须改 scratch"},
    {VerbCall::READ, "SendRecvRead(info, thread)",
     "hcomm_primitives.h:226 HcommReadOnThread 的包装（alg_data_trans_wrapper.cc:521-547），"
     "内含通道级握手三步：NOTIFY_IDX_ACK wait → read 循环 → DATA_SIGNAL record/wait（§10.2 A1）"},
    {VerbCall::WRITE, "SendRecvBatchWrite(info, thread)",
     "HcommWriteOnThread 批量包装（alg_data_trans_wrapper.cc；RS/AR 写 scratch 语义）"},
    {VerbCall::REDUCE, "SendRecvBatchWrite + LocalReduce 组合",
     "P2 随 ReduceSlice 落地（现状 RecvWrite + LocalReduce 两步，ins_temp_reduce_scatter_mesh_1D.cc）"},
    {VerbCall::SYNC_PEER_PRE, "HcommThreadNotifyRecordOnThread + WaitOnThread",
     "hcomm_primitives.h:116/126；现状 PreSyncInterThreads（alg_data_trans_wrapper.cc:829）"},
    {VerbCall::SYNC_PEER_POST, "HcommThreadNotifyRecordOnThread + WaitOnThread",
     "现状 PostSyncInterThreads（alg_data_trans_wrapper.cc:886）"},
    {VerbCall::BROADCAST, "(N/A) AICPU 命令式不使用 broadcast",
     "语义等价展开 = copy + read×(N-1)（§6.13.2 引擎对照表）；表行占位保完整性——"
     "§6.13.3 表 1 要求'每动词每引擎有落点（含不适用声明）'，空洞即告警"},
};

// CCU 表（声明式：动词进微码 Program，CCU 硬件自主执行；同步走 CKE 事件槽 + mask）
static const VerbMapEntry kCcuVerbMap[] = {
    {VerbCall::COPY, "ccu::LocalCopy / ccu::Store",
     "微码本地搬运；现状 mem2mem 系 kernel 的 GroupCopy（ccu_kernel_alg_base.cc）"},
    {VerbCall::POST_COPY, "ccu::LocalCopy",
     "§10.2 B4；CCU AG 走 broadcast 一步到位（dst 含本 rank 落位），本动词 AICPU 专用，表项占位"},
    {VerbCall::READ, "ccu::Load(channel, var)",
     "微码远端读（token 限定合法内存，ccu_kernel.h:194 CreateLocalAddr）"},
    {VerbCall::WRITE, "ccu::WriteVariableWithNotify",
     "写远端变量 + 通知（ccu_primitives_impl.h；PreSync 前同步广播地址即此式）"},
    {VerbCall::REDUCE, "CcuWriteMemToMemReduce",
     "mem2mem 规约写（现状 CcuKfcAllGatherMesh1DMem2Mem 的 writeReduce 路径），P2 落地"},
    {VerbCall::BROADCAST, "GroupBroadcast(ctx, channels, localDst, remoteDst[], src, goSize)",
     "ccu_kernel_alg_base.cc:301；现状 DoAllGather 一行（ccu_kernel_all_gather_mesh1d.cc:103-121）"},
    {VerbCall::SYNC_PEER_PRE, "CcuNotifyRecord / CcuNotifyWait(channel, CKE_IDX, mask)",
     "ccu_kernel.h:131-136；PreSync 互发地址+token 等齐（ccu_kernel_all_gather_mesh1d.cc:78-90）"},
    {VerbCall::SYNC_PEER_POST, "CcuNotifyRecord / CcuNotifyWait(channel, CKE_IDX, mask)",
     "PostSync 完工握手（ccu_kernel_all_gather_mesh1d.cc:92-101）"},
};

// 表查询/dump（§6.13.3 表 1 雏形：启动期一行可核对"该引擎每动词的落点"）
inline const VerbMapEntry* LookupVerbMap(VerbCall::Verb v, bool isCcu)
{
    const VerbMapEntry* table = isCcu ? kCcuVerbMap : kAicpuVerbMap;
    const size_t n = isCcu ? (sizeof(kCcuVerbMap) / sizeof(kCcuVerbMap[0]))
                           : (sizeof(kAicpuVerbMap) / sizeof(kAicpuVerbMap[0]));
    for (size_t i = 0; i < n; ++i) {
        if (table[i].verb == v) return &table[i];
    }
    return nullptr;
}

// 两引擎共用的发射便捷函数（后端 Emit 内一行一动词；sink 为空时走 TraceSink 兜底）
inline void FireVerb(IVerbSink* sink, std::string* trace, const VerbCall& call)
{
    if (sink != nullptr) {
        sink->Emit(call);
        return;
    }
    TraceSink fallback(trace);
    fallback.Emit(call);
}

} // namespace mc2_ops_hccl::refactor

#endif
