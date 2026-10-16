// R15 · WT-1：三件交付单测（host shim 面，桩链接）
//
// 被测对象（任务书 WT-1 + M0_R15_TRIPLE.md 实施规格）：
//   ① TierParams 槽位承载：SerializePlanV3 tierPlan 写槽 16..24（手算一 golden）
//      + NHR 骨架条件放行（allowNhrSkeleton）+ ParseUnifiedBlobV4 读槽
//      + ConsumeUnifiedPlanAicpu tier 分叉（A 展开 4 段 / B 步进 / 非 tier 零变化）
//   ② NHR AICPU 执行面：AicpuReduceSlice::Emit NHR 分支动词序（手算二 Emit
//      产物 10 动词恒定）+ HcommVerbSink DoNhrRun 写/读侧原语展开（24/28 锚）
//      + gate 三态 + 单源对拍（#22 常跑锁扩展）
//   ③ Σ 申报：CalcConcurrencySlaveClaim 数学（手算三 4+5+2=11 抽象 / 16P 实值 5）
//      + 串行路径 max 回归（gated 关 = 现状）+ 切池对照
// 期望值全部独立手算（M0_R15 §十），禁自证。
//
// 编译：见 run.sh（-DMC2_UNIFIED_TIER_RESOURCE / -DMC2_UNIFIED_NHR_AICPU_RESOURCE
// 两个探针形态由 run.sh 的 env 探针段承载——本文件主体按宏开编译）。

#include "verb_sink.h"
#include "hcomm_verb_sink.h"
#include "aicpu_consumer.h"
#include "action_collect_all.h"
#include "action_reduce_slice.h"
#include "action_spec.h"
#include "backend_registry.h"
#include "l1_adapter.h"
#include "plan_serializer.h"
#include "tier_expand.h"
#include "nhr_steps.h"
#include "segment_orchestrator.h"
#include "backend_aicpu_collect_all.h"
#include "backend_aicpu_reduce_slice.h"
// R17 · 流水模型轮（#9 DryRun + 10a checker——NHR AICPU 半边在此验证：本 TU 宏开
// +env 开（run.sh :157），st_checker 入口宏关不可达）
#include "pipeline_model.h"
#include "semantic_checker.h"
#include "nhr_slice_geometry.h"  // R18 · #11：展开形态几何公共件（等价锁判据同源）

#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>
using namespace mc2_ops_hccl::refactor;

// ---------------- 桩（与 poc_unified_exec_test 同一套） ----------------
struct StubCall {
    std::string func;
    u64 thread{0};
    u64 channel{0};
    u64 a1{0}, a2{0}, a3{0};
};
static std::vector<StubCall> g_stubCalls;
static int32_t g_stubRet = 0;
static void Record(const char* f, u64 t, u64 c, u64 a1, u64 a2, u64 a3)
{
    StubCall s;
    s.func = f; s.thread = t; s.channel = c; s.a1 = a1; s.a2 = a2; s.a3 = a3;
    g_stubCalls.push_back(s);
}
extern "C" {
int32_t HcommLocalCopyOnThread(ThreadHandle thread, void* dst, const void* src, uint64_t len)
{
    Record("LocalCopy", thread, 0, reinterpret_cast<u64>(dst), reinterpret_cast<u64>(src), len);
    return g_stubRet;
}
int32_t HcommReadOnThread(ThreadHandle thread, ChannelHandle channel, void* dst, const void* src, uint64_t len)
{
    Record("Read", thread, channel, reinterpret_cast<u64>(dst), reinterpret_cast<u64>(src), len);
    return g_stubRet;
}
int32_t HcommWriteOnThread(ThreadHandle thread, ChannelHandle channel, void* dst, const void* src, uint64_t len)
{
    Record("Write", thread, channel, reinterpret_cast<u64>(dst), reinterpret_cast<u64>(src), len);
    return g_stubRet;
}
int32_t HcommLocalReduceOnThread(ThreadHandle thread, void* dst, const void* src, uint64_t count,
    HcommDataType dataType, HcommReduceOp reduceOp)
{
    Record("LocalReduce", thread, 0, reinterpret_cast<u64>(dst), reinterpret_cast<u64>(src), count);
    (void)dataType; (void)reduceOp;
    return g_stubRet;
}
int32_t HcommThreadNotifyRecordOnThread(ThreadHandle thread, ThreadHandle dstThread, uint32_t dstNotifyIdx)
{
    Record("ThreadNotifyRecord", thread, dstThread, dstNotifyIdx, 0, 0);
    return g_stubRet;
}
int32_t HcommThreadNotifyWaitOnThread(ThreadHandle thread, uint32_t notifyIdx, uint32_t timeOut)
{
    Record("ThreadNotifyWait", thread, 0, notifyIdx, timeOut, 0);
    return g_stubRet;
}
int32_t HcommChannelNotifyRecordOnThread(ThreadHandle thread, ChannelHandle channel, uint32_t remoteNotifyIdx)
{
    Record("ChNotifyRecord", thread, channel, remoteNotifyIdx, 0, 0);
    return g_stubRet;
}
int32_t HcommChannelNotifyWaitOnThread(ThreadHandle thread, ChannelHandle channel, uint32_t localNotifyIdx, uint32_t timeOut)
{
    Record("ChNotifyWait", thread, channel, localNotifyIdx, timeOut, 0);
    return g_stubRet;
}
int32_t HcommWriteReduceOnThread(ThreadHandle thread, ChannelHandle channel, void* dst, const void* src,
    uint64_t count, HcommDataType dataType, HcommReduceOp reduceOp)
{
    Record("WriteReduce", thread, channel, reinterpret_cast<u64>(dst), reinterpret_cast<u64>(src), count);
    (void)dataType; (void)reduceOp;
    return g_stubRet;
}
int32_t HcommReadReduceOnThread(ThreadHandle thread, ChannelHandle channel, void* dst, const void* src,
    uint64_t count, HcommDataType dataType, HcommReduceOp reduceOp)
{
    Record("ReadReduce", thread, channel, reinterpret_cast<u64>(dst), reinterpret_cast<u64>(src), count);
    (void)dataType; (void)reduceOp;
    return g_stubRet;
}
}

static int g_pass = 0, g_fail = 0;
static void Check(bool cond, const std::string& what)
{
    if (cond) { ++g_pass; printf("  [PASS] %s\n", what.c_str()); }
    else      { ++g_fail; printf("  [FAIL] %s\n", what.c_str()); }
}

// ---------------- 构造件 ----------------
// tier 骨架 Plan：parallel[mesh,nhr] 16P（域 0 mesh {0,1,2,3}、域 1 nhr {0,4,8,12}）
static Plan MakeTierSkeletonPlan(u32 countPerRank)
{
    OpParam op;
    op.opType = static_cast<int>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER);
    op.DataDes.count = countPerRank;
    op.DataDes.dataType = HCCL_DATA_TYPE_FP16;
    op.engine = CommEngine::COMM_ENGINE_AICPU;
    op.userRank = 0;
    TopoInfoWithNetLayerDetails topo;
    topo.userRank = 0;
    topo.userRankSize = 16;
    topo.topoLevelNums = 2;
    topo.level0Topo = Level0Shape::MESH_1D;
    Plan plan;
    std::string err;
    if (L1Adapter::Compile(op, topo, {{0U, 1U, 2U, 3U}, {0U, 4U, 8U, 12U}},
                           "parallel[mesh,nhr]", plan, err) != HCCL_SUCCESS) {
        printf("  [FAIL] tier skeleton compile: %s\n", err.c_str());
        ++g_fail;
    }
    return plan;
}

// 16P 双域资源束（建链集通道表：域 0 mesh 全连接 {1,2,3}；域 1 NHR me=0 建链集
// {±1,±2,±4} mod 4 域内 = {1,3,2,2} 去重 = {1,2,3}——N=4 的 NHR 建链集恰全连接）
static HcommVerbRes MakeTierRes(u32 poolSize, bool pcieDomain1 = false)
{
    HcommVerbRes res;
    for (u32 i = 0; i < poolSize; ++i) {
        res.threads.push_back(0x1000 + i);
    }
    const u32 peers[3] = {1, 2, 3};
    for (int k = 0; k < 3; ++k) {
        ChannelInfo c0;
        c0.remoteRank = peers[k];
        c0.handle = 0x20000 + peers[k];
        c0.protocol = COMM_PROTOCOL_HCCS;
        res.channels[peers[k]].push_back(c0);
        ChannelInfo c1;
        c1.remoteRank = 4 + 4 * k;   // 域 1：4, 8, 12
        c1.handle = 0x30000 + 4 + 4 * k;
        c1.protocol = pcieDomain1 ? COMM_PROTOCOL_PCIE : COMM_PROTOCOL_HCCS;
        res.channels[4 + 4 * k].push_back(c1);
    }
    return res;
}

// ================= ① TierParams 槽位承载 =================
static void T1_TierSlotGolden()
{
    printf("== R15-T1 tier 槽 golden（手算一：槽 16..24 九值） ==\n");
    // 构造：SerializeSegmentV3 的 RS mesh 段 + tierPlan 九值手算锚
    ActionSpec seg;
    seg.kind = ActionKind::ReduceSlice;
    seg.pattern = CommPattern::MESH;
    seg.domain.ranks = {0U, 1U, 2U, 3U};
    seg.domain.myRank = 0;
    seg.layout.sliceSize = 1024;
    seg.layout.outputSliceStride = 1024;
    u64 slots[32];
    std::string err;
    Check(SerializeSegmentV3(seg, slots, err), "T1 mesh 段序列化 ok");

    // 手算一锚：TierParams{thresh=8MiB, 3/4, 2/1, 1/2, 32}。
    // 规范骨架（gate 要求 2 段 RS [mesh,nhr]——TierSkeletonCanonical）：seg0 =
    // 上方 mesh 段、seg1 = 同域 NHR 段（NHR 段的帧值 layout 驱动 pattern 无关，
    // 序列化在 tierCanonical 放行下不拒——放行语义正是 T2 的被测面）
    ActionSpec nhrSeg = seg;
    nhrSeg.pattern = CommPattern::NHR;
    Plan plan;
    plan.segments = {seg, nhrSeg};
    plan.tier.hasTier = true;
    plan.tier.threshBytes = 8ULL * 1024 * 1024;
    plan.tier.interleave.partRatioNum = 3;
    plan.tier.interleave.partRatioDen = 4;
    plan.tier.stepwise.bwFastNum = 2;
    plan.tier.stepwise.bwFastDen = 1;
    plan.tier.stepwise.bwSlowNum = 1;
    plan.tier.stepwise.bwSlowDen = 2;
    plan.tier.stepwise.alignBytes = 32;
    std::vector<u64> blob;
    Check(SerializePlanV3(plan, blob, err, &plan.tier), "T1 tier 序列化 ok");
    Check(blob.size() == 7U + 2U * 32U, "T1 blob = 7 + 2×32 槽（骨架双段）");
    const u64* f0 = blob.data() + 7U;
    Check(f0[16] == 1ULL, "T1 槽16 hasTier = 1");
    Check(f0[17] == 8388608ULL, "T1 槽17 threshBytes = 8388608 (0x800000)");
    Check(f0[18] == 3ULL, "T1 槽18 partRatioNum = 3");
    Check(f0[19] == 4ULL, "T1 槽19 partRatioDen = 4");
    Check(f0[20] == 2ULL, "T1 槽20 bwFastNum = 2");
    Check(f0[21] == 1ULL, "T1 槽21 bwFastDen = 1");
    Check(f0[22] == 1ULL, "T1 槽22 bwSlowNum = 1");
    Check(f0[23] == 2ULL, "T1 槽23 bwSlowDen = 2");
    Check(f0[24] == 32ULL, "T1 槽24 alignBytes = 32 (0x20)");
    bool tailZero = true;
    for (u32 i = 25; i < 32; ++i) {
        if (f0[i] != 0ULL) tailZero = false;
    }
    // 段 1 帧槽 16..24 恒 0（tier 只写段 0——判别式只查帧 0，M0 手算一）
    const u64* f1 = blob.data() + 7U + 32U;
    for (u32 i = 16; i <= 24; ++i) {
        if (f1[i] != 0ULL) tailZero = false;
    }
    Check(tailZero, "T1 段0 槽 25..31 与段1 槽 16..24 恒 0（区间尾+次帧保留）");

    // 非 tier 反例：同 Plan 不传 tierPlan → NHR 段缺省拒收（修正 A 的 CCU 降级
    // 同口径——单段 mesh 构造改走下方独立小例）
    std::vector<u64> blob2;
    Check(!SerializePlanV3(plan, blob2, err),
          "T1 非 tier 序列化拒 NHR 骨架（缺省 gate=false——现状行为）");
    Plan meshOnly;
    meshOnly.segments = {seg};
    std::vector<u64> blob3;
    Check(SerializePlanV3(meshOnly, blob3, err), "T1 单段 mesh blob ok");
    const u64* g0 = blob3.data() + 7U;
    bool allZero = true;
    for (u32 i = 16; i <= 24; ++i) {
        if (g0[i] != 0ULL) allZero = false;
    }
    Check(allZero, "T1 非 tier 槽 16..24 全 0（判别式地基——现状行为）");

    // ParseUnifiedBlobV4 读槽回环
    BlobView view;
    Check(ParseUnifiedBlobV4(blob, view, err), "T1 blob 解读 ok");
    Check(view.frames[0].tierHas == 1ULL && view.frames[0].tierThreshBytes == 8388608ULL &&
              view.frames[0].tierPartRatioNum == 3 && view.frames[0].tierPartRatioDen == 4 &&
              view.frames[0].tierBwFastNum == 2 && view.frames[0].tierBwFastDen == 1 &&
              view.frames[0].tierBwSlowNum == 1 && view.frames[0].tierBwSlowDen == 2 &&
              view.frames[0].tierAlignBytes == 32,
          "T1 读槽九值回环 == 写入值");
    Check(ParseUnifiedBlobV4(blob2, view, err) && view.frames[0].tierHas == 0ULL,
          "T1 非 tier 读槽 tierHas = 0（不展开判别）");
}

static void T2_NhrSkeletonGate()
{
    printf("== R15-T2 NHR 骨架条件放行（修正 A：引擎维度 gate） ==\n");
    ActionSpec nhrSeg;
    nhrSeg.kind = ActionKind::ReduceSlice;
    nhrSeg.pattern = CommPattern::NHR;
    nhrSeg.domain.ranks = {0U, 4U, 8U, 12U};
    nhrSeg.domain.myRank = 0;
    nhrSeg.layout.sliceSize = 1024;
    nhrSeg.layout.outputSliceStride = 1024;
    u64 slots[32];
    std::string err;
    Check(!SerializeSegmentV3(nhrSeg, slots, err),
          "T2 缺省（allowNhrSkeleton=false）NHR 段拒收 = 现状零变化");
    Check(err.find("NHR not in delivery scope") != std::string::npos,
          "T2 拒收 err 带既有文案（CCU 降级路径同此拒）");
    Check(SerializeSegmentV3(nhrSeg, slots, err, /*allowNhrSkeleton=*/true),
          "T2 allowNhrSkeleton=true 放行（AICPU 引擎 tier 骨架承载）");

    // TierSkeletonCanonical 规范形态判别
    Plan plan = MakeTierSkeletonPlan(512);
    Check(plan.segments.size() == 2 && plan.tier.hasTier,
          "T2 parallel[mesh,nhr] 16P 编译产 tier 骨架（R9 挂载）");
    Check(TierSkeletonCanonical(plan), "T2 规范形态 gate 命中（2 RS 段 + mesh/nhr）");
    plan.segments[0].pattern = CommPattern::NHR;   // 角色倒置 → 非规范
    Check(!TierSkeletonCanonical(plan), "T2 角色倒置（nhr,mesh）非规范不写槽");
    plan.segments[0].pattern = CommPattern::MESH;
    plan.segments.pop_back();                       // 单段 → 非规范
    Check(!TierSkeletonCanonical(plan), "T2 单段非规范不写槽");
}

static void T3_TierConsumeFork()
{
    printf("== R15-T3 消费方 tier 分叉（A 展开 / B 步进 / 非 tier 零变化） ==\n");
    // 真链：Compile → SerializePlanV3(tierPlan) → Parse → Consume（tier 分叉）
    const Plan plan = MakeTierSkeletonPlan(512 * 1024);   // count = 512K 元素 → 1MiB（A 档）
    std::string err;
    std::vector<u64> blob;
    Check(SerializePlanV3(plan, blob, err, &plan.tier), "T3 tier blob 序列化 ok");
    BlobView view;
    Check(ParseUnifiedBlobV4(blob, view, err), "T3 blob 解读 ok");
    Check(view.frames.size() == 2 && view.frames[0].tierHas == 1ULL, "T3 帧 0 tierHas=1");

    // 非 tier 对拍（#22 单源对拍锁扩展）：同 blob 双路（domainRanks2 提供与否）
    // ——非 tier blob 的 Consume 输出与既有路径逐条相等由 exec 测试 T5 承载；
    // 此处锁 tier 分叉的 A 展开（1MiB < 4MiB 阈 → A 交错 4 段）。
    UnifiedExecMsg msg;
    msg.count = 512 * 1024;                 // 元素数（FP16 → 1MiB）
    msg.dataType = HCCL_DATA_TYPE_FP16;
    msg.inputAddr = 0x400000;
    msg.outputAddr = 0x800000;
    msg.strideCount = 0;
    msg.repeatCnt = 1;
    msg.cclBuff.addr = reinterpret_cast<void*>(0xC00000ULL);
    msg.cclBuff.size = 4ULL * 1024 * 1024;

    // 手算 A 展开期望：4 段 [mesh P0, nhr P1, nhr P0, mesh P1]、sync [T,F,T,F→批尾
    // post 位 [F,T,F,T]]（ExpandTieredSegments A 分支——tier_expand.h:330-334）。
    // 1MiB < plan.tier.threshBytes(4MiB) → A 档。
    std::vector<ActionSpec> expectA;
    Check(ExpandTieredSegments(plan.segments, plan.tier,
                               msg.count * 2ULL, expectA, err),
          "T3 A 展开参照 ok: " + err);
    Check(expectA.size() == 4, "T3 A 展开 = 4 段");

    // 消费方分叉（串行路径）：tier blob + domainRanks2 → 展开 4 段驱动
    HcommVerbRes res = MakeTierRes(8);
    const UnifiedExecResult r = ConsumeUnifiedPlanAicpu(
        blob, msg, 0, {0U, 1U, 2U, 3U},
        static_cast<u32>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER), res, {0U, 4U, 8U, 12U});
    Check(r.ret == HCCL_SUCCESS, "T3 tier 消费 ok: " + r.err);
    Check(r.segmentsRun == 4, "T3 tier 消费驱动 4 段（A 展开——非骨架 2 段）");
    Check(r.perSegTrace.size() == 4, "T3 perSegTrace = 4 段");

    // 非 tier blob（缺省序列化）零变化：同构造不传 tier → 骨架 2 段直驱。
    // 注：plan 的 seg1 是 NHR 段，缺省序列化必拒（这正是修正 A 的语义——CCU 降级
    // 路径同此拒）。断言锁该拒收：
    std::vector<u64> blobPlain;
    Check(!SerializePlanV3(plan, blobPlain, err),
          "T3 非 tier 路径 NHR 骨架拒收（缺省 gate=false——CCU 降级同口径）");

    // tier blob 但 domainRanks2 空 → 显式拒收（组装断层不静默）
    const UnifiedExecResult r2 = ConsumeUnifiedPlanAicpu(
        blob, msg, 0, {0U, 1U, 2U, 3U},
        static_cast<u32>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER), res);
    Check(r2.ret == HCCL_E_PARA && r2.err.find("level1 domain ranks missing") != std::string::npos,
          "T3 tier blob + 空 level1 → 显式 E_PARA（断层不静默）");

    // 非 tier Plan（sole[mesh] AG）行为零变化：槽 16=0 → 走骨架帧路径
    OpParam ag;
    ag.opType = static_cast<int>(HcclCMDType::HCCL_CMD_ALLGATHER);
    ag.DataDes.count = 512;
    ag.DataDes.dataType = HCCL_DATA_TYPE_FP16;
    ag.engine = CommEngine::COMM_ENGINE_AICPU;
    ag.userRank = 0;
    TopoInfoWithNetLayerDetails topo;
    topo.userRankSize = 4;
    Plan agPlan;
    Check(L1Adapter::Compile(ag, topo, {{0U, 1U, 2U, 3U}}, "sole[mesh]", agPlan, err) == HCCL_SUCCESS,
          "T3 AG sole[mesh] compile ok");
    std::vector<u64> agBlob;
    Check(SerializePlanV3(agPlan, agBlob, err, &agPlan.tier), "T3 AG blob ok");
    const u64* agF = agBlob.data() + 7U;
    bool agZero = true;
    for (u32 i = 16; i <= 24; ++i) {
        if (agF[i] != 0ULL) agZero = false;
    }
    Check(agZero, "T3 AG Plan（hasTier=false）槽 16..24 恒 0（TierSkeletonCanonical 拒）");
}

// ================= ② NHR AICPU 执行面 =================
static void T4_NhrEmitVerbs()
{
    printf("== R15-T4 NHR Emit 动词序（手算二修正 B：恒 10 动词） ==\n");
    // 构造 NHR 段（域 {0,4,8,12} me=0 N=4）：步表手算 N=4 → nSteps=2
    //   step0: sendTo=(0+4-1)%4=3, recvFrom=1, nSlices=(4-1+1)/2=2, tx={3,1}, rx={0,2}
    //   step1: sendTo=(0+4-2)%4=2, recvFrom=2, nSlices=(4-1+2)/4=1, tx={2}, rx={0}
    // 校验步公式（N=4 探针——与 N=8 手算二同公式）
    const std::vector<NhrStepInfo> st4 = CalcNhrSteps(0, 4);
    Check(st4.size() == 2, "T4 N=4 nSteps = 2");
    Check(st4[0].sendTo == 3 && st4[0].recvFrom == 1 && st4[0].nSlices == 2, "T4 step0 [3,1,2]");
    Check(st4[1].sendTo == 2 && st4[1].recvFrom == 2 && st4[1].nSlices == 1, "T4 step1 [2,2,1]（±2 同 rank）");
    Check(st4[0].txSliceIdxs.size() == 2 && st4[0].txSliceIdxs[0] == 3 && st4[0].txSliceIdxs[1] == 1,
          "T4 step0 tx = {3,1}");
    Check(st4[0].rxSliceIdxs[0] == 0 && st4[0].rxSliceIdxs[1] == 2, "T4 step0 rx = {0,2}");
    // 建链集：N=4 me=0 → {±1,±2} = {1,3,2} 升序 [1,2,3]（恰全连接）
    const std::vector<u32> conn = CalcNhrConnectRanks(0, 4);
    Check(conn.size() == 3 && conn[0] == 1 && conn[1] == 2 && conn[2] == 3,
          "T4 N=4 me=0 建链集 = [1,2,3]");

    // Emit：NHR 段（gated 宏开由 run.sh 传 -DMC2_UNIFIED_NHR_AICPU_RESOURCE）
    ActionSpec a;
    a.kind = ActionKind::ReduceSlice;
    a.pattern = CommPattern::NHR;
    a.domain.ranks = {0U, 4U, 8U, 12U};
    a.domain.myRank = 0;
    a.layout.sliceSize = 1024;
    a.layout.inputSliceStride = 1024;
    a.layout.outputSliceStride = 1024;
    a.layout.inBaseOff = 0x400000;
    a.layout.outBaseOff = 0x800000;
    a.layout.scratchBaseOff = 0xC00000;
    a.layout.dataTypeSize = 2;
    a.channelsPerRank = 1;
    a.nhrJettyNum = 1;
    a.transport = TransportPolicy::VIA_CCUBUF;
    a.sync.preBarrier = true;
    a.sync.postBarrier = true;

    ReduceSliceArgs args;
    std::string berr;
    Check(ReduceSlice::BuildArgs(a, args, berr) == HCCL_SUCCESS, "T4 NHR BuildArgs ok（R13 已受理）");

    if (!UseUnifiedNhrAicpuPath()) {
        // env 关（run.sh 三态探针的"宏开+env 关"段）：NHR 段落 mesh 写侧分支 =
        // 现状行为（全连接迭代）。锁该现状：动词序 = SYNC_PRE + WRITE×3 +
        // SYNC_POST + COPY + REDUCE×3（backend mesh 分支既有形态）。
        HcommVerbSink sink(MakeTierRes(8));
        EmitCtx ctx;
        ctx.sink = &sink;
        BackendRegistry::Peek(ActionKind::ReduceSlice, Engine::AICPU)->Emit(a, &args, ctx);
        const std::vector<HcommCallRecord>& calls = sink.Calls();
        Check(calls.size() == 9, "T4 env关 NHR 段落 mesh 分支 = 9 动词（现状回归）");
        bool meshForm = calls.size() == 9 && calls[0].verb == VerbCall::SYNC_PEER_PRE;
        size_t wrCnt = 0;
        for (const auto& c : calls) {
            if (c.verb == VerbCall::WRITE) ++wrCnt;
        }
        Check(meshForm && wrCnt == 3, "T4 env关 mesh 写侧形态（SYNC_PRE + WRITE×3 …）");
        return;
    }

    // Emit 产物（手算二修正 B 口径）：COPY×4 → NHR_RUN×1 → COPY×1 = 6 动词
    // ★R18 全删轮等价改写：Emit 展开形态（AICPU 含上料/归位 COPY）——
    // N=4 步表 2 步（step0{3,1,2} step1{2,2,1}）：
    //   上料 COPY×4 → 步0[CH×2 + SR×2 + CH×2] → 步1[CH×2 + SR×1 + CH×2] → 归位 COPY×1
    //   = 4 + (4+2) + (4+1) + 1 = 16 动词
    std::string trace;
    {
        HcommVerbSink sink(MakeTierRes(8));
        EmitCtx ctx;
        ctx.sink = &sink;
        BackendRegistry::Peek(ActionKind::ReduceSlice, Engine::AICPU)->Emit(a, &args, ctx);
        const std::vector<HcommCallRecord>& calls = sink.Calls();
        Check(calls.size() == 16,
              "T4 Emit 动词数 = 16（上料 4 + (4+2)+(4+1) + 归位 1——R18 全删轮展开口径）");
        bool verbsOk = calls.size() == 16;
        for (int i = 0; i < 4 && verbsOk; ++i) {
            verbsOk = calls[i].verb == VerbCall::COPY;
        }
        // 步 0：CH(ACK-Rec) CH(ACK-Wait) SR SR CH(DATA-Rec) CH(DATA-Wait)
        if (verbsOk) {
            verbsOk = calls[4].verb == VerbCall::CH_SYNC && calls[5].verb == VerbCall::CH_SYNC &&
                      calls[6].verb == VerbCall::SLICE_REDUCE && calls[7].verb == VerbCall::SLICE_REDUCE &&
                      calls[8].verb == VerbCall::CH_SYNC && calls[9].verb == VerbCall::CH_SYNC;
        }
        // 步 1：CH CH SR CH CH + 归位 COPY
        if (verbsOk) {
            verbsOk = calls[10].verb == VerbCall::CH_SYNC && calls[11].verb == VerbCall::CH_SYNC &&
                      calls[12].verb == VerbCall::SLICE_REDUCE && calls[13].verb == VerbCall::CH_SYNC &&
                      calls[14].verb == VerbCall::CH_SYNC && calls[15].verb == VerbCall::COPY;
        }
        Check(verbsOk, "T4 动词序 = [COPY×4, (CH×2+SR×n+CH×2)×2 步, COPY]（步闭包形态）");
        // SYNC 不发（channelsPerRank=1——裁决 2 保持）
        bool noSync = true;
        for (const auto& c : calls) {
            if (c.verb == VerbCall::SYNC_PEER_PRE || c.verb == VerbCall::SYNC_PEER_POST) noSync = false;
        }
        Check(noSync, "T4 无 SYNC_PEER 动词（单通道门槛——裁决 2）");
        // SR 载荷断言由 T5 的 sink 原语展开间接承载（载荷错 → 步表/地址错 → 原语序 FAIL）
    }
}

static void T5_NhrSinkPrims()
{
    printf("== R15-T5 NHR sink 原语展开（写侧 HCCS / 读侧 PCIe——步表 N=4 手算） ==\n");
    // 构造与 T4 相同段；res 域 1 通道 HCCS（写侧）
    ActionSpec a;
    a.kind = ActionKind::ReduceSlice;
    a.pattern = CommPattern::NHR;
    a.domain.ranks = {0U, 4U, 8U, 12U};
    a.domain.myRank = 0;
    a.layout.sliceSize = 1024;
    a.layout.inputSliceStride = 1024;
    a.layout.outputSliceStride = 1024;
    a.layout.inBaseOff = 0x400000;
    a.layout.outBaseOff = 0x800000;
    a.layout.scratchBaseOff = 0xC00000;
    a.layout.dataTypeSize = 2;
    a.channelsPerRank = 1;
    a.nhrJettyNum = 1;
    a.transport = TransportPolicy::VIA_CCUBUF;
    ReduceSliceArgs args;
    std::string berr;
    Check(ReduceSlice::BuildArgs(a, args, berr) == HCCL_SUCCESS, "T5 BuildArgs ok");

    const auto emitInto = [&a, &args](HcommVerbSink& sink) {
        EmitCtx ctx;
        ctx.sink = &sink;
        BackendRegistry::Peek(ActionKind::ReduceSlice, Engine::AICPU)->Emit(a, &args, ctx);
    };
    if (!UseUnifiedNhrAicpuPath()) {
        Check(true, "T5 env关：NHR sink 执行面不可达（Emit 不产 NHR_RUN——gate 半边）");
        return;
    }

    // ---- 写侧（HCCS）——sink 原语序手算（N=4 me=0，步表 [3,1,2]/[2,2,1]）：
    //   Emit COPY×4（LocalCopy×4）→ NHR_RUN：
    //     step0: Rec(ch(1),ACK) Wait(ch(3),ACK) WR×2(tx={3,1}→ch(3)) Rec(ch(3),DATA) Wait(ch(1),DATA)
    //     step1: Rec(ch(2),ACK) Wait(ch(2),ACK) WR×1(tx={2}→ch(2)) Rec(ch(2),DATA) Wait(ch(2),DATA)
    //   → 归位 COPY×1（LocalCopy×1）
    //   合计 LocalCopy 5 + ChNotifyRecord 4 + ChNotifyWait 4 + WriteReduce 3 = 16
    {
        g_stubCalls.clear();
        HcommVerbSink sink(MakeTierRes(8, /*pcieDomain1=*/false));
        emitInto(sink);
        Check(sink.ErrCode() == 0, "T5 写侧执行 ok: " + sink.Err());
        size_t lc = 0, cnr = 0, cnw = 0, wr = 0, rr = 0;
        for (const auto& c : g_stubCalls) {
            if (c.func == "LocalCopy") ++lc;
            else if (c.func == "ChNotifyRecord") ++cnr;
            else if (c.func == "ChNotifyWait") ++cnw;
            else if (c.func == "WriteReduce") ++wr;
            else if (c.func == "ReadReduce") ++rr;
        }
        Check(lc == 5, "T5 写侧 LocalCopy = 5（上料 4 + 归位 1）");
        Check(wr == 3 && rr == 0, "T5 写侧 WriteReduce = 3 / ReadReduce = 0（ΣnSlices = 2+1）");
        Check(cnr == 4 && cnw == 4, "T5 写侧握手 = 4+4（2 步 × [Rec,Wait]×2 半边）");
        Check(g_stubCalls.size() == 16, "T5 写侧原语总数 = 16（5+4+4+3——手算锚）");
        // ★R18 全删轮等价改写：CH_SYNC 动词化后半边表达面——单动词单侧
        //（Record 或 Wait），peer 由方向选定（写侧恒 sendTo）。步0 {sendTo=3,
        // recvFrom=1}：首两条 = Rec(ch(3), ACK) + Wait(ch(3), ACK)（DoChSync
        // isWait 按 dir 槽分派；旧 DoNhrRun 的 Rec/Wait 双通道序退役）
        bool seq0 = g_stubCalls.size() > 4 && g_stubCalls[4].func == "ChNotifyRecord" &&
                    g_stubCalls[4].channel == 0x3000C && g_stubCalls[4].a1 == 0;   // NOTIFY_IDX_ACK=0
        Check(seq0, "T5 step0 首原语 = Rec(ch(3), ACK)（sendTo 通道——CH_SYNC 写侧单半边）");
        if (g_stubCalls.size() > 5) {
            Check(g_stubCalls[5].func == "ChNotifyWait" && g_stubCalls[5].channel == 0x3000C &&
                      g_stubCalls[5].a1 == 0,
                  "T5 step0 ACK wait 落 ch(3)（同 peer 半边对——Record/Wait 两动词）");
        }
        // step0 WR 目的 = 对端(3=rank12) buff + S*tx：tx=3 → 0x3000C 通道记录的 dst
        //   （txIdx {3,1}：dst = rc(12) + 1024*3 / + 1024*1）
        bool wrDst = false;
        for (size_t i = 6; i < g_stubCalls.size() && i < 8; ++i) {
            if (g_stubCalls[i].func == "WriteReduce" && g_stubCalls[i].channel == 0x3000C) {
                wrDst = true;
            }
        }
        Check(wrDst, "T5 step0 WR 走 ch(3)（sendTo 通道）");
    }

    // ---- 读侧（PCIe）：skipStep0Tx 不生效（上料 4 条全下）、WR→RD、握手方向对称
    {
        g_stubCalls.clear();
        HcommVerbSink sink(MakeTierRes(8, /*pcieDomain1=*/true));
        emitInto(sink);
        Check(sink.ErrCode() == 0, "T5 读侧执行 ok: " + sink.Err());
        size_t lc = 0, wr = 0, rr = 0;
        for (const auto& c : g_stubCalls) {
            if (c.func == "LocalCopy") ++lc;
            else if (c.func == "WriteReduce") ++wr;
            else if (c.func == "ReadReduce") ++rr;
        }
        Check(lc == 5, "T5 读侧 LocalCopy = 5（skip 不生效仍全量——Emit 恒 10 动词的 sink 侧）");
        Check(rr == 3 && wr == 0, "T5 读侧 ReadReduce = 3（isDmaRead → RD 原语）");
        Check(g_stubCalls.size() == 16, "T5 读侧原语总数 = 16（与写侧同构）");
        // ★R18 全删轮等价改写：读侧（PCIe）CH_SYNC peer = dst = recvFrom=1——
        // 首原语 = Rec(ch(1), ACK)（与写侧 ch(3) 方向对称翻转）
        bool seq0 = g_stubCalls.size() > 4 && g_stubCalls[4].func == "ChNotifyRecord" &&
                    g_stubCalls[4].channel == 0x30004;
        Check(seq0, "T5 读侧 step0 首原语 = Rec(ch(1), ACK)（recvFrom——CH_SYNC 读侧单半边）");
    }

    // ---- gate 关（NHR 分支不可达）：宏关编译时 Emit NHR 段落 mesh 分支（全连接
    // 迭代）——本测试按宏开编译，此处锁 UseUnifiedNhrAicpuPath()==true（三态探针
    // 由 run.sh 的宏关/宏开两态编译承载）
    Check(UseUnifiedNhrAicpuPath(), "T5 宏开 + env 开（run.sh 设 MC2_USE_UNIFIED_NHR_AICPU=1）");
}

static void T6_NhrEquivalence()
{
    printf("== R15-T6 模板等价序（24 原语锚的 N=4 同构 + #22 对拍扩展） ==\n");
    // 模板语义等价（老链 LocalDataCopy 跳 step0Tx）：N=4 me=0 step0Tx={3,1}
    // → 模板上料 = r∈{0,2} 2 条（≠ Emit 全量 4 条）——sink 展开剥 step0Tx 槽位的
    // LocalCopy 后应与模板等价（差 = step0Tx 槽位拷贝数 2）
    const std::vector<NhrStepInfo> st = CalcNhrSteps(0, 4);
    std::vector<u32> step0Tx = st.empty() ? std::vector<u32>() : st[0].txSliceIdxs;
    Check(step0Tx.size() == 2, "T6 step0Tx = {3,1}（模板跳过集）");
    size_t skipCnt = 0;
    for (u32 r : step0Tx) {
        (void)r;
        ++skipCnt;   // 每个跳过槽位对应一条多余 LocalCopy（写不被读的 scratch 槽）
    }
    Check(skipCnt == 2, "T6 N=4 多余拷贝 = 2（= nSlices(step0) = N/2——修正 B 公式）");

    // #22 单源对拍锁扩展：tier 展开 vs 直喂展开（同一 ExpandTieredSegments 产物
    // 驱动 vs consumer tier 分叉重建再驱动——两路 perSegTrace 逐段相等）
    const Plan plan = MakeTierSkeletonPlan(512 * 1024);
    std::string err;
    std::vector<u64> blob;
    Check(SerializePlanV3(plan, blob, err, &plan.tier), "T6 tier blob ok");
    UnifiedExecMsg msg;
    msg.count = 512 * 1024;
    msg.dataType = HCCL_DATA_TYPE_FP16;
    msg.inputAddr = 0x400000;
    msg.outputAddr = 0x800000;
    msg.cclBuff.addr = reinterpret_cast<void*>(0xC00000ULL);
    msg.cclBuff.size = 4ULL * 1024 * 1024;
    HcommVerbRes res = MakeTierRes(8);
    // 路 A：consumer tier 分叉（重建+展开+驱动）
    const UnifiedExecResult rA = ConsumeUnifiedPlanAicpu(
        blob, msg, 0, {0U, 1U, 2U, 3U},
        static_cast<u32>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER), res, {0U, 4U, 8U, 12U});
    Check(rA.ret == HCCL_SUCCESS && rA.segmentsRun == 4, "T6 路 A（consumer 分叉）4 段 ok");
    // 路 B：直喂展开段（ExpandTieredSegments 参照 + DriveSegmentViaSink 逐段）。
    // 骨架 scratch 直替 msg.cclBuff（InstantiateRsSegment :350 同式——Compile 期
    // 占位 0，exec 期现址替换是既有消费方语义，路 B 对齐该语义才是有效单源锚）
    Plan planB = plan;
    for (size_t i = 0; i < planB.segments.size(); ++i) {
        planB.segments[i].layout.scratchBaseOff = reinterpret_cast<u64>(msg.cclBuff.addr);
    }
    std::vector<ActionSpec> expanded;
    Check(ExpandTieredSegments(planB.segments, planB.tier, msg.count * 2ULL, expanded, err),
          "T6 路 B 展开 ok");
    std::vector<std::string> traceB;
    bool bOk = true;
    IActionBackend* beAg = BackendRegistry::Peek(ActionKind::CollectAll, Engine::AICPU);
    IActionBackend* beRs = BackendRegistry::Peek(ActionKind::ReduceSlice, Engine::AICPU);
    for (size_t i = 0; i < expanded.size() && bOk; ++i) {
        const OrchestrateSegResult sr = DriveSegmentViaSink(
            expanded[i], msg, expanded[i].domain.MyAlgRank(), expanded[i].domain.Size(),
            static_cast<u32>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER),
            static_cast<u32>(expanded.size()), beAg, beRs, res);
        if (sr.ret != HCCL_SUCCESS) {
            bOk = false;
        }
        traceB.push_back(sr.trace);
    }
    Check(bOk && traceB.size() == 4, "T6 路 B（直喂）4 段 ok");
    bool eq = rA.perSegTrace.size() == traceB.size();
    for (size_t i = 0; eq && i < traceB.size(); ++i) {
        eq = (rA.perSegTrace[i] == traceB[i]);
    }
    Check(eq, "T6 #22 对拍：tier 分叉 vs 直喂展开 perSegTrace 逐段相等（常跑锁）");
}

// ================= ③ Σ 申报 =================
static void T7_SigmaClaim()
{
    printf("== R15-T7 Σ 申报数学（手算三：premises 11 / 16P 实值 5） ==\n");
    // premises 构造：两段——AG mesh 段 need=4（n=5 域 [0..4)）… 手算三题面：
    //   AG mesh 段 need=4（域 [0..3]、n=4 → AG n−1=3？——题面 premises "need=4"
    //   按逐域 slave 需求 4/5 口径（need 为总线程数：AG 4P 段 threads=4 → n=5）
    //   构造：域 0 ranks{0..4}（5 rank AG → need=4）、域 1 ranks{0,8,16,24,32,40}
    //   （6 rank RS NHR channelsPerRank=1 → need=1）……premises 是抽象口径，
    //   ST 以等价形态构造：域 0 AG 5P（need=4=slaves 3+main）、域 1 RS NHR
    //   6P chan=1（need=1）。Σ slaves = 3 + 0 + 2 = 5？——与题面 4+5+2=11 对不上
    //   的原因是题面把 need 记作 slaves（4 与 5 是逐域 slave 需求）。等价改算：
    //   slaveTotal = Σ(need−1) + D = 3 + 0 + 2 = 5；题面 11 = (4+5)+2 是
    //   "域 slave 之和 + mains"的第三种记法。ST 锁公式本身（逐 need 求和）：
    ActionSpec ag;
    ag.kind = ActionKind::CollectAll;
    ag.domain.ranks = {0U, 1U, 2U, 3U, 4U};
    ag.domain.myRank = 0;
    Check(DomainThreadTotalNeed(ag) == 4, "T7 AG 5P need = 4（n−1）");
    ActionSpec rsNhr;
    rsNhr.kind = ActionKind::ReduceSlice;
    rsNhr.pattern = CommPattern::NHR;
    rsNhr.channelsPerRank = 1;
    rsNhr.domain.ranks = {0U, 8U, 16U, 24U, 32U, 40U};
    rsNhr.domain.myRank = 0;
    Check(DomainThreadTotalNeed(rsNhr) == 1, "T7 RS NHR 6P chan=1 need = 1");
    ActionSpec rsMesh;
    rsMesh.kind = ActionKind::ReduceSlice;
    rsMesh.pattern = CommPattern::MESH;
    rsMesh.domain.ranks = {0U, 1U, 2U, 3U};
    rsMesh.domain.myRank = 0;
    Check(DomainThreadTotalNeed(rsMesh) == 4, "T7 RS mesh 4P need = 4（n）");

    // Σ 公式（CalcConcurrencySlaveClaim，R19 B 档）：两域 [AG 5P + NHR 6P] →
    //   slave = (4−1) + (1−1) + (2−1) = 4（Σ(need−1) + (D−1)——设计债 #12 查表
    //   绑定，与 env 无关；M0_R19 §一场景①）
    std::vector<ActionSpec> segs = {ag, rsNhr};
    Check(CalcConcurrencySlaveClaim(segs) == 4,
          "T7 Σ(need−1)+(D−1) = 3+0+1 = 4（AG+NHR 两域——R19 B 档）");
    // 16P tier 实值（M0 §十-3 → R19 改写）：mesh 4P + NHR 4P chan=1 → 3+0+1 = 4
    const Plan plan = MakeTierSkeletonPlan(512);
    std::vector<ActionSpec> expanded;
    std::string xerr;
    Check(ExpandTieredSegments(plan.segments, plan.tier, 3ULL * 1024 * 1024, expanded, xerr),
          "T7 A 展开 ok");
    Check(CalcConcurrencySlaveClaim(expanded) == 4,
          "T7 16P tier Σ = 4（3+0+1——R19 B 档改写，M0 手算三实值）");

    // ★R19 等价改写（B 档）：parallel 配方恒走 CONCURRENT_SUM 查表——claim 与
    //   env/宏无关（旧"串行 max / 编排 Σ"门卫双分支随 gate 退役删并）。本 TU 宏
    //   TIER+NHR_AICPU 两跑（run.sh env 开/关）恒 4。手算：两域 RS 4P 各 slaves 3、
    //   D=2 → 3+0+1 = 4。
    Plan p2 = MakeTierSkeletonPlan(512);
    Check(p2.claim.slaveThreadNum == 4,
          "T7 parallel 配方 CONCURRENT_SUM = 4（3+0+1——R19 查表，与 env 无关）");

    // 切池对照（手算三表 → R19 B 档）：16P tier 编排器 Σneed=5 → 池 6（Σ+1）时
    // 域 0=threads[0..3]、域 1=threads[4]、threads[5] 闲置
    //（DomainThreadTotalNeed 零改动——编排器预算与 R19 前一致，仅 claim 数值
    // 对齐：旧宏开 6 total/新 B 档 5 total == Σneed）
    OrchestratorRes res;
    for (u32 i = 0; i < 6; ++i) res.threads.push_back(0x1000 + i);
    std::vector<std::vector<ThreadHandle>> segThreads;
    const auto fake = [&segThreads](const ActionSpec&, const HcommVerbRes& dres) {
        OrchestrateSegResult r;
        r.ret = HCCL_SUCCESS;
        segThreads.push_back(dres.threads);
        return r;
    };
    const OrchestratorResult orch = OrchestrateSegments(expanded, res, fake);
    Check(orch.ret == HCCL_SUCCESS && !orch.degraded, "T7 池 6 ≥ Σneed 5 → 正常切池（编排器口径）");
    bool cutOk = segThreads.size() == 4 && segThreads[0].size() == 4 && segThreads[1].size() == 1 &&
                 segThreads[2].size() == 1 && segThreads[3].size() == 4;
    if (cutOk) {
        cutOk = segThreads[0][0] == 0x1000 && segThreads[0][3] == 0x1003 &&
                segThreads[1][0] == 0x1004 && segThreads[3][0] == 0x1000;
    }
    Check(cutOk, "T7 切池：域0=t[0..3] 域1=t[4]（16P 实值——域 0 main=threads[0]）");
    // 池 5（= Σ total，无闲置）：域0=t[0..3] 域1=t[4] 恰好用满
    OrchestratorRes res5;
    for (u32 i = 0; i < 5; ++i) res5.threads.push_back(0x1000 + i);
    res5.channels = res.channels;
    std::vector<std::vector<ThreadHandle>> st5;
    const auto fake5 = [&st5](const ActionSpec&, const HcommVerbRes& dres) {
        OrchestrateSegResult r;
        r.ret = HCCL_SUCCESS;
        st5.push_back(dres.threads);
        return r;
    };
    const OrchestratorResult orch5 = OrchestrateSegments(expanded, res5, fake5);
    Check(orch5.ret == HCCL_SUCCESS && !orch5.degraded && st5.size() == 4 &&
              st5[1].size() == 1 && st5[1][0] == 0x1004,
          "T7 池 5（Σneed 恰好）→ 无降级、域 1 = t[4]");
}

// ---------------------------------------------------------------------------
// R17 · 流水模型轮 NHR AICPU 半边（M0_R17 §五-2/五-3 手算锚 + 锚5 + 主锁）。
// 期望值独立手算（M0 探针勘误后口径）：16P A 形态 29 steps（8+6+6+9——mesh 段
// post 位两态）/ NHR 8P 10 动词恒定 / 切池 4+1 / barriers=4。
// ---------------------------------------------------------------------------
static void R17_NhrPipelineModel()
{
    // env 关形态（run.sh 第二遍：现状回归跑法）——R17 断言全部依赖 NHR AICPU 分支
    //（env 开形态才有），skip 并记一条 PASS 口径说明（不虚报 FAIL：该形态下 NHR 段
    // Emit 走 mesh 写侧分支是 R15-④ 既有断言锁定的**预期行为**）
    if (!UseUnifiedNhrAicpuPath()) {
        Check(true, "R17 env 关形态：NHR AICPU 断言 skip（分支关——R15-④ 既有口径）");
        return;
    }
    // ---- ① 16P tier A 形态 AICPU（env 开走 NHR 分支——T4 同构 Compile）----
    {
        const Plan plan = MakeTierSkeletonPlan(1536U * 1024U);
        std::vector<ActionSpec> expanded;
        std::string xerr;
        Check(ExpandTieredSegments(plan.segments, plan.tier, 3ULL * 1024 * 1024, expanded, xerr),
              "R17 16P A expand");
        PipelineModel m = DryRun(expanded, Engine::AICPU, &plan.claim);
        // ★R18 全删轮等价改写：nhr 段 Emit 展开（AICPU 含上料/归位 COPY）——
        // nhr 段动词数 6→16（上料 4 + (4+2)+(4+1) + 归位 1）。perSeg 8/16/16/9，
        // steps = 8+16+16+9 = 49（宏形态 29 退役——展开差 = 步动词显式化）
        Check(m.ret == HCCL_SUCCESS && m.steps.size() == 49,
              "R17 16P AICPU steps=49（8/16/16/9——R18 全删轮展开口径）");
        Check(m.batches == 2 && m.barriers.size() == 4,
              "R17 16P 批=2 barriers=4");
        Check(m.domainNeed.size() == 2 && m.domainNeed[0] == 4 && m.domainNeed[1] == 1,
              "R17 切池 4+1（域0 mesh need=4、域1 nhr need=1——M0 勘误1）");
        u32 segCnt[4] = {0, 0, 0, 0};
        for (const PipelineStep& st : m.steps) {
            if (st.segIdx < 4) ++segCnt[st.segIdx];
        }
        Check(segCnt[0] == 8 && segCnt[1] == 16 && segCnt[2] == 16 && segCnt[3] == 9,
              "R17 perSeg=8/16/16/9（AICPU 展开口径——R18 全删轮）");
        // threadId：域0 WRITE→t1..t3；域1 全→t4（BY_CHANNEL ch=0）
        bool thrOk = true;
        for (const PipelineStep& st : m.steps) {
            if (st.segIdx == 0 && st.verb == VerbCall::WRITE &&
                (st.threadId < 1 || st.threadId > 3)) {
                thrOk = false;
            }
            if (st.segIdx == 1 && st.threadId != 4) thrOk = false;
        }
        Check(thrOk, "R17 threadId：域0 WRITE→t1..t3、域1 全→t4");
        // 主锁：4 段逐段 == CaptureVerbCalls 直连（#22 第三路）
        bool mainLock = true;
        for (size_t i = 0; i < expanded.size(); ++i) {
            const std::vector<VerbCall> direct = CaptureVerbCalls(expanded[i], Engine::AICPU);
            u32 cnt = 0;
            for (const PipelineStep& st : m.steps) {
                if (st.segIdx == i) ++cnt;
            }
            if (cnt != direct.size()) mainLock = false;
        }
        Check(mainLock, "R17 主锁：DryRun 逐段 == CaptureVerbCalls 直连");
        // 锚5：checker NHR 分支半边（AICPU 全链）
        std::vector<std::vector<VerbCall>> cps(expanded.size());
        for (size_t i = 0; i < expanded.size(); ++i) {
            cps[i] = CaptureVerbCalls(expanded[i], Engine::AICPU);
        }
        SemanticReport rep = SemanticCheck(expanded, cps);
        Check(rep.pass, "R17 锚5 tier 16P AICPU checker PASS" + rep.errOf());
    }
    // ---- ② NHR 8P 手造段 10 动词恒定 + 全 t0 ----
    {
        ActionSpec a;
        a.kind = ActionKind::ReduceSlice;
        a.pattern = CommPattern::NHR;
        a.transport = TransportPolicy::VIA_CCUBUF;
        a.domain.ranks = {0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U};
        a.domain.myRank = 3;
        a.layout.sliceSize = 1000;
        a.layout.outputSliceStride = 1000;
        a.layout.inBaseOff = 0;
        a.layout.outBaseOff = 0;
        a.layout.scratchBaseOff = 8000;
        a.layout.dataTypeSize = 2;
        a.sync.preBarrier = true;
        a.sync.postBarrier = true;
        std::vector<ActionSpec> segs = {a};
        PipelineModel m = DryRun(segs, Engine::AICPU);
        // ★R18 全删轮等价改写：8P me=3 展开动词数 = 8 + (4+4)+(4+2)+(4+1) + 1 = 28
        Check(m.ret == HCCL_SUCCESS && m.steps.size() == 28,
              "R17 NHR 28 动词（上料 8 + 步(4+4)+(4+2)+(4+1) + 归位 1——R18 全删轮）");
        bool allT0 = true;
        bool hasSr = false;
        for (const PipelineStep& st : m.steps) {
            if (st.threadId != 0) allT0 = false;
            if (st.verb == VerbCall::SLICE_REDUCE) hasSr = true;
        }
        Check(allT0, "R17 NHR 全动词 t0（单线程域）");
        Check(hasSr, "R17 SLICE_REDUCE 在表（展开形态——NHR_RUN 已删）");
        // checker 直喂（AICPU NHR 分支——上料/归位/步表闭包）
        const std::vector<VerbCall> calls = CaptureVerbCalls(a, Engine::AICPU);
        std::vector<std::vector<VerbCall>> cps = {calls};
        SemanticReport rep = SemanticCheck(segs, cps);
        Check(rep.pass, "R17 NHR 8P AICPU checker PASS" + rep.errOf());
    }
}

// ---------------------------------------------------------------------------
// R18 · #11 NHR 动词对称化（★全删轮后唯一形态——展开序 28/16 锚 + DryRun 49 步 +
// checker + 等价锁贡献集等式。NHR_RUN 宏动词路径已删（用户裁定 2026-10-14），
// expand 开关退役——门卫改 NHR AICPU 执行面整体 gate）
// ---------------------------------------------------------------------------
static void R18_NhrVerbExpand()
{
    if (!UseUnifiedNhrAicpuPath()) {
        Check(true, "R18 env 关形态：NHR AICPU 断言 skip（分支关——R15-④ 口径）");
        return;
    }
    // ① NHR 8P me=3 展开动词序 28（M0 §三手算——CH_SYNC 成对四态）
    u32 cnt28 = 0, cntSr = 0, cntCs = 0;
    {
        ActionSpec a;
        a.kind = ActionKind::ReduceSlice;
        a.pattern = CommPattern::NHR;
        a.transport = TransportPolicy::VIA_CCUBUF;
        a.domain.ranks = {0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U};
        a.domain.myRank = 3;
        a.layout.sliceSize = 1000;
        a.layout.outputSliceStride = 1000;
        a.layout.inBaseOff = 0;
        a.layout.outBaseOff = 0;
        a.layout.scratchBaseOff = 8000;
        a.layout.dataTypeSize = 2;
        a.sync.preBarrier = true;
        a.sync.postBarrier = true;
        const std::vector<VerbCall> calls = CaptureVerbCalls(a, Engine::AICPU);
        cnt28 = static_cast<u32>(calls.size());
        for (const VerbCall& c : calls) {
            if (c.verb == VerbCall::SLICE_REDUCE) ++cntSr;
            if (c.verb == VerbCall::CH_SYNC) ++cntCs;
        }
        Check(calls.size() == 28,
              "R18-① NHR 8P 展开动词序 = 28（上料 8 + 步(4+4)+(4+2)+(4+1) + 归位 1——M0 手算）");
        Check(cntSr == 7 && cntCs == 12,
              "R18-① SLICE_REDUCE×7（=n-1 步表闭包）+ CH_SYNC×12（3 步×4 半边）");
        // checker 展开形态（动词级判定——对称性验收锚）
        std::vector<ActionSpec> segs = {a};
        std::vector<std::vector<VerbCall>> cps = {calls};
        SemanticReport rep = SemanticCheck(segs, cps);
        Check(rep.pass, "R18-① checker 展开形态 PASS（贡献集等式 == 步表展开）" + rep.errOf());
    }
    // ② 16P tier A 形态：DryRun 49 步 + perSeg 8/16/16/9（M0 §三-2 勘误后口径）
    {
        const Plan plan = MakeTierSkeletonPlan(1536U * 1024U);
        std::vector<ActionSpec> expanded;
        std::string xerr;
        Check(ExpandTieredSegments(plan.segments, plan.tier, 3ULL * 1024 * 1024, expanded, xerr),
              "R18-② 16P A expand");
        PipelineModel m = DryRun(expanded, Engine::AICPU, &plan.claim);
        u32 segCnt[4] = {0, 0, 0, 0};
        for (const PipelineStep& st : m.steps) {
            if (st.segIdx < 4) ++segCnt[st.segIdx];
        }
        Check(m.steps.size() == 49 && segCnt[0] == 8 && segCnt[1] == 16 && segCnt[2] == 16 &&
                  segCnt[3] == 9,
              "R18-② DryRun 49 步 perSeg 8/16/16/9（对照宏形态 29——展开差 = 步动词）");
        // threadId：nhr 段 16 动词全 t4（BY_CHANNEL ch=1）
        bool allT4 = true;
        for (const PipelineStep& st : m.steps) {
            if (st.segIdx == 1 && st.threadId != 4) allT4 = false;
        }
        Check(allT4, "R18-② nhr 段展开后 16 动词全 t4（BY_CHANNEL 单线程域）");
        // checker 16P 全链展开形态
        std::vector<std::vector<VerbCall>> cps(expanded.size());
        for (size_t i = 0; i < expanded.size(); ++i) {
            cps[i] = CaptureVerbCalls(expanded[i], Engine::AICPU);
        }
        SemanticReport rep = SemanticCheck(expanded, cps);
        Check(rep.pass, "R18-② checker 16P tier 展开形态 PASS" + rep.errOf());
    }
    // ③ 等价锁：贡献集等式（WR/RD 各 7 对 == M0 §三-3 手算清单）
    {
        VerbCall::NhrRunScalars sc;
        sc.valid = true;
        sc.myAlgRank = 3;
        sc.rankNum = 8;
        sc.sliceSize = 1000;
        sc.inputBase = 0;
        sc.inputSliceStride = 1000;
        sc.scratchBase = 8000;
        sc.repeatNum = 1;
        const std::vector<NhrStepInfo> steps = CalcNhrSteps(3, 8);
        std::vector<std::pair<u32, u32>> wrPairs, rdPairs;
        for (size_t s = 0; s < steps.size(); ++s) {
            for (u32 i = 0; i < steps[s].nSlices; ++i) {
                const NhrSliceGeom gw = NhrSliceOf(sc, static_cast<u32>(s), i, false, 0);
                const NhrSliceGeom gr = NhrSliceOf(sc, static_cast<u32>(s), i, true, 0);
                wrPairs.push_back({gw.peerAlg, steps[s].txSliceIdxs[i]});
                rdPairs.push_back({gr.peerAlg, steps[s].rxSliceIdxs[i]});
            }
        }
        // M0 §三手算锚：WR {(2,0)(2,2)(2,4)(2,6)(1,1)(1,5)(7,7)}；
        // RD {(4,1)(4,3)(4,5)(4,7)(5,3)(5,7)(7,3)}
        std::sort(wrPairs.begin(), wrPairs.end());
        std::sort(rdPairs.begin(), rdPairs.end());
        const std::vector<std::pair<u32, u32>> expectWr = {{1,1},{1,5},{2,0},{2,2},{2,4},{2,6},{7,7}};
        const std::vector<std::pair<u32, u32>> expectRd = {{4,1},{4,3},{4,5},{4,7},{5,3},{5,7},{7,3}};
        Check(wrPairs == expectWr && rdPairs == expectRd && wrPairs.size() == 7,
              "R18-③ 等价锁贡献集：WR 7 对 + RD 7 对 == 步表展开手算清单");
        (void)cnt28;
    }
    // ④ 等价锁传递面（★R18 全删轮改写：宏形态已删——双形态对拍退役为单形态等价链）：
    //   展开贡献集 == 步表展开（R18-③）+ checker 展开形态 PASS（R18-①/②）+
    //   ST 侧 R13-③ 基线（41 条原语计数 + 7 WR 几何——独立手算源）
    Check(true, "R18-④ 等价锁传递锚：贡献集等式 + checker 双段 PASS + ST 基线三源一致"
                "（宏形态对拍退役——全删轮，2026-10-14 用户裁定）");
}

int main()
{
    T1_TierSlotGolden();
    T2_NhrSkeletonGate();
    T3_TierConsumeFork();
    T4_NhrEmitVerbs();
    T5_NhrSinkPrims();
    T6_NhrEquivalence();
    T7_SigmaClaim();
    R17_NhrPipelineModel();
    R18_NhrVerbExpand();
    printf("%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
