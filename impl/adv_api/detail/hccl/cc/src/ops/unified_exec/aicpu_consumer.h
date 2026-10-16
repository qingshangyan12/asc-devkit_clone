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
#include "action_reduce_slice.h"   // R12 · WP-9⑥：ReduceSlice::BuildArgs（RS 段分派）
#include "backend_registry.h"
#include "backend_aicpu_reduce_slice.h"   // R12 · WP-9⑥：AicpuReduceSlice 注册（include 即注册）
#include "action_spec.h"
#include "plan_serializer.h"   // v4 帧布局常量（UNIFIED_V3_*/U_V3_* + R15 U_V3_TIER_*）
#include "segment_orchestrator.h"   // R14 · WP-2：并发编排器（env 门控分叉半边）
#include "unified_route.h"     // R14：UseUnifiedConcurrencyPath（三层灰度开关）
#include "tier_expand.h"       // R15 · ①：ExpandTieredSegments（tier 骨架 exec 期展开）

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
//
// R12 · WP-9⑥（2026-10-09）：ReduceSlice 段的 AICPU 消费支持——帧视图扩 RS 字段
//（U_V3_RS_* 布局，plan_serializer.h:88-104：槽 3=scratch 非 AG 的 outputOffset——
// 逐槽语义按帧 kind 分读，R12-2⑥"不得按 AG 语义误读"）。RS 的 goSize/chunk 槽
// AICPU 不消费（CCU kernel 口径），解读层只取 in/out/scratch/rankSliceOffset 四槽。
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
    // RS 段字段（actionId==ReduceSlice 时槽 3 的 RS 语义——scratch 基址）
    u64 rsScratch{0};       // 槽 3 RS 读法：scratch 中转基址（hcclBuff 语义）
    u64 rsRankSliceOffset{0}; // 槽 4 RS 读法：rankId*inputSliceStride（纯偏移）
    // R15 · ① tier 承载槽（仅帧 0 有效——Plan 级数据挂首段，M0_R15 §一）：
    // 槽 16==1 → 槽 17..24 九值有效（hasTier 判别式，非 tier 恒 0 是序列化器
    // 清零的构造性事实——M0 §三穷举）。非 tier 帧 tierHas=0，九值不消费。
    u64 tierHas{0};               // 槽 16：1=tier 有效
    u64 tierThreshBytes{0};       // 槽 17
    u64 tierPartRatioNum{0};      // 槽 18
    u64 tierPartRatioDen{0};      // 槽 19
    u64 tierBwFastNum{0};         // 槽 20
    u64 tierBwFastDen{0};         // 槽 21
    u64 tierBwSlowNum{0};         // 槽 22
    u64 tierBwSlowDen{0};         // 槽 23
    u64 tierAlignBytes{0};        // 槽 24
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
        // R12 · WP-9⑥：帧 kind 分派——CollectAll（AG 布局）/ReduceSlice（RS 布局，
        // U_V3_RS_*，plan_serializer.h:88-104）两套读法；槽 3/4 语义按 kind 分读
        //（AG：outputOffset/mySlice；RS：scratch/rankSliceOffset——R12-2⑥ 不得互串）。
        // 其余动作 id 显式拒（E_PARA 语义 err——与 Validate 的 E_NOT_SUPPORT 分层：
        // 此处是格式面拒，Validate 是能力面拒）。
        if (v.actionId == static_cast<u64>(ActionKind::ReduceSlice)) {
            v.inBaseOff = f[U_V3_RS_INPUT];
            v.outBaseOff = f[U_V3_RS_OUTPUT];
            v.rsScratch = f[U_V3_RS_SCRATCH];
            v.rsRankSliceOffset = f[U_V3_RS_RANK_SLICE_OFFSET];
            // RS 无 per-rank output 偏移（kernel currentRankSliceOutputOffset 恒 0）
            v.outRankOff = 0;
            v.mySlice = 0;             // RS 槽 4 是 rankSliceOffset（已读）——mySlice 不消费
            // goSize/chunk 槽 5..15 是 CCU kernel 口径，AICPU 不读（不误读即不赋值）
        } else if (v.actionId == static_cast<u64>(ActionKind::CollectAll)) {
            v.inBaseOff = f[U_V3_INPUT];
            v.outBaseOff = f[U_V3_OUTPUT];
            v.outRankOff = f[U_V3_OUTPUT_OFFSET];
            v.mySlice = f[U_V3_SLICE_SIZE];
            v.inRankOff = f[U_V3_CUR_RANK_SLICE_INPUT_OFFSET];
            v.repeatNumInv = f[U_V3_REPEAT_NUM_INV];
            v.inRepeatStride = f[U_V3_INPUT_REPEAT_STRIDE];
            v.outRepeatStride = f[U_V3_OUTPUT_REPEAT_STRIDE];
            v.lastSliceSize = f[U_V3_LAST_SLICE_SIZE];
        } else {
            err = "frame " + std::to_string(i) + ": action id " + std::to_string(v.actionId) +
                  " not in AICPU consumer scope (CollectAll/ReduceSlice)";
            return false;
        }
        // R15 · ① tier 槽读取（M0 §一/§三）：槽 16..24 只在帧 0 读取（tier 写入
        // 方只写段 0 帧——SerializePlanV3 的 TierSkeletonCanonical gate；槽 16==1
        // 是唯一判别，非 tier Plan 槽 16 恒 0 无反例）。非帧 0 不读（保持 0 缺省）。
        if (i == 0) {
            v.tierHas = f[U_V3_TIER_HAS_TIER];
            if (v.tierHas == 1ULL) {
                v.tierThreshBytes = f[U_V3_TIER_THRESH_BYTES];
                v.tierPartRatioNum = f[U_V3_TIER_PART_RATIO_NUM];
                v.tierPartRatioDen = f[U_V3_TIER_PART_RATIO_DEN];
                v.tierBwFastNum = f[U_V3_TIER_BW_FAST_NUM];
                v.tierBwFastDen = f[U_V3_TIER_BW_FAST_DEN];
                v.tierBwSlowNum = f[U_V3_TIER_BW_SLOW_NUM];
                v.tierBwSlowDen = f[U_V3_TIER_BW_SLOW_DEN];
                v.tierAlignBytes = f[U_V3_TIER_ALIGN_BYTES];
            }
        }
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
{    SegRuntimeParams p;
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
// R12 · WP-9⑥（2026-10-09）：RS 段的 exec 现值实例化（对照 CCU 侧 R7 覆写名单
// hccl_ccu_v0_prepare.h:787-839 的 AICPU 投影）：
//   in/out 合成式（msg 现址 + 帧 inBaseOff/outBaseOff 结构量）、
//   scratch 直替（msg.cclBuff——资源期 blob 槽是占位，R12-2⑥）、
//   rankSliceOffset 三分支（strideCount==0 → sliceSize : strideCount*dts——CCU
//   写入方 :802-803 同式）。AICPU 不消费 goSize/chunk 槽（CCU kernel 口径——
//   写侧几何的 sliceSize 由 sliceStride 语义承载，无 chunk 循环）。
// ---------------------------------------------------------------------------
inline SegRuntimeParams InstantiateRsSegment(const BlobFrameView& f, const UnifiedExecMsg& msg,
                                              u32 myAlgRank, u32 rankNum, std::string* err)
{
    SegRuntimeParams p;
    if (err != nullptr) {
        err->clear();
    }
    const u64 dts = UnifiedDataTypeSizeOf(msg.dataType);
    const u32 repeatCnt = (msg.repeatCnt == 0U) ? 1U : msg.repeatCnt;
    const u64 repeatOffset = msg.count * repeatCnt * dts;   // RS：count×repeat（每 rank 输出份）
    p.myAlgRank = myAlgRank;
    p.rankNum = rankNum;
    p.repeatNum = 1;
    p.inBaseOff = msg.inputAddr + repeatOffset + f.inBaseOff;
    p.outBaseOff = msg.outputAddr + repeatOffset + f.outBaseOff;
    p.scratchBaseOff = reinterpret_cast<u64>(msg.cclBuff.addr);   // scratch 直替（v2 :547 同式）
    // RS 语义：msg.count = 每 rank 输出份元素数（selector 口径）——sliceSize = count×dts
    const u64 sliceSize = msg.count * dts;
    p.sliceSize = sliceSize;
    p.tailSize = 0;   // RS 尾片在 chunk/tail 槽（CCU 口径）——写侧逐 peer 片由
                      // SliceSizeOf(peer,n) 从 sliceSize 推（无尾片构造），P2 尾片在此扩展
    // inputSliceStride：RS 读"对端 input 的我的片"→ 写"我的 input 的 peer 的份"——
    // stride = strideCount==0 ? sliceSize : strideCount*dts（executor :152-153 同式）
    p.inputSliceStride = (msg.strideCount == 0U) ? sliceSize : msg.strideCount * dts;
    p.outputSliceStride = (msg.strideCount == 0U) ? sliceSize : msg.strideCount * dts;
    p.inputRepeatStride = 0;
    p.outputRepeatStride = 0;
    // rankSliceOffset 三分支（CCU 写入方 :802-803 同式——写侧 src 推导的输入；
    // AICPU 消费方直接在 BuildArgs 内由 layout 推，此字段仅供诊断对照）
    (void)f.rsRankSliceOffset;
    if (err != nullptr && sliceSize == 0ULL) {
        *err = "InstantiateRsSegment: exec sliceSize is 0 (msg count missing?)";
    }
    return p;
}

// ---------------------------------------------------------------------------
// R14 · WP-2 分叉半边（单源件，2026-10-10）：段重建 + 段驱动。
//
// 与既有 for 循环体（:462-550）的**单源关系**：本两函数是循环体"实例化 →
// ActionSpec 组装 → BuildArgs → Emit → trace 重放"的逐行提抽（Instantiate*/
// BuildArgs/Emit 全部同一函数调用，非第二实现）；既有循环体保持原样（env 关 =
// 零变化），分叉分支经此闭包获得同一份重建逻辑——两路输出的逐条相等由
// exec 测试 env 开/关对拍断言锁（常跑，非一次性）。
//
// R16 · 8f：tier 骨架段构造唯一实现点（BuildTierSkeletonSeg，下方）——原
// skelFast/skelSlow 两个手写 16 字段块（14 字段同源、真差异仅帧序/域表/pattern
// 三自由度）收敛为同一构造器。
// ---------------------------------------------------------------------------
inline ActionSpec RebuildActionSpecFromFrame(const BlobFrameView& f, bool isRs,
                                             const UnifiedExecMsg& msg, u32 myAlgRank, u32 rankNum,
                                             u32 opType, u32 frameTotal,
                                             const std::vector<u32>& domainRanks, u32 myRank,
                                             std::string& err)
{
    ActionSpec a;
    SegRuntimeParams p;
    if (isRs) {
        p = InstantiateRsSegment(f, msg, myAlgRank, rankNum, &err);
        if (!err.empty()) {
            return a;
        }
    } else {
        p = InstantiateSegment(f, msg, myAlgRank, rankNum, opType, frameTotal, &err);
        if (!err.empty()) {
            return a;
        }
    }
    a.kind = isRs ? ActionKind::ReduceSlice : ActionKind::CollectAll;
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
    a.transport = TransportPolicy::VIA_CCUBUF;   // AG/RS mesh 非 symmem 缺省（Compile 同判）
    a.sync.preBarrier = true;
    a.sync.postBarrier = true;
    return a;
}

// R16 · 8f：tier 骨架段构造唯一实现点（M0_R15 §二位置语义：seg0=快轴 mesh /
// seg1=慢轴 nhr——pattern 按位置赋值，规范形态 gate 在写入方 TierSkeletonCanonical
// 已保证）。base 取 Compile 期结构底版口径（帧槽 1/2 的 inBaseOff/outBaseOff，
// 不加 InstantiateRsSegment 的 repeatOffset/msg 现址——骨架基址是 ForChunk/
// fastBase 切分的段内相对量，绝对化在展开段驱动期由 DriveSegmentViaSink 链路
// 完成；提前加 msg 现址会让切分基址被双计。#22 对拍的路 B 单源锚）。
// 原两处手写块（skelFast/skelSlow，14 字段同源）收敛于此：差异仅
// frame（帧 0/1）/domRanks（level0/level1 域表）/pattern（MESH/NHR）三参数。
inline ActionSpec BuildTierSkeletonSeg(const BlobFrameView& frame,
                                       const std::vector<u32>& domRanks,
                                       CommPattern pattern, u32 myRank,
                                       u64 rsSliceSize, u64 rsStride, u64 dts,
                                       u64 repeatNum, u64 scratchBase)
{
    ActionSpec seg;
    seg.kind = ActionKind::ReduceSlice;
    seg.layout.sliceSize = rsSliceSize;
    seg.layout.inputSliceStride = rsStride;
    seg.layout.outputSliceStride = rsStride;
    seg.layout.repeatNum = repeatNum;
    seg.layout.inputRepeatStride = 0;
    seg.layout.outputRepeatStride = 0;
    seg.layout.inBaseOff = frame.inBaseOff;
    seg.layout.outBaseOff = frame.outBaseOff;
    seg.layout.scratchBaseOff = scratchBase;
    seg.layout.dataTypeSize = dts;
    seg.domain.ranks = domRanks;
    seg.domain.myRank = myRank;
    seg.pattern = pattern;
    if (pattern == CommPattern::NHR) {
        seg.nhrJettyNum = 1U;   // devkit RS NHR 链恒值（KFC_RS_NHR_PORT_NUM=1）
    }
    seg.transport = TransportPolicy::VIA_CCUBUF;
    return seg;
}

// 段驱动：BuildArgs → Emit（sink 逐段新建——与循环体 :507 同型；域子束/全局束
// 由 caller 注入 res，sink 内部零感知）→ trace 重放（:536-541 同型）。
inline OrchestrateSegResult DriveSegmentViaSink(const ActionSpec& a, const UnifiedExecMsg& msg,
                                                u32 myAlgRank, u32 rankNum, u32 opType,
                                                u32 frameTotal, IActionBackend* beAg,
                                                IActionBackend* beRs, const HcommVerbRes& res)
{
    (void)msg; (void)myAlgRank; (void)rankNum; (void)opType; (void)frameTotal;   // 已在重建期消费
    OrchestrateSegResult out;
    const bool isRs = (a.kind == ActionKind::ReduceSlice);
    HcommVerbSink segSink(res);
    EmitCtx ctx;
    ctx.sink = &segSink;
    if (isRs) {
        ReduceSliceArgs rsArgs;
        std::string rsBuildErr;
        const HcclResult rsRet = ReduceSlice::BuildArgs(a, rsArgs, rsBuildErr);
        if (rsRet != HCCL_SUCCESS) {
            out.ret = rsRet;
            out.err = "ReduceSlice BuildArgs: " + rsBuildErr;
            return out;
        }
        beRs->Emit(a, &rsArgs, ctx);
    } else {
        LoopSplitter splitter;   // BuildArgs 的 goSize 形参（AICPU 路径不消费）
        const CollectAllArgs args = CollectAll::BuildArgs(a, splitter);
        beAg->Emit(a, &args, ctx);
    }
    std::string segTrace;
    for (const HcommCallRecord& rec : segSink.Calls()) {
        VerbCall vc;
        vc.verb = rec.verb; vc.src = rec.p0; vc.dst = rec.p1; vc.size = rec.p2; vc.peer = rec.peer;
        TraceSink replay(&segTrace);
        replay.Emit(vc);
        out.calls.push_back(vc);
    }
    if (segSink.ErrCode() != 0) {
        out.ret = HCCL_E_INTERNAL;
        out.err = segSink.Err() + " (primitive ret=" + std::to_string(segSink.ErrCode()) + ")";
        return out;
    }
    out.trace = segTrace;
    return out;
}

// ---------------------------------------------------------------------------
// ③ 驱动：blob + msg + 资源束 → 逐段 Emit（注入 HcommVerbSink）
//
// 对照 OrchestrateLoop（ins_v2_all_gather_sole_executor.cc:95-190）的段循环：
//   Plan.segments 已切好轮（L1Adapter::Compile 的 RoundCapacity）——逐段顺序执行，
//   不重切。每段：InstantiateSegment → CollectAll::BuildArgs → AicpuCollectAll::Emit
//   （sink 注入 HcommVerbSink → 真原语）。R12 · WP-9⑥：ReduceSlice 段走
//   InstantiateRsSegment → ReduceSlice::BuildArgs → AicpuReduceSlice::Emit（帧
//   kind 分派，R12-2⑥）。
//   ★段序即轮序：切轮段的基址推进已在帧槽 1/2（inBaseOff/outBaseOff 的段间差），
//     消费方无需重算推进量——与 SplitRounds 的 ForRound(baseOff=round*stride) 对应。
// ---------------------------------------------------------------------------
inline UnifiedExecResult ConsumeUnifiedPlanAicpu(
    const std::vector<u64>& blob,          // v4 blob（HcclEngineCtxGet D2H 产物）
    const UnifiedExecMsg& msg,             // exec 期 msg 现值
    u32 myRank,                            // 本 rank（userRank）
    const std::vector<u32>& domainRanks,   // 域成员（含本 rank；resCtx 恢复或 msg 附带）
    u32 opType,                            // HcclCMDType 底层数
    const HcommVerbRes& res,               // 线程/通道资源束
    // R15 · ① tier 慢轴域成员（level1——双域配方域 1；非 tier 调用方不传 = 空缺省，
    // tier blob 遇空显式拒收。既有调用方零改动——参数尾追加带缺省）
    const std::vector<u32>& domainRanks2 = std::vector<u32>())
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

    // R15 · ① tier 骨架展开分叉（M0_R15 §二数据流；槽 16==1 判别——非 tier blob
    // tierHas=0 零行为变化）：tier 骨架（2 段 RS：帧 0 mesh + 帧 1 NHR）在消费方
    // exec 期展开（R6-③ 裁决不预编——骨架即 blob，A/B 由 msg.count 现值定）。
    // 展开产物（A 交错 4 段 / B 步进 2S 段，sync 位/pattern 齐全）替换骨架段序列
    // 进下方既有驱动路径（编排器分叉或串行 for——两条共用本展开，单源）。
    // 域成员：骨架双域——帧 0 段用 domainRanks（快轴 mesh 域，level0），帧 1 段
    // 用 domainRanks2（慢轴 nhr 域，level1；tier blob 遇空显式拒收——组装段
    // 断层不得静默退化单域）。
    std::vector<ActionSpec> tierExpanded;   // 非 tier 恒空 = 走骨架直驱（现状路径）
    // tier 判别（帧 0 tierHas）：segNum==0 的空 blob 已在消费方上游拒（segs=0 走
    // E_PARA 的既有闸——见 unified_aicpu_exec.cc 空计划拒收；此处防御 frames 非空）
    if (!view.frames.empty()) {
        const BlobFrameView& head = view.frames[0];
        if (head.tierHas == 1ULL && view.frames.size() == 2U &&
            head.actionId == static_cast<u64>(ActionKind::ReduceSlice) &&
            view.frames[1].actionId == static_cast<u64>(ActionKind::ReduceSlice)) {
        // TierParams 重建（槽 17..24 九值 → action_spec.h 值对象）
        TierParams tier;
        tier.hasTier = true;
        tier.threshBytes = head.tierThreshBytes;
        tier.interleave.partRatioNum = head.tierPartRatioNum;
        tier.interleave.partRatioDen = (head.tierPartRatioDen != 0ULL) ? head.tierPartRatioDen : 1ULL;
        tier.stepwise.bwFastNum = head.tierBwFastNum;
        tier.stepwise.bwFastDen = (head.tierBwFastDen != 0ULL) ? head.tierBwFastDen : 1ULL;
        tier.stepwise.bwSlowNum = head.tierBwSlowNum;
        tier.stepwise.bwSlowDen = (head.tierBwSlowDen != 0ULL) ? head.tierBwSlowDen : 1ULL;
        tier.stepwise.alignBytes = (head.tierAlignBytes != 0ULL) ? head.tierAlignBytes : 512ULL;
        // 骨架段重建：位置语义（seg0=快轴 mesh / seg1=慢轴 nhr——tier_expand.h
        // :304-307 segFast/segSlow 同位序；pattern 按位置赋值，规范形态 gate 在
        // 写入方 TierSkeletonCanonical 已保证）。RS 实例化用 Instantiate 单源件
        //——但 base 合成取 **Compile 期结构底版口径**（f.inBaseOff 结构量，不加
        // InstantiateRsSegment 的 repeatOffset/msg 现址——骨架基址是 ForChunk/
        // fastBase 切分的段内相对量，绝对化在展开段驱动期由 DriveSegmentViaSink
        // 链路完成；此处提前加 msg 现址会让 ForChunk 切分基址被双计。单源锚 =
        // #22 对拍的路 B（直喂 Compile 骨架展开）逐段相等）。
        const u64 dts = UnifiedDataTypeSizeOf(msg.dataType);
        const u64 totalBytes = msg.count * dts;   // RS 口径：count = 每 rank 输出份
        const u64 rsSliceSize = msg.count * dts;
        const u64 rsStride = (msg.strideCount == 0ULL) ? rsSliceSize : msg.strideCount * dts;
        const u64 rsRepeatNum = (msg.repeatCnt == 0U) ? 1U : msg.repeatCnt;
        const u64 scratchBase = reinterpret_cast<u64>(msg.cclBuff.addr);
        // 骨架段重建（R16 · 8f：BuildTierSkeletonSeg 唯一实现点——结构底版直取，
        // slice/stride 由 msg 现值重算（InstantiateRsSegment 同式但 base 不绝对化）；
        // skelFast/skelSlow 原 15/16 字段手写块收敛为两行调用）
        const ActionSpec skelFast = BuildTierSkeletonSeg(
            view.frames[0], domainRanks, CommPattern::MESH, myRank,
            rsSliceSize, rsStride, dts, rsRepeatNum, scratchBase);
        // 慢轴域成员：level1（tierSlowRanks 由调用方经 resCtx.algHierarchyInfo
        // infos[0][1] 提供——unified_aicpu_exec.cc ②段；host 测试直填）。退化
        // （空/不含 myRank）时拒收：双域配方域 1 必含 myRank，空 = 组装段断层。
        if (!domainRanks2.empty()) {
            // 域 1 必含 myRank（双域配方约定）——校验后 slowAlg 供诊断（骨架
            // 重建的 myRank 即 myRank 本身，不再需要 slowAlg 推导 layout）
            u32 slowAlg = 0;
            bool slowFound = false;
            for (u32 i = 0; i < domainRanks2.size(); ++i) {
                if (domainRanks2[i] == myRank) {
                    slowAlg = i;
                    slowFound = true;
                    break;
                }
            }
            (void)slowAlg;
            if (!slowFound) {
                out.ret = HCCL_E_PARA;
                out.err = "tier slow-domain ranks do not contain myRank (level1 hierarchy missing?)";
                return out;
            }
            const ActionSpec skelSlow = BuildTierSkeletonSeg(
                view.frames[1], domainRanks2, CommPattern::NHR, myRank,
                rsSliceSize, rsStride, dts, rsRepeatNum, scratchBase);
            std::vector<ActionSpec> skeleton = {skelFast, skelSlow};
            std::string expandErr;
            if (!ExpandTieredSegments(skeleton, tier, totalBytes, tierExpanded, expandErr)) {
                out.ret = HCCL_E_PARA;
                out.err = "tier expand: " + expandErr;
                return out;
            }
        } else {
            out.ret = HCCL_E_PARA;
            out.err = "tier blob but level1 domain ranks missing (resource-phase assembly gap)";
            return out;
        }
    }
    }
    // tier 展开后的驱动：展开段已带 pattern/sync/domain（ExpandTieredSegments 产物
    // ——骨架段拷贝携带 ranks，域成员无需再注入），直接进两条既有驱动路径之一
    //（编排器分叉或串行逐段 DriveSegmentViaSink——同一 driver 单源件，与骨架帧
    // 路径的"重建后驱动"共用 DriveSegmentViaSink，无第二实现）。非 tier =
    // tierExpanded 空，走下方骨架帧路径（现状零变化）。

    // 后端条目（CollectAll/AICPU + R12 · WP-9⑥：ReduceSlice/AICPU——帧 kind 分派，
    // R12-2⑥：:319/:343 的 CollectAll 硬编码改帧 kind 分派）
    IActionBackend* beAg = BackendRegistry::Peek(ActionKind::CollectAll, Engine::AICPU);
    IActionBackend* beRs = BackendRegistry::Peek(ActionKind::ReduceSlice, Engine::AICPU);
    if (beAg == nullptr) {
        out.ret = HCCL_E_NOT_SUPPORT;
        out.err = "AICPU CollectAll backend not registered";
        return out;
    }

    // R14 · WP-2：A 形态并发编排分叉（env 门控——MC2_USE_UNIFIED_CONCURRENCY 三层
    // 开关，unified_route.h UseUnifiedConcurrencyPath；缺省关 = 走下方既有串行 for
    // **一行不动**）。分叉分支以 driver 闭包把"重建一段 + 驱动一段"单源化注入
    // OrchestrateSegments（l5/segment_orchestrator.h：域分组/线程切池/批划分/
    // 合取屏障/降级）；重建逻辑（Instantiate*/ActionSpec 组装/BuildArgs/Emit）与
    // 下方循环体同一份（DriverOfFrame 闭包捕获 msg/myAlgRank/rankNum/opType/
    // beAg/beRs——两路共用，漂移由 exec 测试 env 开/关对拍断言锁，常跑）。
    // R15 · ① tier 展开段序列优先（tierExpanded 非空时以展开段替换骨架重建——
    // 展开段由上方 tier 分叉单源产出（InstantiateRsSegment+ExpandTieredSegments），
    // driver 同一 DriveSegmentViaSink；非 tier 走既有帧重建路径零变化）。
    if (UseUnifiedConcurrencyPath()) {
        OrchestratorRes orchRes;
        orchRes.threads = res.threads;
        orchRes.channels = res.channels;
        std::vector<ActionSpec> rebuilt;
        if (!tierExpanded.empty()) {
            // tier 展开段：重建已在上文完成（含 pattern/sync/domain），仅校验后端
            if (beRs == nullptr) {
                out.ret = HCCL_E_NOT_SUPPORT;
                out.err = "tier segment: AICPU ReduceSlice backend not registered "
                          "(R12 WP-9 expected registered)";
                return out;
            }
            rebuilt = tierExpanded;
        } else {
            rebuilt.reserve(view.frames.size());
            for (size_t segIdx = 0; segIdx < view.frames.size(); ++segIdx) {
                const BlobFrameView& f = view.frames[segIdx];
                const bool isRs = (f.actionId == static_cast<u64>(ActionKind::ReduceSlice));
                if (isRs && beRs == nullptr) {
                    out.ret = HCCL_E_NOT_SUPPORT;
                    out.err = "segment " + std::to_string(segIdx) +
                              ": AICPU ReduceSlice backend not registered (R12 WP-9 expected registered)";
                    out.segmentsRun = static_cast<u32>(segIdx);
                    return out;
                }
                std::string buildErr;
                ActionSpec a = RebuildActionSpecFromFrame(f, isRs, msg, myAlgRank, rankNum, opType,
                                                          static_cast<u32>(view.frames.size()),
                                                          domainRanks, myRank, buildErr);
                if (!buildErr.empty()) {
                    out.ret = HCCL_E_PARA;
                    out.err = "segment " + std::to_string(segIdx) + ": " + buildErr;
                    out.segmentsRun = static_cast<u32>(segIdx);
                    return out;
                }
                rebuilt.push_back(a);
            }
        }
        const OrchestratorResult orch = OrchestrateSegments(
            rebuilt, orchRes,
            [&msg, myAlgRank, rankNum, opType, frameTotal = static_cast<u32>(view.frames.size()),
             &beAg, &beRs](const ActionSpec& a, const HcommVerbRes& domainRes) {
                return DriveSegmentViaSink(a, msg, myAlgRank, rankNum, opType, frameTotal,
                                           beAg, beRs, domainRes);
            });
        out.ret = orch.ret;
        out.err = orch.err;
        out.segmentsRun = orch.segmentsRun;
        out.perSegTrace = orch.perSegTrace;
        return out;
    }

    // R15 · ① tier 展开段 × 串行路径：展开段逐段 DriveSegmentViaSink（与编排分叉
    // 同一 driver 单源件——串行下段序全序、批=段，与 R14 组 2 退化语义同构）。
    // 非 tier 走下方既有骨架帧 for 循环（一行不动）。
    if (!tierExpanded.empty()) {
        if (beRs == nullptr) {
            out.ret = HCCL_E_NOT_SUPPORT;
            out.err = "tier segment: AICPU ReduceSlice backend not registered "
                      "(R12 WP-9 expected registered)";
            return out;
        }
        for (size_t segIdx = 0; segIdx < tierExpanded.size(); ++segIdx) {
            const OrchestrateSegResult sr = DriveSegmentViaSink(
                tierExpanded[segIdx], msg, myAlgRank, rankNum, opType,
                static_cast<u32>(view.frames.size()), beAg, beRs, res);
            if (sr.ret != HCCL_SUCCESS) {
                out.ret = sr.ret;
                out.err = "segment " + std::to_string(segIdx) + ": " + sr.err;
                out.segmentsRun = static_cast<u32>(segIdx);
                return out;
            }
            out.perSegTrace.push_back(sr.trace);
            out.segmentsRun = static_cast<u32>(segIdx + 1);
        }
        return out;
    }

    // ②③ 逐段：实例化 → BuildArgs → Emit（sink 逐段新建——READ 线程序按段从
    // threads[0] 起算，存量 KernelRun 逐轮重入同型）
    LoopSplitter splitter;   // BuildArgs 的 goSize 形参（AICPU 路径不消费）
    for (size_t segIdx = 0; segIdx < view.frames.size(); ++segIdx) {
        const BlobFrameView& f = view.frames[segIdx];
        const bool isRs = (f.actionId == static_cast<u64>(ActionKind::ReduceSlice));
        if (isRs && beRs == nullptr) {
            // R12-2⑦：actionId=1 的 Validate 失败显式 NOT_SUPPORT 非静默空跑
            out.ret = HCCL_E_NOT_SUPPORT;
            out.err = "segment " + std::to_string(segIdx) +
                      ": AICPU ReduceSlice backend not registered (R12 WP-9 expected registered)";
            out.segmentsRun = static_cast<u32>(segIdx);
            return out;
        }
        // RS 段实例化（R12-2⑥：对照 CCU 侧 R7 覆写名单 hccl_ccu_v0_prepare.h:787-839
        // ——in/out 合成式/scratch 直替/rankSliceOffset 三分支；AICPU 不消费
        // goSize/chunk 槽但绝不按 AG 语义误读）。
        SegRuntimeParams p;
        if (isRs) {
            std::string rsErr;
            p = InstantiateRsSegment(f, msg, myAlgRank, rankNum, &rsErr);
            if (!rsErr.empty()) {
                out.ret = HCCL_E_PARA;
                out.err = "segment " + std::to_string(segIdx) + ": " + rsErr;
                out.segmentsRun = static_cast<u32>(segIdx);
                return out;
            }
        } else {
            std::string instErr;
            p = InstantiateSegment(
                f, msg, myAlgRank, rankNum, opType, static_cast<u32>(view.frames.size()), &instErr);
            if (!instErr.empty()) {
                out.ret = HCCL_E_PARA;
                out.err = "segment " + std::to_string(segIdx) + ": " + instErr;
                out.segmentsRun = static_cast<u32>(segIdx);
                return out;
            }
        }

        // ActionSpec 重建（BuildArgs 的输入形状——只填其消费字段；RS 段走 RS 形状）
        ActionSpec a;
        a.kind = isRs ? ActionKind::ReduceSlice : ActionKind::CollectAll;
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
        a.transport = TransportPolicy::VIA_CCUBUF;   // AG/RS mesh 非 symmem 缺省（Compile 同判）
        a.sync.preBarrier = true;
        a.sync.postBarrier = true;

        HcommVerbSink segSink(res);
        EmitCtx ctx;
        ctx.sink = &segSink;
        if (isRs) {
            // R12 · WP-9⑥：RS 段走 AicpuReduceSlice::Emit（写侧动词序——
            // backend_aicpu_reduce_slice.h；绝对地址在此合成：sink DoWrite 的 dst
            // 合成需要 remoteCclMem，src=sendBuf 现址 + InOffset 结构量）
            ReduceSliceArgs rsArgs;
            std::string rsBuildErr;
            const HcclResult rsRet = ReduceSlice::BuildArgs(a, rsArgs, rsBuildErr);
            if (rsRet != HCCL_SUCCESS) {
                out.ret = rsRet;
                out.err = "segment " + std::to_string(segIdx) + ": ReduceSlice BuildArgs: " + rsBuildErr;
                out.segmentsRun = static_cast<u32>(segIdx);
                return out;
            }
            beRs->Emit(a, &rsArgs, ctx);
        } else {
            const CollectAllArgs args = CollectAll::BuildArgs(a, splitter);
            beAg->Emit(a, &args, ctx);
        }

        // Emit：真 sink 驱动原语；trace 由本段 sink 调用记录重放（FireVerb 注入 sink
        // 时 EmitCtx.trace 不再由 TraceSink 兜底拼接——同源双收从 sink 侧重建）。
        // ★多段（切轮）语义：READ 计数按段清零——sink 构造逐段新建（存量 KernelRun
        // 逐轮重入同型：每轮 LocalDataCopy→PreSync→reads→PostSync，线程序每轮从
        // threads[0] 重新起算，ins_temp_all_gather_mesh_1D.cc:103/:216 在 RunAllGatherMesh
        // 调用内计数）。复用同一 res（线程/通道是进程级资源）。
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
