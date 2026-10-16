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
        // R13（2026-10-09，M0_R13_NHR_V5.md §八）：NHR 段数据动词——ReduceScatter
        // NHR 的 WriteReduce 步进流整段一帧承载（step/slice/jetty/repeat 循环在
        // server 帧内展开，M0_R13 §一投影）。值 8 尾追加（枚举序即动词 ID，devkit
        // 侧 KFC_UNIFIED_VERB_NHR_RUN=8 镜像互锁）。★不走隐式通道游标——通道由
        // chIdx(toRank) 显式推导（M0_R13 §二：NHR 数据帧序 ≠ 通道序）。
        NHR_RUN,
        // R18 · #11（M0_R18 §四）：NHR 步进流展开动词——与 mesh 的 READ/WRITE
        // 逐动词地位对等（Emit 全展开语义面，步/片循环外置到动词序）。
        // SLICE_REDUCE = 方向无关的"该 slot 与对端规约合一"（WR/RD 由执行端查
        // 通道协议自判）；载荷复用 VerbCall.nhr 种子 + step/sliceIdx 索引（几何
        // 由 NhrSliceOf 公共件推导——帧槽不花在偏移上）。devkit 镜像
        // KFC_UNIFIED_VERB_SLICE_REDUCE=9（kfc_server_message.h 纯追加段互锁）。
        SLICE_REDUCE,
        // 通道级握手半边（NHR 步间 ACK/DATA × WR/RD 侧四态——DoNhrRun ①③段
        // 的逐条动词化）。devkit 镜像 KFC_UNIFIED_VERB_CH_SYNC=10。
        CH_SYNC,
    };
    Verb verb{COPY};
    u64 src{0};
    u64 dst{0};
    u64 size{0};
    u32 peer{0};        // READ/WRITE 的对端 rank；其余为 0

    // R15 · ②（M0_R15 §四定稿）：NHR_RUN（AICPU 半边）的标量载荷——批量 slice
    // 列表不打包（步几何是纯函数，sink 按公式重算 CalcNhrSteps——M0_R13 §三
    // "容器是纯函数投影，序列化即冗余"同裁决），11 标量随动词走（值内嵌非指针
    // ——Emit 栈帧即亡，指针悬垂；VerbCall 是 Emit→sink 唯一桥梁，载荷可见
    // trace/calls_/编排器/等价断言全链）。valid=false = 非 NHR_RUN 动词（缺省
    // 零语义变化）；CCU v5 帧不含此载荷（AICPU 不进 v5 序列化，两引擎载荷互不
    // 相见——verbPath 恒 CCU，M0_R15 §四动词选型）。
    struct NhrRunScalars {
        bool valid{false};
        u32 myAlgRank{0};          // 域内序（步公式的 me）
        u32 rankNum{0};            // 域规模 n
        u32 channelsPerRank{0};    // 通道数（sink 内 channelIdx 循环上界；devkit 恒 1）
        u64 sliceSize{0};          // 本 rank 片字节（SliceSizeOf(me,n)）
        u64 inputBase{0};          // input 绝对基址（InstantiateRsSegment 合成式产物）
        u64 inputSliceStride{0};   // input 片间步长（三分支后真值）
        u64 scratchBase{0};        // scratch（cclBuff）绝对基址
        u64 repeatNum{1};          // repeat 轮数
        u64 inputRepeatStride{0};  // repeat 轮 input 步进
        u64 outputRepeatStride{0}; // repeat 轮 output 步进
        std::vector<u32> domainRanks;   // 域成员（algRank→userRank 映射表——sink
                                        //   通道查表键是 userRank，步表值是 algRank）
        // R18 · #11：展开动词索引（SLICE_REDUCE/CH_SYNC 专用——非 NHR_RUN 宏动词
        // 载荷；几何由执行端 NhrSliceOf(种子, step, sliceIdx) 推导，帧槽只花两索引）
        u32 stepIdx{0};
        u32 sliceIdx{0};
    };
    NhrRunScalars nhr;
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
        case VerbCall::NHR_RUN:       return "nhrRun";
        case VerbCall::SLICE_REDUCE:  return "sliceReduce";
        case VerbCall::CH_SYNC:       return "chSync";
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
// R16 · #7：加"线程策略"列——动词→线程的分配策略（哪个动词跑哪条线程、串/并、
// 计数器偏移）原散落在 hcomm_verb_sink.h 各 Do* 首行（Emit 调用点不可见，读者要
// 对比多个函数实现才能拼出全表——用户 2026-10-13 点名的设计缺陷）。现收敛为
// 表驱动单点声明：sink 侧 ThreadFor(verb) 查表取策略，改策略改一处。
//
// 边界（表管什么/不管什么）：
//   · 管段内"动词→线程池内哪条"（MAIN/计数器偏移/按通道）；
//   · 不管池怎么切（编排器域切池 segment_orchestrator.h 是池供给方）；
//   · 不管 notify 槽位策略（DoSyncPre idx 全 0 / DoSyncPost idx=i-1 / 编排器
//     跨域合取槽 0——另一族隐式策略，登记 R17 复核，本轮 note 注记归属）；
//   · kCcuVerbMap 填 NA——CCU 微码自驱动无线程概念（策略真身在设备侧 server
//     的 dataCursor 通道游标，unified 树外不可收编）。
// ---------------------------------------------------------------------------
// AICPU sink 的线程分配策略（枚举序即语义粒度，注释里是消费它的 sink 实现）：
enum class ThreadPolicy {
    MAIN,              // 恒 threads[0] 主线程（COPY/POST_COPY/REDUCE——DoCopy/DoReduce）
    COUNTER_OFF_0,     // 独立计数器：第 i 条 → threads[i+0]（READ——主线程承载首条，
                       //   AG mesh threadNum=N-1、read 数恰等于池大小）
    COUNTER_OFF_1,     // 独立计数器：第 i 条 → threads[i+1]（WRITE——主线程不下发写，
                       //   RS threadNum=N、WRITE 用 threads[1..N-1]，偏移 1）
    BY_CHANNEL,        // threads[channelIdx]（NHR_RUN——载荷 channelsPerRank 循环，
                       //   不进 READ/WRITE 计数器，自持通道序）
    MAIN_PLUS_SLAVES,  // main 下发 record + 全 sub 参与 wait/record 两段循环
                       //   （SYNC_PEER_PRE/POST——DoSyncPre/DoSyncPost）
    NOT_WIRED,         // 本引擎不接线（BROADCAST——AICPU 命令式显式拒绝）
    NA,                // 引擎无线程概念（kCcuVerbMap 全表——CCU 微码自驱动）
};

struct VerbMapEntry {
    VerbCall::Verb verb;
    const char* target;   // 真实物理接口（P2 接线工单）
    const char* note;     // 现状出处 / 附加语义
    ThreadPolicy thread;  // R16 · #7：线程策略（AICPU 侧；kCcuVerbMap 恒 NA）
};

// AICPU 表（命令式：逐动词下发 stream 任务，KFC server 串行驱动）
// 现状原语声明：include/adv_api/hccl/internal/hcomm/hcomm_primitives.h
static const VerbMapEntry kAicpuVerbMap[] = {
    {VerbCall::COPY, "HcommLocalCopyOnThread(thread, dst, src, len)",
     "hcomm_primitives.h:87；现状包装 LocalCopy（alg_data_trans_wrapper.cc:719）",
     ThreadPolicy::MAIN},
    {VerbCall::POST_COPY, "HcommLocalCopyOnThread(thread, dst, src, len)",
     "★现状勘误（§10.2 B4）：PostLocalCopy（ins_temp_all_gather_mesh_1D.cc:278）无调用点，"
     "read 直达 output（:170 rxDstPtr=outputPtr）。表行保留——P2 中转形态的接线工单，"
     "启用时 read 目的地须改 scratch",
     ThreadPolicy::MAIN},
    {VerbCall::READ, "SendRecvRead(info, thread)",
     "hcomm_primitives.h:226 HcommReadOnThread 的包装（alg_data_trans_wrapper.cc:521-547），"
     "内含通道级握手三步：NOTIFY_IDX_ACK wait → read 循环 → DATA_SIGNAL record/wait（§10.2 A1）",
     ThreadPolicy::COUNTER_OFF_0},
    {VerbCall::WRITE, "SendRecvBatchWrite(info, thread)",
     "HcommWriteOnThread 批量包装（alg_data_trans_wrapper.cc；RS/AR 写 scratch 语义）",
     ThreadPolicy::COUNTER_OFF_1},
    {VerbCall::REDUCE, "SendRecvBatchWrite + LocalReduce 组合",
     "P2 随 ReduceSlice 落地（现状 RecvWrite + LocalReduce 两步，ins_temp_reduce_scatter_mesh_1D.cc）",
     ThreadPolicy::MAIN},
    {VerbCall::SYNC_PEER_PRE, "HcommThreadNotifyRecordOnThread + WaitOnThread",
     "hcomm_primitives.h:116/126；现状 PreSyncInterThreads（alg_data_trans_wrapper.cc:829）；"
     "notify 槽位策略（idx 全 0）不归本表管——sink DoSyncPre 实现层知识",
     ThreadPolicy::MAIN_PLUS_SLAVES},
    {VerbCall::SYNC_PEER_POST, "HcommThreadNotifyRecordOnThread + WaitOnThread",
     "现状 PostSyncInterThreads（alg_data_trans_wrapper.cc:886）；notify 槽位策略（idx=i-1）"
     "不归本表管——sink DoSyncPost 实现层知识",
     ThreadPolicy::MAIN_PLUS_SLAVES},
    {VerbCall::BROADCAST, "(N/A) AICPU 命令式不使用 broadcast",
     "语义等价展开 = copy + read×(N-1)（§6.13.2 引擎对照表）；表行占位保完整性——"
     "§6.13.3 表 1 要求'每动词每引擎有落点（含不适用声明）'，空洞即告警",
     ThreadPolicy::NOT_WIRED},
    // R15 · ②（M0_R15 §四）：NHR_RUN 的 AICPU 半边接线（DoNhrRun——gated
    // UseUnifiedNhrAicpuPath，ungated 照旧 default 拒绝）。步循环 = 每步五段
    // 通道级握手（ACK/DATA）+ 逐 slice WriteReduce/ReadReduce（wrapper
    // SendRecvWriteReduce :406-435 / SendRecvReadReduce :642-687 的动词化平移）。
    // R18 全删轮（2026-10-14）：DoNhrRun 步循环执行面已删——本行保留为历史锚
    //（枚举值 8 ABI 空洞保留，kAicpuVerbMap/kCcuVerbMap 值序互锁消费）；NHR 步进流
    // 一律走下方 SLICE_REDUCE/CH_SYNC 两行动词（BY_CHANNEL 同策略）
    {VerbCall::NHR_RUN, "【已删形态·历史锚】DoNhrRun 步循环（R15 落地、R18 全删）——"
     "无生产 Emit/执行路径；词汇表行保留撑枚举值序（VerbCall::NHR_RUN == 8 互锁）",
     "载荷 = VerbCall.nhr 11 标量（值内嵌）——载荷结构被 SLICE_REDUCE 复用（种子+两索引）",
     ThreadPolicy::BY_CHANNEL},
    // R18 · #11：NHR 展开动词（与 mesh 逐动词对等——宏动词 NHR_RUN 的对称化）。
    // 执行端自判 WR/RD（isDmaRead 通道协议知识下沉执行端，Emit 产物两侧同构）
    {VerbCall::SLICE_REDUCE, "HcommWriteReduceOnThread / HcommReadReduceOnThread（执行端自判方向）",
     "NHR 步进流逐 slice 动词化（M0_R18 §一）：载荷 = nhr 种子 + step/sliceIdx，"
     "几何 NhrSliceOf 公共件推导（与 DoNhrRun 同源）；peer = WR:sendTo / RD:recvFrom",
     ThreadPolicy::BY_CHANNEL},
    {VerbCall::CH_SYNC, "HcommChannelNotifyRecordOnThread / HcommChannelNotifyWaitOnThread",
     "NHR 步间通道级握手半边（dir 四态：WR/RD × ACK/DATA——DoNhrRun ①③段的动词化）",
     ThreadPolicy::BY_CHANNEL},
};

// CCU 表（声明式：动词进微码 Program，CCU 硬件自主执行；同步走 CKE 事件槽 + mask）
// ★R12-1 修正令（2026-10-08，kCcuVerbMap 逐行重订——原表系早期望文生义注记，本轮按
// 老 kernel 消费式逐行考证修正，M0 考古 §八 留痕，验证见 poc_unified_verb_exec_test.cc）：
static const VerbMapEntry kCcuVerbMap[] = {
    {VerbCall::COPY, "ccu::LocalCopy(dst, src, len, event, mask)",
     "微码本地搬运（ccu_primitives_dl.hpp:105）。R12-1 修正：剔 Store（Store 是 HBM 变量写，"
     "非内存搬运）；GroupCopy 是 goSize 分块变体（alg_base.cc:716）——>8P RS 本 rank 上料"
     "（rs_m2m.cc:176-179 LocalCopy(scratchMem[me], myInput)）与 AG 本地归位（ag_m2m.cc:131）"
     "均直用 LocalCopy/GroupCopy，见 rs_m2m.cc:176 / ag_m2m.cc:131",
     ThreadPolicy::NA},
    {VerbCall::POST_COPY, "ccu::LocalCopy",
     "§10.2 B4；CCU AG 走 broadcast 一步到位（dst 含本 rank 落位），本动词 AICPU 专用，表项占位",
     ThreadPolicy::NA},
    {VerbCall::READ, "ccu::Read(channel, local, remote, len, event, mask)",
     "微码远端 DMA 读：src 在对端（RemoteAddr）/dst 在本地（LocalAddr），ccu_primitives_dl.hpp:142。"
     "R12-1 修正：原'ccu::Load(channel,var)'系误注——Load 是 HBM→XN 变量装载（server 队列循环"
     "LoadFuncParamFromMemory 即此式），非机间搬运；真实消费式 = 老 RS kernel Phase1 "
     "ccu::Read(ch, scratchMem[rankIdx], remoteInput[rankIdx], slice, events, rankMask)"
     "（ccu_kernel_kfc_reduce_scatter_mesh1d_mem2mem.cc:180-182）",
     ThreadPolicy::NA},
    {VerbCall::WRITE, "ccu::Write(channel, remote, local, len, event, mask)",
     "微码远端 DMA 写：src 本地/dst 对端，ccu_primitives_dl.hpp:164。R12-1 修正：原"
     "WriteVariableWithNotify 挪至 SYNC_PEER_PRE 行（那是地址交换本体非数据写）；真实消费式 = "
     "老 AG kernel Phase1 ccu::Write(ch, dst[rankIdx], src, sliceSize, events, rankMask)"
     "（ccu_kernel_kfc_all_gather_mesh1d_mem2mem.cc:176-177）",
     ThreadPolicy::NA},
    {VerbCall::REDUCE, "GroupLocalReduce(ctx, outDst, scratch[], goSize, dt, odt, op)",
     "本地折叠原语（ccu_kernel_alg_base.cc:823）——R12-1 修正：原'CcuWriteMemToMemReduce'是"
     "远端规约写（WriteReduce 通道原语），RS 的 REDUCE 动词是本地折叠：≤8P 走 GroupLocalReduce"
     "（rs_m2m.cc:249-250，scratch[me] 槽替换为 input 直读），>8P 走 PairwiseLocalReduce"
     "（rs_m2m.cc:204-232，log2 两两折叠链）——server verb 分发按 rankSize 分叉",
     ThreadPolicy::NA},
    {VerbCall::BROADCAST, "GroupBroadcast(ctx, channels, localDst, remoteDst[], src, goSize)",
     "ccu_kernel_alg_base.cc:301；现状 DoAllGather 一行（ccu_kernel_kfc_all_gather_mesh1d.cc:103-121）。"
     "扇出语义：本地落位（LocalCopy）+ 逐通道机间写（CreateMultiOpBroadcast 的 loop 体 "
     "ccu::Write，alg_base.cc:283-287）",
     ThreadPolicy::NA},
    {VerbCall::SYNC_PEER_PRE, "ccu::WriteVariableWithNotify + ccu::NotifyWait(channel, CKE_IDX, mask)",
     "R12-1 修正（补全）：地址交换本体 = WriteVariableWithNotify 逐通道广播本端基址/Token"
     "（ccu_primitives_dl.hpp:73；老 AG PreSync :115-120、老 RS PreSync :136-143 逐 XN 一条）→ "
     "NotifyWait 等齐（ag_m2m.cc:121-123 / rs_m2m.cc:144-147）。原注只写了 Notify 半边。"
     "v5：掩码逐段不同（AG=OUT|TOK / RS=IN|SCR|TOK，M0 §二 XN 统一表）",
     ThreadPolicy::NA},
    {VerbCall::SYNC_PEER_POST, "ccu::NotifyRecord / ccu::NotifyWait(channel, CKE_IDX, mask)",
     "PostSync 完工握手（ccu_kernel_all_gather_mesh1d.cc:92-101；ag_m2m.cc:127-137）——"
     "v5 XN 统一表 POST=4（迁位避让 OUT=3，R12-2 尾款：AG 老表 POST=3 与新 OUT=3 撞位互踩）",
     ThreadPolicy::NA},
    // R18 全删轮（2026-10-14）：server VerbNhrRun 帧内展开分支已删（CCU NHR 段
    // 改 Emit 逐动词 SLICE_REDUCE/CH_SYNC 帧）——本行保留为历史锚（枚举值序互锁）
    {VerbCall::NHR_RUN, "【已删形态·历史锚】NhrRunStep 系帧内整段展开（R13 落地、R18 全删）——"
     "无生产帧路径；CCU 侧改两动词（VerbSliceReduce/VerbChSync server 分支承载）",
     "通道显式推导（chIdx(toRank)）语义被 SLICE_REDUCE peer 槽显式查表继承",
     ThreadPolicy::NA},
    // R18 · #11：NHR 展开动词的 CCU 侧（server VerbSliceReduce/VerbChSync 分支
    // ——kfc_server_message.h KFC_UNIFIED_VERB_SLICE_REDUCE/CH_SYNC=9/10 互锁）。
    // ccu::WriteReduce 微码现役（ccu_kernel_kfc_server.cc :714/:732——VerbNhrRun
    // 逐 slice 消费同一原语）；通道显式查表（帧 peer 槽——dataCursor 不动，M0_R18
    // §四：两动词不计 dataFrames，隐式映射只索引 READ/WRITE 不变）
    {VerbCall::SLICE_REDUCE, "ccu::WriteReduce（执行端自判方向；RD 侧 CCU v5 首版 WR 单侧——"
     "DMA 读侧形态登记 P2，M0_R18 §四）",
     "NHR 步进流逐 slice 动词化：帧槽 = step/sliceIdx/size/peerWR/peerRD + repeat 种子"
     "（槽 7-10 照 NHR_RUN 同型）；peer 用 algRank（执行端经域成员表换 userRank 查表）",
     ThreadPolicy::NA},
    {VerbCall::CH_SYNC, "ccu::NotifyRecord / ccu::NotifyWait（通道级，CKE_IDX_ACK/DATA）",
     "NHR 步间握手半边四态（dir 槽：WR/RD × ACK/DATA）——VerbNhrRun 步前/步末屏障的动词化",
     ThreadPolicy::NA},
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

// 线程策略查询便捷件（R16 · #7：sink 侧 ThreadFor 的单点输入——AICPU 表专用，
// CCU 表恒 NA；查无表行返回 NA 由调用方按不可达处理）
inline ThreadPolicy ThreadPolicyOf(VerbCall::Verb v, bool isCcu)
{
    const VerbMapEntry* e = LookupVerbMap(v, isCcu);
    return (e == nullptr) ? ThreadPolicy::NA : e->thread;
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
