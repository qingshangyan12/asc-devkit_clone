/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * R6 · WP-A2：AICPU 侧 Plan 消费方（v4 blob 解释器 + 逐 turn 实例化 + HcommVerbSink 驱动）
 *
 * 定位：替代存量 executor 的 Orchestrate 等价物（不注册进 CollAlgExecRegistryV2、
 * 不改任何存量 executor/template——exec 期分叉在 Orchestrate 调用点前，主会话预裁②）。
 *
 * 三段职责（对照存量链路）：
 *   ① 解释  blob（HcclEngineCtxGet 取 device ctx → D2H → v4 帧解读）——对照
 *          CCU 侧 CcuPrepareForUnifiedPlan 的 blob 消费，但 AICPU 侧在 host 进程
 *          （AICPU TS server 路径）内解释，无 device 写入方
 *   ② 实例化  结构底版（blob：段序/动作 ID/秩布局/符号布局）+ exec 期 msg 现值
 *          （count/inputPtr/outputPtr/dataType/strideCount/reduceType）——方向 B，
 *          对齐 FormatOpenOpParamDataFromMsg 的覆写集（adapter.cc:120-158）。
 *          头部槽 4/5/6 纯维测 dump（R6 终裁），不作语义判定输入
 *   ③ 驱动  逐段 BuildArgs→Emit（注入 HcommVerbSink 真原语）——对照
 *          OrchestrateLoop 的段循环（Plan.segments 已切好轮，不重切）
 *
 * 依赖边界：本文件是"消费方"（存量侧平行新增文件，unified_exec 目录），与
 * hcomm_verb_sink.h 同级引真实引擎头；不含设备代码（host 解释 + host 下发原语任务）。
 * host 单测经 shim 编译（blob 构造走 SerializePlanV3 直调，msg 现值手工构造）。
 */
#ifndef REFACTOR_POC_UNIFIED_EXEC_AICPU_CONSUMER_H
#define REFACTOR_POC_UNIFIED_EXEC_AICPU_CONSUMER_H

#include "verb_sink.h"
#include "hcomm_verb_sink.h"
#include "action_collect_all.h"
#include "action_spec.h"
#include "backend_registry.h"
#include "plan_serializer.h"   // v4 帧布局常量（UNIFIED_V3_*/U_V3_*）

#include "alg_param.h"

#include <string>
#include <vector>

namespace mc2_ops_hccl::refactor {

// ---------------------------------------------------------------------------
// exec 期 msg 现值（方向 B 的实例化输入）——字段集对齐 adapter.cc:120-158 覆写集
// ---------------------------------------------------------------------------
struct UnifiedExecMsg {
    u64 count{0};                       // msg.dataCnt（每 rank 元素数）
    u64 inputAddr{0};                   // msg.sendBuffer（本 rank input 设备地址）
    u64 outputAddr{0};                  // msg.recvBuffer（本 rank output 设备地址）
    u64 strideCount{0};                 // msg.strideCount（0=紧凑）
    u32 dataType{0};                    // msg.addMsg.v1Msg.hcclDataType
    u32 repeatCnt{1};                   // msg.addMsg.v1Msg.repeatCnt（0 视为 1）
    int reduceType{0};                  // msg.opType（reduce 算子的 reduce op）
    HcclMem cclBuff;                    // resCtx.cclMem（中转 buffer，COPY 目的地之一）
};

// 消费结果（诊断/测试断言用）
struct UnifiedExecResult {
    HcclResult ret{HCCL_SUCCESS};
    std::string err;                    // 失败原因
    u32 segmentsRun{0};                 // 实际驱动的段数
    std::vector<std::string> perSegTrace;   // 逐段动词 trace（TraceSink 口径——诊断与
                                            // HcommVerbSink.calls 同源，测试断言两侧一致）
};

// ---------------------------------------------------------------------------
// v4 blob 帧解读（单段）——CollectAll 段的 AICPU 消费视图
// 帧内槽位（与 SerializeSegmentV3 的 CollectAll 布局逐槽对应，plan_serializer.h）：
//   [0]=动作 ID [1]=inBaseOff [2]=outBaseOff [3]=outputStride*me [4]=mySlice
//   [5..8]=goSize（CCU 专用，AICPU 忽略）[9]=inputStride*me [10]=repeatNumInv
//   [11]/[12]=repeatStride [13]=lastSliceSize [14]=inOutEqual
// ---------------------------------------------------------------------------
struct BlobFrameView {
    u64 actionId{0};
    u64 inBaseOff{0};
    u64 outBaseOff{0};
    u64 outRankOff{0};      // 槽 3：纯 rank 偏移（stride*me，不含基址）
    u64 mySlice{0};         // 槽 4：结构期 count=0 下此槽为 0（占位）——真值 exec 期算
    u64 inRankOff{0};       // 槽 9
    u64 repeatNumInv{0};    // 槽 10
    u64 inRepeatStride{0};  // 槽 11
    u64 outRepeatStride{0}; // 槽 12
    u64 lastSliceSize{0};   // 槽 13
    u64 outRoundStrideHint{0};  // 切轮步长提示（消费方从相邻帧 outBaseOff 差恢复；
                                // 0=单帧/未切轮——InstantiateSegment 的 slice 口径依据）
};

// 整 blob 解读结果
struct BlobView {
    u64 magic{0};
    u64 version{0};
    u64 segNum{0};
    u64 dataType{0};        // 头部槽 4：纯 dump（终裁：不作语义判定输入）
    u64 outputType{0};      // 头部槽 5：纯 dump
    u64 reduceType{0};      // 头部槽 6：纯 dump
    std::vector<BlobFrameView> frames;
};

// blob（u64 槽序列）→ BlobView。三重校验（magic/版本/段数×32+7 == 总槽数）——
// 与 CCU 侧写入方头部校验（hccl_ccu_v0_prepare.h:628-649）同口径，消费侧再验。
inline bool ParseUnifiedBlobV4(const std::vector<u64>& blob, BlobView& out, std::string& err)
{
    if (blob.size() < UNIFIED_V3_HEADER_SLOTS) {
        err = "blob too small for v4 header (7 slots)";
        return false;
    }
    out.magic = blob[0];
    out.version = blob[1];
    out.segNum = blob[2];
    out.dataType = blob[UNIFIED_V3_HEADER_DATA_TYPE];
    out.outputType = blob[UNIFIED_V3_HEADER_OUTPUT_TYPE];
    out.reduceType = blob[UNIFIED_V3_HEADER_REDUCE_TYPE];
    if (out.magic != PLAN_V3_MAGIC) {
        err = "blob magic mismatch (expect UPL3)";
        return false;
    }
    if (out.version != PLAN_V3_VERSION_FIELD) {
        err = "blob version " + std::to_string(out.version) + " != " + std::to_string(PLAN_V3_VERSION_FIELD);
        return false;
    }
    const size_t expectSlots = static_cast<size_t>(UNIFIED_V3_HEADER_SLOTS) +
                               static_cast<size_t>(out.segNum) * UNIFIED_V3_SLOTS_PER_RECORD;
    if (blob.size() != expectSlots) {
        err = "blob size " + std::to_string(blob.size()) + " != 7 + segs*32 (" +
              std::to_string(expectSlots) + ")";
        return false;
    }
    out.frames.resize(static_cast<size_t>(out.segNum));
    for (size_t i = 0; i < out.segNum; ++i) {
        const u64* f = blob.data() + UNIFIED_V3_HEADER_SLOTS + i * UNIFIED_V3_SLOTS_PER_RECORD;
        BlobFrameView& v = out.frames[i];
        v.actionId = f[U_V3_ACTION_ID];
        if (v.actionId != static_cast<u64>(ActionKind::CollectAll)) {
            err = "frame " + std::to_string(i) + ": action id " + std::to_string(v.actionId) +
                  " not in R6 AICPU consumer scope (CollectAll only)";
            return false;
        }
        v.inBaseOff = f[U_V3_INPUT];
        v.outBaseOff = f[U_V3_OUTPUT];
        v.outRankOff = f[U_V3_OUTPUT_OFFSET];
        v.mySlice = f[U_V3_SLICE_SIZE];
        v.inRankOff = f[U_V3_CUR_RANK_SLICE_INPUT_OFFSET];
        v.repeatNumInv = f[U_V3_REPEAT_NUM_INV];
        v.inRepeatStride = f[U_V3_INPUT_REPEAT_STRIDE];
        v.outRepeatStride = f[U_V3_OUTPUT_REPEAT_STRIDE];
        v.lastSliceSize = f[U_V3_LAST_SLICE_SIZE];
    }
    // 切轮步长恢复：相邻帧 outBaseOff 之差（差>0 即切轮 blob 的 roundStride——
    // SplitRounds 的 ForRound baseOff 推进量，方向 B 下由消费方从结构底版自行恢复，
    // 不经 msg/capacity）。步长广播到**全部帧**（末帧与下一帧无相邻对，若只标
    // 非末帧，末帧会落回全量口径——切轮段的末轮片与非末轮同 stride 语义）
    u64 roundStride = 0;
    for (size_t i = 0; i + 1 < out.segNum; ++i) {
        const u64 diff = out.frames[i + 1].outBaseOff - out.frames[i].outBaseOff;
        if (diff > roundStride) {
            roundStride = diff;
        }
    }
    if (roundStride > 0) {
        for (size_t i = 0; i < out.segNum; ++i) {
            out.frames[i].outRoundStrideHint = roundStride;
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
// ② 实例化：结构帧 + msg 现值 → 该段的本 rank 数据参数
//
// exec 期字段解析（方向 B）：
//   sliceSize   = msg.count * dataTypeSize（结构帧的 mySlice 槽是资源期 count=0 的
//                 占位 0，不消费——真值在此计算）
//   尾片取值   = SliceSizeOf 口径（layout.tailSize 与 sliceSize 的关系由帧内
//                 lastSlice/mySlice 比对恢复；AG 场景结构期 tailSize==sliceSize 即
//                 无尾片语义，exec 期 count 整除性产生的真尾片按
//                 (tail!=0 && me==N-1) ? tail : slice 取——与 BuildArgs 同式）
//   strideCount strideFallback 照抄 adapter.cc:143-154（stride==0 且 repeat>1 且
//               AG/RS/AR → stride=count*repeat）
//   dataType    msg 现值为唯一权威源（照抄 adapter.cc:141 强等行为：
//               dataType=outputType=msg.dataType——P2 债台账已登记 RS dstType 分歧）
// ---------------------------------------------------------------------------
struct SegRuntimeParams {
    u64 sliceSize{0};        // 本段本 rank 片字节（exec 期真值）
    u64 tailSize{0};         // 尾片（非末 rank 消费时为 0 语义由 SliceSizeOf 承载）
    u64 outputSliceStride{0};
    u64 inputSliceStride{0};
    u64 repeatNum{1};
    u64 inputRepeatStride{0};
    u64 outputRepeatStride{0};
    u64 inBaseOff{0};
    u64 outBaseOff{0};
    u64 scratchBaseOff{0};
    u32 myAlgRank{0};
    u32 rankNum{0};
};

inline u64 UnifiedDataTypeSizeOf(u32 dt)
{
    // 消费方只经 msg 现值取 dataType（枚举值 = HcclDataType 底层数）
    return DATATYPE_SIZE_TABLE[dt];
}

inline SegRuntimeParams InstantiateSegment(const BlobFrameView& f, const UnifiedExecMsg& msg,
                                           u32 myAlgRank, u32 rankNum, u32 opType,
                                           u32 roundFramesHint = 1, std::string* err = nullptr)
{
    SegRuntimeParams p;
    const u64 dts = UnifiedDataTypeSizeOf(msg.dataType);
    // strideFallback（adapter.cc:143-154 同式）
    u64 strideCount = msg.strideCount;
    const u32 repeatCnt = (msg.repeatCnt == 0U) ? 1U : msg.repeatCnt;
    if (strideCount == 0U && repeatCnt > 1U &&
        (opType == static_cast<u32>(HcclCMDType::HCCL_CMD_ALLGATHER) ||
         opType == static_cast<u32>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER) ||
         opType == static_cast<u32>(HcclCMDType::HCCL_CMD_ALLREDUCE))) {
        strideCount = msg.count * repeatCnt;
    }
    // 布局恢复：帧内槽位是结构底版（基址推进量/stride/repeat 语义）——
    //   outputSliceStride：结构帧槽 3 是 outStride*me——除以 me 恢复 stride？
    //   不除：槽 3 除 me 在 me=0 时除零。改从槽 13/4 推不可靠（count=0 占位）。
    //   ★消费方约定：blob 是结构底版，per-rank 偏移槽（3/9）保留"结构期 me"的
    //   乘积形态不可复原除数——故 Compile 侧 serialize 时这些槽以"结构期 me"写入，
    //   消费方以**同 me**（域成员与 myRank 在资源期与 exec 期一致——同一 comm 的
    //   rank 布局不变）直接消费乘积值，无需复原。sliceSize 等数据量真值则全部
    //   exec 期现算（下方）。
    p.repeatNum = (~0ULL - f.repeatNumInv);          // 槽 10 倒数恢复（==1 → 1）
    p.myAlgRank = myAlgRank;
    p.rankNum = rankNum;
    p.inBaseOff = msg.inputAddr + f.inBaseOff;       // 结构基址推进 + exec 期 input 现址
    p.outBaseOff = msg.outputAddr + f.outBaseOff;
    p.scratchBaseOff = reinterpret_cast<u64>(msg.cclBuff.addr);
    p.inputSliceStride = 0;                          // AG 语义：input 无片间步长
    // ★切轮段的全片口径：msg.count 是**全量**每 rank 元素数，切轮段的每轮片 =
    // 全量 slice / 帧数（SplitRounds 的 loopTimes）。帧数可从 blob 恢复
    //（segNum），每轮片 = 全量 slice / segNum（整除时；SplitRounds 的切轮以
    // maxPerRound 为步长，段数 = ceil(full/maxPerRound)，非末轮恒 maxPerRound、
    // 末轮余量——消费方统一按"全量/段数"整除口径取片，非整除的余量场景在
    // 方向 B 下 blob 恒单帧（count=0 资源期切不出多帧），多帧场景必来自带真值
    // count 的 Compile（其切轮整除性由 SplitRounds 保证与段数一致）。
    // roundStrideHint（相邻帧 outBaseOff 差）同时恢复 stride 口径：切轮段的
    // stride == roundStride（ForRound 不改 outputSliceStride，全量 stride 与
    // 轮 stride 同值——SplitRounds 传 maxPerRound 作推进、layout.stride 不变）。
    const u64 fullSlice = msg.count * dts;
    const bool isRounded = (f.outRoundStrideHint > 0U);
    const u64 segSlice = isRounded ? (fullSlice / static_cast<u64>(roundFramesHint)) : fullSlice;
    p.sliceSize = segSlice;
    p.outputSliceStride = (strideCount == 0U) ? fullSlice : strideCount * dts;
    // capacity 防御（终审收尾件 3——R6 落防御、R7 首补丁补协议面）：方向 B 下
    // 资源期 blob 按 count=0 单帧编译，exec 期 msg 现值可能远超资源期中转上限
    //（如 cclBuff 4KB 但 msg 8KB——超轮窗口真实存在）。切轮 blob 的步长即
    // SplitRounds 的 maxPerRound（512 对齐）——实例化片超它 = msg 与 blob 的容量
    // 约束脱节，静默错跑会读/写越界：WARNING + err 置位（caller 拒收）。
    // 单帧 blob 无步长可依（hint=0），容量自查随 R7 的 capacity 进协议（msg 附带
    // 或 blob 槽 3 携带）——本轮只守"切轮 blob 的片不得超步长"这一条硬不变量。
    if (isRounded && segSlice > f.outRoundStrideHint) {
        if (err != nullptr) {
            *err = "InstantiateSegment: segSlice " + std::to_string(segSlice) +
                   " exceeds round stride " + std::to_string(f.outRoundStrideHint) +
                   " (msg count drift from resource-phase capacity; refusing silent mis-run)";
        }
        return p;   // err 置位 = 拒收（p 为部分填充，caller 不消费）
    }
    // AG 尾片：count 不被 rankNum 整除时末 rank 短片（SliceSizeOf 语义；AG 现状
    // executor 硬编码 tail==slice 即无尾片——概念不同源，统一走 SliceSizeOf 取值式）
    const u64 tail = (msg.count % rankNum != 0U) ? (msg.count % rankNum) * dts : 0U;
    p.tailSize = tail;
    p.inputRepeatStride = f.inRepeatStride;
    p.outputRepeatStride = f.outRepeatStride;
    return p;
}

// ---------------------------------------------------------------------------
// ③ 驱动：blob + msg + 资源束 → 逐段 Emit（注入 HcommVerbSink）
//
// 对照 OrchestrateLoop（ins_v2_all_gather_sole_executor.cc:95-190）的段循环：
//   Plan.segments 已切好轮（L1Adapter::Compile 的 RoundCapacity）——逐段顺序执行，
//   不重切。每段：InstantiateSegment → CollectAll::BuildArgs → AicpuCollectAll::Emit
//   （sink 注入 HcommVerbSink → 真原语）。
//   ★段序即轮序：切轮段的基址推进已在帧槽 1/2（inBaseOff/outBaseOff 的段间差），
//     消费方无需重算推进量——与 SplitRounds 的 ForRound(baseOff=round*stride) 对应。
// ---------------------------------------------------------------------------
inline UnifiedExecResult ConsumeUnifiedPlanAicpu(
    const std::vector<u64>& blob,          // v4 blob（HcclEngineCtxGet D2H 产物）
    const UnifiedExecMsg& msg,             // exec 期 msg 现值
    u32 myRank,                            // 本 rank（userRank）
    const std::vector<u32>& domainRanks,   // 域成员（含本 rank；resCtx 恢复或 msg 附带）
    u32 opType,                            // HcclCMDType 底层数
    const HcommVerbRes& res)               // 线程/通道资源束
{
    UnifiedExecResult out;
    // ① blob 解读（三重校验）
    BlobView view;
    if (!ParseUnifiedBlobV4(blob, view, out.err)) {
        out.ret = HCCL_E_PARA;
        return out;
    }
    if (domainRanks.empty()) {
        out.ret = HCCL_E_PARA;
        out.err = "empty domain ranks";
        return out;
    }
    // myAlgRank（域内序）——与 Compile 期 MyAlgRank 同式
    u32 myAlgRank = 0;
    bool found = false;
    for (u32 i = 0; i < domainRanks.size(); ++i) {
        if (domainRanks[i] == myRank) {
            myAlgRank = i;
            found = true;
            break;
        }
    }
    if (!found) {
        out.ret = HCCL_E_PARA;
        out.err = "myRank not in domain ranks";
        return out;
    }
    const u32 rankNum = static_cast<u32>(domainRanks.size());

    // 后端条目（CollectAll/AICPU）
    IActionBackend* be = BackendRegistry::Peek(ActionKind::CollectAll, Engine::AICPU);
    if (be == nullptr) {
        out.ret = HCCL_E_NOT_SUPPORT;
        out.err = "AICPU CollectAll backend not registered";
        return out;
    }

    // ②③ 逐段：实例化 → BuildArgs → Emit（sink 逐段新建——READ 线程序按段从
    // threads[0] 起算，存量 KernelRun 逐轮重入同型）
    LoopSplitter splitter;   // BuildArgs 的 goSize 形参（AICPU 路径不消费）
    for (size_t segIdx = 0; segIdx < view.frames.size(); ++segIdx) {
        const BlobFrameView& f = view.frames[segIdx];
        std::string instErr;
        const SegRuntimeParams p = InstantiateSegment(
            f, msg, myAlgRank, rankNum, opType, static_cast<u32>(view.frames.size()), &instErr);
        if (!instErr.empty()) {
            out.ret = HCCL_E_PARA;
            out.err = "segment " + std::to_string(segIdx) + ": " + instErr;
            out.segmentsRun = static_cast<u32>(segIdx);
            return out;
        }

        // ActionSpec 重建（BuildArgs 的输入形状——只填其消费字段）
        ActionSpec a;
        a.kind = ActionKind::CollectAll;
        a.layout.sliceSize = p.sliceSize;
        a.layout.tailSize = p.tailSize;
        a.layout.inputSliceStride = p.inputSliceStride;
        a.layout.outputSliceStride = p.outputSliceStride;
        a.layout.repeatNum = p.repeatNum;
        a.layout.inputRepeatStride = p.inputRepeatStride;
        a.layout.outputRepeatStride = p.outputRepeatStride;
        a.layout.inBaseOff = p.inBaseOff;
        a.layout.outBaseOff = p.outBaseOff;
        a.layout.scratchBaseOff = p.scratchBaseOff;
        a.layout.dataTypeSize = UnifiedDataTypeSizeOf(msg.dataType);
        a.domain.ranks = domainRanks;
        a.domain.myRank = myRank;
        a.transport = TransportPolicy::VIA_CCUBUF;   // AG mesh 非 symmem 缺省（Compile 同判）
        a.sync.preBarrier = true;
        a.sync.postBarrier = true;

        const CollectAllArgs args = CollectAll::BuildArgs(a, splitter);

        // Emit：真 sink 驱动原语；trace 由本段 sink 调用记录重放（FireVerb 注入 sink
        // 时 EmitCtx.trace 不再由 TraceSink 兜底拼接——同源双收从 sink 侧重建）。
        // ★多段（切轮）语义：READ 计数按段清零——sink 构造逐段新建（存量 KernelRun
        // 逐轮重入同型：每轮 LocalDataCopy→PreSync→reads→PostSync，线程序每轮从
        // threads[0] 重新起算，ins_temp_all_gather_mesh_1D.cc:103/:216 在 RunAllGatherMesh
        // 调用内计数）。复用同一 res（线程/通道是进程级资源）。
        HcommVerbSink segSink(res);
        EmitCtx ctx;
        ctx.sink = &segSink;
        be->Emit(a, &args, ctx);
        std::string segTrace;
        for (const HcommCallRecord& rec : segSink.Calls()) {
            VerbCall vc;
            vc.verb = rec.verb; vc.src = rec.p0; vc.dst = rec.p1; vc.size = rec.p2; vc.peer = rec.peer;
            TraceSink replay(&segTrace);
            replay.Emit(vc);
        }
        out.perSegTrace.push_back(segTrace);

        if (segSink.ErrCode() != 0) {
            out.ret = HCCL_E_INTERNAL;
            out.err = "segment " + std::to_string(segIdx) + ": " + segSink.Err() +
                      " (primitive ret=" + std::to_string(segSink.ErrCode()) + ")";
            out.segmentsRun = static_cast<u32>(segIdx + 1);
            return out;
        }
    }
    out.segmentsRun = static_cast<u32>(view.frames.size());
    return out;
}

} // namespace mc2_ops_hccl::refactor

#endif
