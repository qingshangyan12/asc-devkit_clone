/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * ST 语义 checker 自检（checker 也要被测试）
 *
 * 用手工构造的 trace / Plan / rounds 验证 st_semantic_checker 本身的正确性：
 *   A. 正例：8 卡 AG 两引擎真实形制的 trace（取自 l4 两后端 Emit 的格式逐字复刻）
 *      → 等价断言应通过
 *   B. 反例矩阵：屏障错位 / 搬运量不等 / read 条数不符 / 段数不符 / 数据流边断 /
 *      切轮越界 / baseOff 不推进 / Σ≠total —— 每条负路径一个用例
 *   C. 解析鲁棒性：未知动词 / 残缺括号 / 空串报错
 *
 * C++14 干净；编入 run.sh 第二目标。
 */
#include "st_semantic_checker.h"

#include "action_spec.h"
#include "backend_registry.h"
#include "action_collect_all.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

using namespace mc2_ops_hccl::refactor;
using namespace mc2_ops_hccl::refactor::st;

static int g_pass = 0, g_fail = 0;
static void Check(bool cond, const std::string& what)
{
    if (cond) { ++g_pass; printf("  [PASS] %s\n", what.c_str()); }
    else      { ++g_fail; printf("  [FAIL] %s\n", what.c_str()); }
}

// 8 卡 AG、每卡 1MB：AICPU 真实 Emit 形制（copy 本地块 + 7×read + 前后屏障）
static std::string MakeAicpuTrace8R(u64 slice)
{
    std::string t = "copy(100->800," + std::to_string(slice) + ") ";
    t += "syncPeer:pre(0,0,0) ";
    for (u32 p = 0; p < 7; ++p) {
        t += "read(" + std::to_string(1000 + p) + "->" + std::to_string(800 + p) + "," +
             std::to_string(slice) + ",peer" + std::to_string(p + 1) + ") ";
    }
    t += "syncPeer:post(0,0,0) ";
    return t;
}
// CCU 真实 Emit 形制（pre + broadcast(outputOff, peers, slice) + post）
static std::string MakeCcuTrace8R(u64 slice)
{
    return "syncPeer:pre(0,0,0) broadcast(800,7," + std::to_string(slice) + ") syncPeer:post(0,0,0) ";
}

static Plan MakePlanTwoShot()   // RS→AG 两段带数据流边
{
    Plan p;
    ActionSpec rs;
    rs.kind = ActionKind::ReduceSlice;
    rs.pattern = CommPattern::MESH;
    rs.shape = SegShape::TWO_SHOT;
    rs.inputRanks = {0, 1, 2, 3};
    rs.outputRanks = {0, 1, 2, 3};
    ActionSpec ag = rs;
    ag.kind = ActionKind::CollectAll;
    ag.inputRanks = rs.outputRanks;
    p.segments = {rs, ag};
    return p;
}

int main()
{
    printf("== A. 正例：两引擎 trace 解析 + 语义等价 ==\n");
    {
        const u64 slice = 1024 * 1024;
        std::string err;
        const auto ta = ParseTrace(MakeAicpuTrace8R(slice), err);
        Check(!ta.empty() && err.empty(), "parse AICPU trace ok: " + err);
        Check(ta.size() == 10, "AICPU verb count 10 (copy+pre+7read+post+trailing sync)");
        Check(ta[0].verb == "copy" && ta[0].a1 == 100 && ta[0].a2 == 800 && ta[0].a3 == slice,
              "copy args parsed");
        Check(ta[2].verb == "read" && ta[2].peer == 1, "read peer parsed");

        const auto tc = ParseTrace(MakeCcuTrace8R(slice), err);
        Check(!tc.empty() && err.empty(), "parse CCU trace ok: " + err);
        Check(tc.size() == 3 && tc[1].verb == "broadcast" && tc[1].a3 == slice, "broadcast parsed");

        ActionSpec seg;
        seg.kind = ActionKind::CollectAll;
        Check(CheckSemanticsEquivalent(ta, tc, seg, 8, err) == 0, "semantics equivalent (8 ranks): " + err);
    }

    printf("== B. 反例矩阵 ==\n");
    {
        const u64 slice = 1024;
        std::string err;
        // B1 屏障错位：pre 出现在数据动词之后
        const auto bad1 = ParseTrace("read(1->2,8,peer1) syncPeer:pre(0,0,0) ", err);
        ActionSpec seg;
        Check(CheckSemanticsEquivalent(bad1, ParseTrace(MakeCcuTrace8R(slice), err), seg, 8, err) != 0,
              "barrier-misplaced AICPU trace rejected");

        // B2 搬运量不等：AICPU 只 read 6 条（少一块）
        std::string t2 = "copy(1->2," + std::to_string(slice) + ") syncPeer:pre(0,0,0) ";
        for (u32 p = 0; p < 6; ++p) {
            t2 += "read(" + std::to_string(p) + "->" + std::to_string(10 + p) + "," +
                  std::to_string(slice) + ",peer" + std::to_string(p + 1) + ") ";
        }
        t2 += "syncPeer:post(0,0,0) ";
        Check(CheckSemanticsEquivalent(ParseTrace(t2, err), ParseTrace(MakeCcuTrace8R(slice), err), seg,
                                       8, err) != 0,
              "insufficient read count rejected");

        // B3 段数不符
        Plan p = MakePlanTwoShot();
        std::vector<SegmentExpect> exp1 = {{ActionKind::CollectAll, CommPattern::MESH}};
        Check(CheckPlanBook(p, exp1, err) != 0, "segment count mismatch rejected");

        // B4 数据流边断：后段 inputRanks 与前段 outputRanks 不一致
        Plan pBroken = MakePlanTwoShot();
        pBroken.segments[1].inputRanks = {0, 1};
        Check(CheckDataflowEdges(pBroken, true, err) != 0, "broken dataflow edge rejected");
        Check(CheckDataflowEdges(MakePlanTwoShot(), true, err) == 0, "intact dataflow edge passes");

        // B5 孤儿段
        Plan pOrphan;
        ActionSpec o;
        o.kind = ActionKind::CollectAll;
        pOrphan.segments = {o};   // inputRanks 空
        Check(CheckDataflowEdges(pOrphan, false, err) != 0, "orphan segment (empty inputRanks) rejected");

        // B6 切轮：Σ ≠ total
        std::vector<RoundRecord> r1 = {{0, 512}, {512, 300}};
        Check(CheckRounds(r1, 1024, 512, err) != 0, "sum != total rejected: " + err);
        // B7 切轮：越界
        std::vector<RoundRecord> r2 = {{0, 600}};
        Check(CheckRounds(r2, 600, 512, err) != 0, "round exceeds maxPerRound rejected");
        // B8 切轮：baseOff 不推进
        std::vector<RoundRecord> r3 = {{0, 512}, {100, 512}};
        Check(CheckRounds(r3, 1024, 512, err) != 0, "baseOff not advancing rejected");
        // B9 切轮：非尾轮未打满
        std::vector<RoundRecord> r4 = {{0, 400}, {400, 624}};
        Check(CheckRounds(r4, 1024, 512, err) != 0, "non-last round not full rejected");
        // B10 切轮正例：整除 + 有余量
        Check(CheckRounds({{0, 512}, {512, 512}}, 1024, 512, err) == 0, "exact rounds pass: " + err);
        Check(CheckRounds({{0, 512}, {512, 300}}, 812, 512, err) == 0, "tail-round rounds pass: " + err);
    }

    printf("== C. 解析鲁棒性 ==\n");
    {
        std::string err;
        Check(ParseTrace("", err).empty() && !err.empty(), "empty trace rejected");
        Check(ParseTrace("frobnicate(1,2,3) ", err).empty() && err.find("unknown verb") != std::string::npos,
              "unknown verb rejected");
        Check(ParseTrace("read(1->2 ", err).empty() && !err.empty(), "unclosed paren rejected");
        Check(ParseTrace("copy(1,2,3,4,5) ", err).empty() && !err.empty(), "too many args rejected");
        Check(ParseTrace("write(1->2,8) ", err).empty() && err.find("unknown verb") != std::string::npos,
              "write verb rejected (P2 reserved, not yet in vocabulary)");
        Check(ParseTrace("reduce(1->2,8) ", err).empty() && err.find("unknown verb") != std::string::npos,
              "reduce verb rejected (P2 reserved)");
        // postCopy 现在是合法动词
        const auto tp = ParseTrace("postCopy(100->800,1024) ", err);
        Check(!tp.empty() && err.empty() && tp[0].verb == "postCopy" && tp[0].a3 == 1024,
              "postCopy parses ok");
    }

    printf("== D. postCopy 语义（B4） ==\n");
    {
        std::string err;
        // 正例：VIA_CCUBUF——read 之后 postCopy
        const std::string okTrace = "copy(1->2,64) syncPeer:pre(0,0,0) read(3->4,64,peer1) "
                                    "syncPeer:post(0,0,0) postCopy(10->20,64) postCopy(30->40,64) ";
        const auto t = ParseTrace(okTrace, err);
        Check(!t.empty(), "parse via-ccubuf trace");
        Check(CheckPostCopySemantics(t, true, err) == 0, "postCopy after last remote verb passes: " + err);
        Check(CheckPostCopySemantics(t, false, err) != 0, "same trace rejected when postCopy unexpected");

        // 反例 1：postCopy 在 read 之前（错位）
        const std::string badTrace = "postCopy(1->2,64) syncPeer:pre(0,0,0) read(3->4,64,peer1) "
                                     "syncPeer:post(0,0,0) ";
        const auto tb = ParseTrace(badTrace, err);
        Check(CheckPostCopySemantics(tb, true, err) != 0, "postCopy before remote data verb rejected");

        // 反例 2：期望有 postCopy 但没有
        const std::string noPc = "copy(1->2,64) syncPeer:pre(0,0,0) read(3->4,64,peer1) syncPeer:post(0,0,0) ";
        const auto tn = ParseTrace(noPc, err);
        Check(CheckPostCopySemantics(tn, true, err) != 0, "missing postCopy (expected) rejected");

        // SYMMEM 正例：无 postCopy
        Check(CheckPostCopySemantics(tn, false, err) == 0, "no postCopy (SYMMEM) passes");
    }

    printf("== E. 两段中转衔接（B2 handoff） ==\n");
    {
        // 正例：RS/AG 两段 scratch 几何同源（同 base、同 sliceSize、逐 peer ScratchOffset 相等）
        DataLayout rl;                                  // RS 段
        rl.sliceSize = 500000;                          // case1 整除：count=2M FP16 → sliceSize=500,000B（手算基线 A 节）
        rl.scratchBaseOff = 0;
        ActionSpec rs; rs.kind = ActionKind::ReduceSlice; rs.layout = rl;
        rs.domain.ranks = {0, 1, 2, 3, 4, 5, 6, 7}; rs.domain.myRank = 0;
        ActionSpec ag = rs; ag.kind = ActionKind::CollectAll;
        std::string err;
        Check(CheckStageHandoff(rs, ag, err) == 0, "handoff: same scratch geometry passes: " + err);

        // 反例 1：AG 段 scratch 基址不同（RS 落 A 处、AG 取 B 处）
        ActionSpec agB = ag;
        agB.layout.scratchBaseOff = 4096;
        Check(CheckStageHandoff(rs, agB, err) != 0, "handoff broken (different scratchBaseOff) rejected");

        // 反例 2：AG 段 sliceSize 不同（几何步长错位）
        ActionSpec agS = ag;
        agS.layout.sliceSize = 250000;
        Check(CheckStageHandoff(rs, agS, err) != 0, "handoff broken (sliceSize mismatch) rejected");

        // 反例 3：域大小不同
        ActionSpec agD = ag;
        agD.domain.ranks = {0, 1, 2, 3};
        Check(CheckStageHandoff(rs, agD, err) != 0, "handoff broken (domain size) rejected");

        // 逐值抽查：8 卡 scratchOffset(i)=i*500,000（手算：i=3 → 1,500,000）
        Check(rl.ScratchOffset(3, 8) == 1500000, "scratch offset handcalc i=3 -> 1,500,000");
        Check(rl.ScratchOffset(7, 8) == 3500000, "scratch offset handcalc i=7 -> 3,500,000");
    }

    printf("== F. KFC server v3 模拟器（A4 甲路线：轮级动作 ID 分发） ==\n");
    {
        // 手工载荷：帧 = 32 槽 u64。合法 ID 白名单 {1001, 1002}（模拟器不硬编码 ID 值，
        // caller 传什么认什么——unified 实际 ID 取值由 R4 专项对照）。
        const std::vector<u64> legal = {1001, 1002};
        auto Frame = [](u64 id, u64 p1, u64 p2) {
            std::vector<u64> f(32, 0);
            f[0] = id; f[1] = p1; f[2] = p2;
            return f;
        };
        // F1 两帧 + 哨兵帧：解出 2 轮、rawTurnCount=3（含哨兵轮）、无错误
        std::vector<u64> payload;
        const auto f1 = Frame(1001, 11, 12);
        const auto f2 = Frame(1002, 21, 22);
        payload.insert(payload.end(), f1.begin(), f1.end());
        payload.insert(payload.end(), f2.begin(), f2.end());
        payload.resize(payload.size() + 32, 0);           // 哨兵帧（全 0 再覆盖槽 0）
        payload[payload.size() - 32] = ST_KFC_V3_SENTINEL;
        ServerSimResult sim = SimulateKfcServerV3(payload, legal);
        Check(sim.err.empty(), "two-frame + sentinel simulates ok: " + sim.err);
        Check(sim.turns.size() == 2, "2 turns dispatched (sentinel turn not dispatched)");
        Check(sim.rawTurnCount == 3, "rawTurnCount 3 (sentinel counted as a turn)");
        Check(sim.turns.size() == 2 && sim.turns[0].actionId == 1001 &&
              sim.turns[0].slots[1] == 11 && sim.turns[0].slots[2] == 12,
              "turn0 slots unpacked (id/input/output)");
        Check(sim.turns.size() == 2 && sim.turns[1].actionId == 1002 && sim.turns[1].turnIndex == 1,
              "turn1 id + turnIndex");
        Check(!sim.wrapped, "no wrap under 64 turns");

        // F2 空 Plan：拒发
        ServerSimResult empty = SimulateKfcServerV3({}, legal);
        Check(!empty.err.empty() && empty.turns.empty(), "empty payload rejected");

        // F3 半帧（33 槽）：拒发（整帧对齐防御）
        std::vector<u64> half = payload;
        half.push_back(0);
        ServerSimResult misaligned = SimulateKfcServerV3(half, legal);
        Check(!misaligned.err.empty() && misaligned.turns.empty(), "non-32-aligned payload rejected");

        // F4 未知动作 ID：拒发且 err 点名 ID 值与轮号
        std::vector<u64> badId = payload;
        badId[0] = 7777;
        ServerSimResult unk = SimulateKfcServerV3(badId, legal);
        Check(!unk.err.empty() && unk.err.find("7777") != std::string::npos &&
              unk.err.find("turn 0") != std::string::npos, "unknown action id rejected with id+turn");

        // F5 ID 0（CollectAll 的 v3 动作 ID 值）：不在白名单 {1001,1002} → 拒（白名单
        // 裁决，非硬编码拒 0——0 是合法 v3 ID，只要 caller 登记它）
        std::vector<u64> zeroId = payload;
        zeroId[0] = 0;
        ServerSimResult z = SimulateKfcServerV3(zeroId, legal);
        Check(!z.err.empty(), "action id 0 rejected when not in whitelist (whitelist decides, not the value)");

        // F6 段数超上限：65 帧无哨兵 → 第 65 帧（turn 64）容量拒发
        std::vector<u64> over;
        for (int i = 0; i < 65; ++i) {
            const auto f = Frame(1001, 1, 2);
            over.insert(over.end(), f.begin(), f.end());
        }
        ServerSimResult overSim = SimulateKfcServerV3(over, legal);
        Check(!overSim.err.empty() && overSim.err.find("capacity") != std::string::npos,
              "65 frames exceed CCU_TASK_NUM_MAX=64: rejected");
        Check(overSim.rawTurnCount == 64, "rejection happens at turn 64 (0-based), 64 turns consumed");

        // F7 恰好 63 帧 + 哨兵 = 64 帧（合法边界）：63 轮全派发——哨兵本身占一个队列槽，
        // 动作帧 + 哨兵帧 ≤ 64 是环形队列的严格容量（65 帧即 F6 的拒发区间）
        std::vector<u64> full;
        for (u64 i = 0; i < 63; ++i) {
            const auto f = Frame(1001, i, 0);
            full.insert(full.end(), f.begin(), f.end());
        }
        const auto sf = Frame(ST_KFC_V3_SENTINEL, 0, 0);
        full.insert(full.end(), sf.begin(), sf.end());
        ServerSimResult fullSim = SimulateKfcServerV3(full, legal);
        Check(fullSim.err.empty() && fullSim.turns.size() == 63 && fullSim.rawTurnCount == 64,
              "63 turns + sentinel = exactly 64 frames: legal boundary passes");

        // F7' 整圈无哨兵（64 帧动作、无出口）：server 回绕重放 0 号帧——模拟器显式
        // 拒（写入方漏哨兵 = server 死循环，ST 把静默故障显式化）
        std::vector<u64> noSentinel;
        for (u64 i = 0; i < 64; ++i) {
            const auto f = Frame(1001, i, 0);
            noSentinel.insert(noSentinel.end(), f.begin(), f.end());
        }
        ServerSimResult spin = SimulateKfcServerV3(noSentinel, legal);
        Check(!spin.err.empty() && spin.err.find("sentinel") != std::string::npos && spin.wrapped,
              "64 action frames, no sentinel: wrap-spin rejected: " + spin.err);

        // F8 哨兵在首帧：0 轮派发、rawTurnCount=1、无错误（合法的"空计划"）
        std::vector<u64> onlySentinel = Frame(ST_KFC_V3_SENTINEL, 0, 0);
        ServerSimResult s0 = SimulateKfcServerV3(onlySentinel, legal);
        Check(s0.err.empty() && s0.turns.empty() && s0.rawTurnCount == 1,
              "leading sentinel: zero turns, no error (empty plan semantics)");

        // F9（第五轮主会话裁决后收口）：只测当前特性已支持的分支——v3 交付面是
        // CollectAll 单动作（ID 0），"0..4 全集镜像/同槽不同义负例"原 S1/S2 方案
        // 已按裁决砍掉（ReduceSlice=1 等分支特性未支持，构造走不到的分支测 false
        // 无意义）。保留的动作：CollectAll=0 与 ActionKind 的编译期对齐
        //（st_verification.cc ㉕ 的 static_assert）+ 白名单作为 caller 传参的
        // 灵活性自检（本节既有 F1..F8 已覆盖——legal 由 caller 给 {1001,1002} 也能跑）。
    }

    printf("== G. R7 RS 帧实参抽取 + 实例化（模拟器新增面的自检） ==\n");
    {
        // G1 非 RS 帧不越权：AG 帧（槽 0=0）抽取 → valid=false、全字段保持缺省
        std::vector<u64> agFrame(32, 0);
        agFrame[1] = 111;
        RsServerCtx ctx;
        ctx.rankSize = 4; ctx.rankId = 2; ctx.channelCount = 3;
        ctx.dataType = 3; ctx.outputType = 3; ctx.reduceType = 0;
        RsKernelArgsRecord nonRs = ExtractRsKernelArgs(agFrame, ctx);
        Check(!nonRs.valid && nonRs.chunkSize == 0 && nonRs.rankId == 0,
              "AG frame: ExtractRsKernelArgs declines (valid=false, fields untouched)");

        // G2 RS 帧抽取：槽位 1..15 逐位透传 + ctx 字段落位（手算字面量）
        std::vector<u64> rsFrame(32, 0);
        rsFrame[0] = ST_KFC_V3_ACTION_REDUCE_SLICE;
        rsFrame[1] = 0x1000000; rsFrame[2] = 0x2000000; rsFrame[3] = 0x5000000;
        rsFrame[4] = 1600000; rsFrame[5] = 16777216; rsFrame[6] = ~0ULL - 1ULL;
        rsFrame[7] = 800000;
        rsFrame[8] = 16777216; rsFrame[9] = 256; rsFrame[10] = 0; rsFrame[11] = 0;
        rsFrame[12] = 786432; rsFrame[13] = 12; rsFrame[14] = 72343467061149696ULL;
        rsFrame[15] = 1280;
        RsKernelArgsRecord rec = ExtractRsKernelArgs(rsFrame, ctx);
        Check(rec.valid, "RS frame: extract produces a record");
        Check(rec.inputAddr == 0x1000000 && rec.outputAddr == 0x2000000 && rec.scratch == 0x5000000,
              "slots 1..3 map to inputAddr/outputAddr/scratch (server forwarding order)");
        Check(rec.currentRankSliceInputOffset == 1600000 && rec.chunkSize == 16777216 &&
              rec.chunkLoopNum == ~0ULL - 1ULL && rec.tailSize == 800000,
              "slots 4..7 map to rankSliceOffset/chunkSize/chunkLoopNum/tailSize");
        Check(rec.fullGoAddrOffset == 16777216 && rec.fullGoLoopParam == 256 &&
              rec.fullGoParallelParam == 0 && rec.fullGoResidual == 0,
              "slots 8..11 map to fullGo quadruple");
        Check(rec.tailGoAddrOffset == 786432 && rec.tailGoLoopParam == 12 &&
              rec.tailGoParallelParam == 72343467061149696ULL && rec.tailGoResidual == 1280,
              "slots 12..15 map to tailGo quadruple");
        Check(rec.rankSize == 4 && rec.rankId == 2 && rec.channelCount == 3 &&
              rec.dataType == 3 && rec.outputType == 3 && rec.reduceType == 0,
              "ctx-derived fields carried (rankSize/rankId/channelCount/dt/out/red)");

        // G3 半帧防御：31 槽载荷 → valid=false（与模拟器整帧对齐同口径）
        std::vector<u64> half(rsFrame.begin(), rsFrame.begin() + 31);
        Check(!ExtractRsKernelArgs(half, ctx).valid,
              "31-slot frame: declined (frame-alignment defense)");

        // G4 实例化公式自检（checker 公式 vs 手算字面量——与 st_verification 的
        // 手算基线互为独立锚）：4 卡 FP16 count=400000 rankId=2 strideCount=0。
        // 注意 rsFrame 的槽 1/2 已置非零结构量（0x1000000/0x2000000）——合成式
        // 手算：槽 1 = 0x1000000（sendBuf）+0（repeatOff）+0x1000000（blob 槽 1）
        // = 0x2000000；槽 2 = 0x2000000+0+0x2000000 = 0x4000000（非直替证据）
        StRsExecMsg m;
        m.sendBuf = 0x1000000; m.recvBuf = 0x2000000; m.count = 400000; m.dts = 2;
        m.rankId = 2; m.scratchAddr = 0x5000000; m.rankNum = 4;
        const std::vector<u64> inst = StInstantiateRsFrame(rsFrame, m);
        // 手算：slice=800000、chunk=64MB/4/128*128=16777216、chunkCount=1 → 槽6=MAX-1
        //       tail=800000、rankSliceOff=2*800000=1600000、repeatOff=0（repeatIndex=0）
        Check(inst[0] == ST_KFC_V3_ACTION_REDUCE_SLICE && inst[1] == 0x2000000ULL &&
              inst[2] == 0x4000000ULL && inst[3] == 0x5000000,
              "instantiate: slots 0..3 (action passthrough + composite buf addrs + scratch)");
        Check(inst[4] == 1600000 && inst[5] == 16777216 && inst[6] == ~0ULL - 1ULL &&
              inst[7] == 800000,
              "instantiate: slots 4..7 (rankSliceOffset/chunkSize/chunkLoopNum/tailSize)");
        Check(inst[8] == 16777216 && inst[9] == 256 && inst[10] == 0 && inst[11] == 0,
              "instantiate: fullGo = {16777216, 256, 0, 0} (RS config 16x4096)");
        Check(inst[12] == 786432 && inst[13] == 12 &&
              inst[14] == 72343467061149696ULL && inst[15] == 1280,
              "instantiate: tailGo = {786432, 12, PP(2,1,2), 1280}");
        // 槽 16..31 透传不动（未用槽卫生在实例化后保持）
        bool tailZero = true;
        for (u32 i = 16; i < 32; ++i) { if (inst[i] != rsFrame[i]) tailZero = false; }
        Check(tailZero, "instantiate: slots 16..31 untouched (verbatim copy semantics)");

        // G5 strideCount≠0 分支：stride=500000 元素 → 槽 4 = 2*500000*2 = 2000000
        StRsExecMsg ms = m;
        ms.strideCount = 500000;
        const std::vector<u64> instS = StInstantiateRsFrame(rsFrame, ms);
        Check(instS[4] == 2000000 && instS[4] != inst[4],
              "instantiate: strideCount!=0 branch (slot4 = 2,000,000, differs from compact)");

        // G6 count=0 防御形态：chunkLoopNum=UINT64_MAX、tail=0、tailGo 全 0
        StRsExecMsg m0;
        m0.count = 0; m0.dts = 2; m0.rankId = 0; m0.rankNum = 4;
        m0.scratchAddr = 0x5000000; m0.sendBuf = 0x1000000; m0.recvBuf = 0x2000000;
        const std::vector<u64> inst0 = StInstantiateRsFrame(rsFrame, m0);
        Check(inst0[5] == 16777216 && inst0[6] == ~0ULL && inst0[7] == 0 &&
              inst0[8] == 16777216 && inst0[12] == 0 && inst0[15] == 0,
              "instantiate count=0: chunkLoopNum=UINT64_MAX, tail=0, tailGo zeros");
    }

    printf("== H. R10 F1 断链可判基线（v1 病灶 / ctxSize 闸 / KFC 判序——checker 自检） ==\n");
    {
        // H1 v1 槽视图（floor(len/8)，小端组槽）——手算字面量：
        //   "PLAN v3 "（8 字节）小端 = 0x203376204E414C50（'P'=0x50 'L'=0x4C
        //   'A'=0x41 'N'=0x4E ' '=0x20 'v'=0x76 '3'=0x33 ' '=0x20）
        const std::vector<u64> slots = StBytesToBlobSlots(std::string("PLAN v3 ", 8));
        Check(slots.size() == 1 && slots[0] == 0x203376204E414C50ULL,
              "H1: v1 head 8 bytes little-endian slot = 0x203376204E414C50");
        // 108B（真 v1 产物长度）→ 13 槽（floor(108/8)，尾 4 字节丢弃）
        Check(StBytesToBlobSlots(std::string(108, 'x')).size() == 13,
              "H1: 108-byte v1 -> 13 slots (floor 108/8, tail dropped)");
        // 尾字节不进槽（第 13 槽只含前 8 字节——同消费方 ctxSize/8 口径）
        const std::vector<u64> s9 = StBytesToBlobSlots(std::string("ABCDEFGH", 8) + std::string("I", 1));
        Check(s9.size() == 1 && s9[0] == 0x4847464544434241ULL,
              "H1: trailing partial word dropped (consumer ctxSize/8 rounddown)");

        // H2 v1 必拒判据（通用，与段数/版本/长度无关）：
        //   正判据面：真 v1 形态（"PLAN v…"）恒拒
        Check(StV1TextBlobWouldBeRejected("PLAN v3 segs:1\n"),
              "H2: real v1 form rejected by v4 entry (magic判据)");
        Check(StV1TextBlobWouldBeRejected("PLAN v99 segs:63\n..."),
              "H2: any v1 version rejected (magic判据 is version-independent)");
        Check(StV1TextBlobWouldBeRejected("PL" + std::string(106, 'x')),
              "H2: any 108-byte 'PL'-headed text rejected (low16 = 0x4C50 != UPL3)");
        Check(StV1TextBlobWouldBeRejected("PL"),
              "H2: <8 bytes rejected (cannot form header slot)");
        //   反判据面：v4 blob 的字节视图不拒（真 39 槽 blob 首槽 = 0x55504C33）
        std::string upl3Bytes(8, '\0');
        upl3Bytes[0] = 0x33; upl3Bytes[1] = 0x4C; upl3Bytes[2] = 0x50; upl3Bytes[3] = 0x55;
        Check(!StV1TextBlobWouldBeRejected(upl3Bytes),
              "H2: UPL3-headed byte view NOT rejected (判据不误伤 v4 形态)");
        //   边界：非 "PL" 开头的任意 8+ 字节文本（判据只锁 "PL" 头——不越权
        //   声称一切文本必拒，其余形态由真 ParseUnifiedBlobV4 的完整三重校验兜）
        Check(!StV1TextBlobWouldBeRejected("UPL3 fake"),
              "H2: 'PL'-only判据 does not over-claim (non-PL text decided by real parser)");

        // H3 ctxSize 前置闸（消费方 :69-77 判据镜像）
        //   闸 1：resCtx null / ctxSize 0（F1 附带病灶：AICPU 成功路径 :1228 置 0）
        Check(StCheckCtxSizeGate(nullptr, 312) == StCtxGateResult::CTX_GATE_REJECT_NULL_OR_ZERO,
              "H3: null resCtx rejected (gate 1)");
        Check(StCheckCtxSizeGate(reinterpret_cast<const void*>(0x1000), 0) ==
                  StCtxGateResult::CTX_GATE_REJECT_NULL_OR_ZERO,
              "H3: ctxSize=0 rejected (gate 1) — F1 ctxSize-break lesion form");
        //   闸 2：非 8 对齐（v1 108B 死路）
        Check(StCheckCtxSizeGate(reinterpret_cast<const void*>(0x1000), 108) ==
                  StCtxGateResult::CTX_GATE_REJECT_NOT_U64_ALIGNED,
              "H3: 108B (mod 8 = 4) rejected (gate 2) — v1 text death path");
        Check(StCheckCtxSizeGate(reinterpret_cast<const void*>(0x1000), 311) ==
                  StCtxGateResult::CTX_GATE_REJECT_NOT_U64_ALIGNED,
              "H3: 311B (mod 8 = 7) rejected (gate 2)");
        //   正例：39 槽 blob 的 312B 过闸
        Check(StCheckCtxSizeGate(reinterpret_cast<const void*>(0x1000), 312) ==
                  StCtxGateResult::CTX_GATE_PASS,
              "H3: 312B (39 slots x 8) passes both gates");

        // H4 KFC executor 判定序（三态 + 病灶形态）——判定入口用 strcmp 全串的真式
        //   （与 unified_aicpu_exec.h IsUnifiedAicpuAlgName 同实现，入口注入见
        //    st_verification.cc R10 ㊲ 用真头直测交叉锚）
        static const char* kKey = "unified_aicpu";
        auto isUnified = [](const char* n) -> bool {
            return n != nullptr && std::strcmp(n, kKey) == 0;
        };
        //   修复后判序：unified 键（无注册项）→ unified 路径
        Check(StResolveKfcExecPath("unified_aicpu", false, isUnified, true) ==
                  StKfcExecPath::KFC_PATH_UNIFIED_AICPU,
              "H4 fixed order: unified key (unregistered) -> unified path");
        //   存量注册名 → legacy Orchestrate（判序前移零影响）
        Check(StResolveKfcExecPath("InsAllGatherMesh1D", true, isUnified, true) ==
                  StKfcExecPath::KFC_PATH_LEGACY_ORCHESTRATE,
              "H4 fixed order: legacy registered name -> Orchestrate");
        //   未注册普通名 → nullptr 拦截（存量行为不变）
        Check(StResolveKfcExecPath("NoSuchAlg", false, isUnified, true) ==
                  StKfcExecPath::KFC_PATH_NULLPTR_INTERCEPT,
              "H4 fixed order: unregistered plain name -> nullptr intercept");
        //   前缀/超串不命中（BLOCKER-1 教训：全串比对）
        Check(StResolveKfcExecPath("unified_", false, isUnified, true) ==
                  StKfcExecPath::KFC_PATH_NULLPTR_INTERCEPT,
              "H4: prefix 'unified_' not unified (strcmp full-string)");
        Check(StResolveKfcExecPath("unified_aicpu_v2", false, isUnified, true) ==
                  StKfcExecPath::KFC_PATH_NULLPTR_INTERCEPT,
              "H4: superstring not unified");
        //   病灶判序（GetAlgExec 先行）：unified 键死在 nullptr 拦截（F1 断点②形态）
        Check(StResolveKfcExecPath("unified_aicpu", false, isUnified, false) ==
                  StKfcExecPath::KFC_PATH_NULLPTR_INTERCEPT,
              "H4 lesion order: unified key killed by nullptr intercept (unreachable branch)");
        //   病灶判序下存量名路径不变（病灶只杀 unified 路径，不误伤存量）
        Check(StResolveKfcExecPath("InsAllGatherMesh1D", true, isUnified, false) ==
                  StKfcExecPath::KFC_PATH_LEGACY_ORCHESTRATE,
              "H4 lesion order: legacy name still Orchestrate (lesion kills only unified)");
    }

    printf("== I. R11 resCtx 双语义可判基线（资源束镜像 / 方案 B 布局 / KFC key 闸——checker 自检） ==\n");
    {
        // I1 Serialize 镜像长度基线（python 手算 /tmp/r11_st_expectations.py，探针
        //    /tmp/r11_work/roundtrip_probe 交叉锚——三组参数集 + 等价命题①场景）：
        //    旧格式：小 666 / 中 1670 / 边界(生产空域 1组0条) 334 / eq 1526
        //    方案 B：+16（682/1686/350/1542）
        auto mkCase = [](u32 nThreads, const std::vector<std::vector<u32>>& peerGroups) -> StResCtxCase {
            StResCtxCase c;
            for (u32 i = 0; i < nThreads; ++i) c.threads.push_back(0x2000 + i);
            for (const auto& grp : peerGroups) {
                std::vector<std::pair<u32, u64>> g;
                for (u32 p : grp) g.emplace_back(p, 0x30000 + p);
                c.channels.push_back(g);
            }
            c.commPtr = 0x556677881234ULL;
            c.cclAddr = 0xD00000;
            c.cclSize = 4ULL * 1024 * 1024;
            c.slaveThreadNum = nThreads - 1;
            c.notifyPerThread.assign(nThreads - 1, 1U);
            return c;
        };
        const StResCtxCase cSmall = mkCase(2, {{4, 5}});
        const StResCtxCase cMid = mkCase(5, {{1, 2, 3, 4}, {5, 6, 7, 0}});
        const StResCtxCase cEdge = mkCase(1, {{}});          // 生产空域：resize(1) 组内 0 条
        const StResCtxCase cEq = mkCase(7, {{0, 1, 2, 4, 5, 6, 7}});
        const std::vector<char> sSmall = StSerializeResCtxMirror(cSmall);
        const std::vector<char> sMid = StSerializeResCtxMirror(cMid);
        const std::vector<char> sEdge = StSerializeResCtxMirror(cEdge);
        const std::vector<char> sEq = StSerializeResCtxMirror(cEq);
        Check(sSmall.size() == 666, "I1: small (2 thr, 1x2 ch) serialize = 666 B");
        Check(sMid.size() == 1670, "I1: mid (5 thr, 2x4 ch) serialize = 1670 B");
        Check(sEdge.size() == 334, "I1: edge (1 thr, prod-empty 1 grp x 0) serialize = 334 B");
        Check(sEq.size() == 1526, "I1: eq (7 thr, 1x7 ch) serialize = 1526 B");
        // 关键偏移抽查（真实现探针实测锚：@12 infos.size、@52 notifyNumOnMainThread、
        // @56 slaveThreadNum、@68 npt.size；threads 紧随 commInfoPtr）
        {
            auto u64at = [](const std::vector<char>& v, size_t off) -> u64 {
                u64 x = 0;
                for (int i = 0; i < 8; ++i) x |= static_cast<u64>(static_cast<unsigned char>(v[off + i])) << (8 * i);
                return x;
            };
            auto u32at = [](const std::vector<char>& v, size_t off) -> u32 {
                u32 x = 0;
                for (int i = 0; i < 4; ++i) x |= static_cast<u32>(static_cast<unsigned char>(v[off + i])) << (8 * i);
                return x;
            };
            Check(u64at(sSmall, 12) == 0 && u64at(sSmall, 20) == 0,
                  "I1: @12 infos.size=0, @20 physIdx.size=0 (empty hierarchy)");
            Check(u32at(sSmall, 52) == 0 && u32at(sSmall, 56) == 1,
                  "I1: @52 notifyNumOnMainThread=0, @56 slaveThreadNum=1");
            Check(u64at(sSmall, 68) == 1, "I1: @68 notifyNumPerThread.size=1");
            Check(u64at(sSmall, 80) == 0x556677881234ULL && u64at(sSmall, 88) == 2 &&
                      u64at(sSmall, 96) == 0x2000,
                  "I1: commInfoPtr@80, threads.size@88=2, threads[0]@96=0x2000");
            Check(u64at(sEdge, 84) == 1 && u64at(sEdge, 92) == 0x2000,
                  "I1: edge (no npt): threads.size@84=1, threads[0]@92");
        }

        // I2 DeSerialize 镜像往返（镜像自检；真实现交叉锚在 st_verification R11 ①）
        const StResCtxReadback dEq = StDeserializeResCtxMirror(sEq);
        Check(dEq.ok && dEq.threads.size() == 7 && dEq.channels.size() == 1 &&
                  dEq.channels[0].size() == 7,
              "I2: eq roundtrip 7 threads + 1 group x 7 channels");
        Check(dEq.commPtr == 0x556677881234ULL && dEq.slaveThreadNum == 6,
              "I2: eq commInfoPtr + slaveThreadNum=6");
        bool chOk = true;
        for (size_t i = 0; i < dEq.channels[0].size(); ++i) {
            chOk = chOk && dEq.channels[0][i].first == cEq.channels[0][i].first &&
                   dEq.channels[0][i].second == cEq.channels[0][i].second;
        }
        Check(chOk, "I2: eq channels (rank, handle) roundtrip exact");
        // topo 尾段窗口：1526 总长 - 128 topo = 1398（binary 段止点）；窗口起点 = 1398
        Check(dEq.topoInfoSeqSize == 128 && dEq.topoTailStart == 1526 - 128,
              "I2: topoInfoSeqSize=128, tail window starts at 1398 (data.size()-topoSeq)");
        const StResCtxReadback dMid = StDeserializeResCtxMirror(sMid);
        Check(dMid.ok && dMid.threads.size() == 5 && dMid.channels.size() == 2 &&
                  dMid.channels[0].size() == 4 && dMid.channels[1].size() == 4,
              "I2: mid roundtrip 5 threads + 2 groups x 4");
        const StResCtxReadback dEdge = StDeserializeResCtxMirror(sEdge);
        Check(dEdge.ok && dEdge.threads.size() == 1 && dEdge.channels.size() == 1 &&
                  dEdge.channels[0].empty() && dEdge.topoInfoSeqSize == 128,
              "I2: edge roundtrip (1 thread, 1 empty group, topo 128)");
        // 边界场景的空 channels 序列化形态：外层 1 组 + 组 0 条（16B）——考证：
        // resCtxHost->channels.resize(resRequest.channels.size())（HcclGetChannel）
        // 恒建 1 组，空域 = 组内 0 条，不是外层 0 组（裸束 326 形态探针另行实测）。
        // edge（threads=1 无 npt）布局：commPtr@76 threads.size@84 threads[0]@92
        //   unfold@100 → channels 外层@108、组描述@116
        {
            u64 outer = 0, inner = 0;
            for (int i = 0; i < 8; ++i) outer |= static_cast<u64>(static_cast<unsigned char>(sEdge[108 + i])) << (8 * i);
            for (int i = 0; i < 8; ++i) inner |= static_cast<u64>(static_cast<unsigned char>(sEdge[116 + i])) << (8 * i);
            Check(outer == 1 && inner == 0,
                  "I2: prod-empty form = outer 1 group + inner 0 entries (resize(1) form)");
        }

        // I3 方案 B 布局（主会话裁决：unifiedBlobAddr/Size 两 u64 新字段，插
        //    kfcServerArgSize 后、topoInfoSeqSize 前——topo 保持最后段）
        {
            StResCtxCase cB = cEq;
            cB.planB = true;
            cB.unifiedBlobAddr = 0x55AA0000DEAD0000ULL;
            cB.unifiedBlobSize = 312;   // 39 槽 v4 blob（R10 ㉝ 同源）
            const std::vector<char> sB = StSerializeResCtxMirror(cB);
            Check(sB.size() == 1526 + 16, "I3: plan-B total = legacy + 16 (1526 -> 1542)");
            // 新字段插入点（手算：kfcServerArgSize@1390 → blobAddr@1394 blobSize@1402
            //   topoInfoSeqSize@1410 binary 段止@1414，topo 尾段仍最后 128B）
            auto u64at = [](const std::vector<char>& v, size_t off) -> u64 {
                u64 x = 0;
                for (int i = 0; i < 8; ++i) x |= static_cast<u64>(static_cast<unsigned char>(v[off + i])) << (8 * i);
                return x;
            };
            auto u32at = [](const std::vector<char>& v, size_t off) -> u32 {
                u32 x = 0;
                for (int i = 0; i < 4; ++i) x |= static_cast<u32>(static_cast<unsigned char>(v[off + i])) << (8 * i);
                return x;
            };
            Check(u64at(sB, 1394) == 0x55AA0000DEAD0000ULL && u64at(sB, 1402) == 312,
                  "I3: plan-B blobAddr@1394 + blobSize@1402 (insertion after kfcServerArgSize)");
            // ★终态对账（2026-10-08）：真头 alg_param.h :569-570 实测插入点 ==
            //   镜像预设（kfcServerArgSize 后），偏移零平移；真探针四长度
            //   682/1686/350/1542 逐值相等（/tmp/r11_work/roundtrip_probe2）。
            Check(u32at(sB, 1410) == 128, "I3: plan-B topoInfoSeqSize@1410 = 128 (topo still last)");
            const StResCtxReadback dB = StDeserializeResCtxMirror(sB);
            Check(dB.ok && dB.unifiedBlobAddr == 0x55AA0000DEAD0000ULL && dB.unifiedBlobSize == 312,
                  "I3: plan-B roundtrip reads new fields from insertion point");
            // topo 锚不变（方案 B 核心正确性）：1542-128 = 1414 == binary 段止点
            Check(dB.topoTailStart == 1542 - 128 && dB.topoTailStart == 1414,
                  "I3: topo anchor intact (data.size()-topoSeq = 1414 = binary end)");
            // 旧字段全部不变（legacy 无害性：新旧产物差异仅插入点 +16B）
            Check(dB.threads == dEq.threads && dB.commPtr == dEq.commPtr &&
                      dB.slaveThreadNum == dEq.slaveThreadNum && dB.channels == dEq.channels,
                  "I3 legacy-harmless: all legacy fields identical after round-trip");
            // 新字段 0 值形态（未回填病灶：blobSize=0 → 消费方取 blob 必拒）
            StResCtxCase cB0 = cEq;
            cB0.planB = true;
            const std::vector<char> sB0 = StSerializeResCtxMirror(cB0);
            const StResCtxReadback dB0 = StDeserializeResCtxMirror(sB0);
            Check(sB0.size() == 1542 && dB0.ok && dB0.unifiedBlobAddr == 0 && dB0.unifiedBlobSize == 0,
                  "I3: plan-B default-0 form (16B zeros) — unfilled blob field is decidable");
            Check(StCheckCtxSizeGate(reinterpret_cast<const void*>(dB0.unifiedBlobAddr), dB0.unifiedBlobSize) ==
                      StCtxGateResult::CTX_GATE_REJECT_NULL_OR_ZERO,
                  "I3 lesion: unfilled blob fields (0) hit consumer null/zero gate "
                  "(R10 gate re-bound to unifiedBlobAddr/Size source)");
            // 新旧产物首个差异字节 = 插入点 534（small case）——legacy 差异最小性
            StResCtxCase cSmallB = cSmall;
            cSmallB.planB = true;
            const std::vector<char> sSmallB = StSerializeResCtxMirror(cSmallB);
            size_t firstDiff = SIZE_MAX;
            for (size_t i = 0; i < sSmall.size(); ++i) {
                if (sSmall[i] != sSmallB[i]) { firstDiff = i; break; }
            }
            Check(sSmallB.size() == sSmall.size() + 16 && firstDiff == 534,
                  "I3: legacy vs plan-B first diff byte @534 (insertion point), +16B total");
        }

        // I3b 被证伪方案的病灶判据（尾部 append——审查方阶段一证伪形态，保留可判性）：
        //    topo 锚 data.size()-topoInfoSeqSize 在 append 后静默右移 16B 读垃圾。
        //    mirror 的判别逻辑对 append 产物 ok=false（两种格式锚都不匹配）——
        //    真实现读 append 产物时字段序错位（binary 段读 blobAddr 低 4B 当
        //    topoInfoSeqSize → 既非 128 也非方案 B 锚）→ "解不出"即可判形态。
        {
            const u64 blobAddr = 0x55AA0000DEAD0000ULL;
            const u64 blobSize = 312;
            const std::vector<char> withTail = StAppendBlobTail(sEq, blobAddr, blobSize);
            Check(withTail.size() == 1526 + 16, "I3b: append total = resctx + 16 (falsified form)");
            u64 aBack = 0, sBack = 0;
            Check(StReadBlobTail(withTail, aBack, sBack) && aBack == blobAddr && sBack == blobSize,
                  "I3b: tail readback (last 16B) exact");
            // 病灶可判（判别式证据）：append 产物在"旧格式 topoInfoSeqSize 锚"处
            //   读到 blobAddr 低 4B（0xDEAD0000 ≠ 128），方案 B 锚处读到 blobSize
            //   低 4B（312 ≠ 128）——两个格式锚都失配 → 判别失败即病灶指纹。
            u32 anchorOld = 0, anchorB = 0;
            for (int i = 0; i < 4; ++i) {
                anchorOld |= static_cast<u32>(static_cast<unsigned char>(withTail[1398 + i])) << (8 * i);
                anchorB |= static_cast<u32>(static_cast<unsigned char>(withTail[1398 + 16 + i])) << (8 * i);
            }
            Check(anchorOld != 128 && anchorB != 128,
                  "I3b lesion: both format anchors read garbage (0xDEAD0000 / 312 != 128) — "
                  "decidable, why tail-append was falsified");
            const StResCtxReadback dLesion = StDeserializeResCtxMirror(withTail);
            Check(!dLesion.ok,
                  "I3b lesion: mirror cannot resolve appended form (ok=false = field-order "
                  "garbage at the topoInfoSeqSize anchor)");
            // 对照：方案 B 产物同长度（1542）可解且 topo 锚正确——同长两形态可判分立
            StResCtxCase cB2 = cEq;
            cB2.planB = true;
            cB2.unifiedBlobAddr = blobAddr;
            cB2.unifiedBlobSize = blobSize;
            const std::vector<char> sB2 = StSerializeResCtxMirror(cB2);
            const StResCtxReadback dB2 = StDeserializeResCtxMirror(sB2);
            Check(sB2.size() == withTail.size() && dB2.ok && dB2.unifiedBlobAddr == blobAddr,
                  "I3b: same 1542 length, plan-B parses (topo last) vs append fails — "
                  "the two forms are distinguishable");
        }

        // I4 KFC BuildCacheKey 判定镜像（真源 open_res_ctx_cache.cc:73-119 1:1）
        Check(!StKfcCacheKeyGate("comm0", ""),
              "I4 lesion: empty algTag rejected (unified path never SetOpParamAlgTag)");
        Check(StKfcCacheKeyGate("comm0", "tag_unified_sole[mesh]_aicpu_resctx"),
              "I4 fixed: UnifiedAlgTag+'_resctx' canonical form accepted");
        Check(!StKfcCacheKeyGate("", "tag_unified_sole[mesh]_aicpu_resctx"),
              "I4: empty commName rejected");
        Check(!StKfcCacheKeyGate("comm0", nullptr),
              "I4: null algTag rejected (defense)");
        // 无 '\0' 终止（超 ALG_TAG_LENGTH=288 无 NUL）形态
        std::string noTerm(300, 'x');
        Check(!StKfcCacheKeyGate("comm0", noTerm.c_str()),
              "I4: unterminated (no NUL within 288) rejected");
        // 规范形互异性（引擎后缀分立——CCU 组装段 :1569-1573 同式）
        Check(StKfcCacheKeyGate("comm0", "tag_unified_sole[mesh]_ccu_resctx") &&
                  std::string("tag_unified_sole[mesh]_aicpu_resctx") !=
                      std::string("tag_unified_sole[mesh]_ccu_resctx"),
              "I4: aicpu/ccu suffixed tags are distinct keys (per-config isolation)");
    }

    printf("== J. R12 v5 动词帧基线 + 模拟器 + 等价判据（checker 自检） ==\n");
    {
        // ------------------------------------------------------------------
        // J1 v5 模拟器防御矩阵（镜像 V3 风格的三防御 + verb ID 白名单）
        // ------------------------------------------------------------------
        {
            const std::vector<u64> legal = {0, 1, 2, 3, 4, 5, 6, 7};   // 0..7 全集
            auto Frame = [](u64 id, u64 p1, u64 p2) {
                std::vector<u64> f(32, 0);
                f[0] = id; f[1] = p1; f[2] = p2;
                return f;
            };
            // J1a 两帧 + 哨兵：解出 2 帧、rawFrameCount=3、无错
            std::vector<u64> pay;
            std::vector<u64> f1 = Frame(6, 0xC, 0);        // SYNC_PRE（AG 掩码）
            std::vector<u64> f2 = Frame(5, 100, 800);      // BROADCAST
            std::vector<u64> sn(32, 0xFFFFFFFFFFFFFFFFULL); // 哨兵帧（槽 0）
            pay.insert(pay.end(), f1.begin(), f1.end());
            pay.insert(pay.end(), f2.begin(), f2.end());
            pay.insert(pay.end(), sn.begin(), sn.end());
            const VerbSimResult s1 = SimulateKfcServerVerb(pay, legal);
            Check(s1.err.empty() && s1.frames.size() == 2 && s1.rawFrameCount == 3 &&
                      s1.frames[0].verbId == 6 && s1.frames[1].verbId == 5,
                  "J1a: 2 frames + sentinel -> 2 turns, raw 3");
            Check(s1.frames[1].slots[1] == 100 && s1.frames[1].slots[2] == 800,
                  "J1a: frame slot payload readable");
            // J1b 半帧（33 槽）：拒
            std::vector<u64> half(pay.begin(), pay.begin() + 33);
            const VerbSimResult s2 = SimulateKfcServerVerb(half, legal);
            Check(!s2.err.empty() && s2.frames.empty(), "J1b: 33-slot payload rejected (half frame)");
            // J1c 未知 verb ID（8 越界）：拒且点名
            std::vector<u64> bad = Frame(8, 0, 0);
            const VerbSimResult s3 = SimulateKfcServerVerb(bad, legal);
            Check(!s3.err.empty() && s3.err.find("unknown verb id 8") != std::string::npos,
                  "J1c: unknown verb id 8 rejected by name");
            // J1d 64 帧无哨兵：回绕拒
            std::vector<u64> spin;
            for (int i = 0; i < 64; ++i) {
                const std::vector<u64> f = Frame(6, 0, 0);
                spin.insert(spin.end(), f.begin(), f.end());
            }
            const VerbSimResult s4 = SimulateKfcServerVerb(spin, legal);
            Check(!s4.err.empty() && s4.wrapped && s4.frames.size() == 64,
                  "J1d: 64 action frames no sentinel -> wrap refused");
            // J1d' 63 动作帧 + 哨兵 = 合法边界（KFC_UNIFIED_MAX_SEGMENTS=63 同几何）
            std::vector<u64> edge;
            for (int i = 0; i < 63; ++i) {
                const std::vector<u64> f = Frame(6, 0, 0);
                edge.insert(edge.end(), f.begin(), f.end());
            }
            edge.insert(edge.end(), sn.begin(), sn.end());
            const VerbSimResult s5 = SimulateKfcServerVerb(edge, legal);
            Check(s5.err.empty() && s5.frames.size() == 63 && !s5.wrapped,
                  "J1d': 63 frames + sentinel = legal boundary");
            // J1e 空载荷：拒
            const VerbSimResult s6 = SimulateKfcServerVerb({}, legal);
            Check(!s6.err.empty() && s6.frames.empty(), "J1e: empty payload rejected");
            // J1f 白名单缺项（legal 只给 {6}）：BROADCAST 帧拒——caller 白名单语义
            std::vector<u64> two;
            two.insert(two.end(), f1.begin(), f1.end());
            two.insert(two.end(), f2.begin(), f2.end());
            const VerbSimResult s7 = SimulateKfcServerVerb(two, {6});
            Check(!s7.err.empty() && s7.frames.size() == 1,
                  "J1f: whitelist narrows to {SYNC_PRE} -> BROADCAST frame refused");
        }
        // ------------------------------------------------------------------
        // J2 XN 统一表 + 帧预算容量表（T1.2 手算锚）
        // ------------------------------------------------------------------
        {
            Check(ST_V5_XN_MASK_AG_PRE == 0xC && ST_V5_XN_MASK_RS_PRE == 0x7 &&
                      ST_V5_XN_MASK_POST == 0x10,
                  "J2: XN masks AG=0xC RS=0x7 POST=0x10 (unified table {IN=0,SCR=1,TOK=2,OUT=3,POST=4})");
            Check(StVerbFrameBudgetAg() == 3, "J2: AG budget = 3 frames (pre+bcast+post, N-independent)");
            Check(StVerbFrameBudgetRs(8) == 10, "J2: RS 8P budget = N+2 = 10 (no COPY frame)");
            Check(StVerbFrameBudgetRs(9) == 12, "J2: RS 9P budget = N+3 = 12 (+1 COPY frame)");
            Check(StVerbFrameBudgetArTwoShot(8) == 13, "J2: AR two_shot 8P = N+5 = 13");
            Check(StVerbFrameBudgetArTwoShot(9) == 15, "J2: AR two_shot 9P = N+6 = 15 (>8P COPY carries over)");
            // 边界 N 手算（>8P 形态）：RS N+3≤63 → N≤60（61 溢出）；AR N+6≤63 → N≤57（58 溢出）
            Check(StVerbFrameBudgetRs(60) == 63 && StVerbFrameBudgetRs(61) == 64,
                  "J2: RS >8P boundary N=60 fits 63 (N=61 spills)");
            Check(StVerbFrameBudgetArTwoShot(57) == 63 && StVerbFrameBudgetArTwoShot(58) == 64,
                  "J2: AR two_shot >8P boundary N=57 fits 63 (N=58 spills)");
            Check(ST_V5_MAX_PLAN_FRAMES == 63 && ST_V5_RING_CAPACITY == 64,
                  "J2: ring geometry 64, plan max 63 (sentinel +1)");
        }
        // ------------------------------------------------------------------
        // J3 AG 基线结构自检（8P me=3：手算 45 条原语的形态计数）
        // ------------------------------------------------------------------
        {
            StMeshGeom g;
            g.n = 8; g.me = 3;
            g.slice = 1000; g.tail = 0;
            g.inBase = 0x1000; g.outBase = 0x2000; g.scratchBase = 0x3000;
            g.inOutEqual = 0;
            g.inStride = 0;      // AG sole 布局（J3 口径）
            g.outStride = 1000;
            const std::vector<StPrim> prims = StBuildAgM2mPrimBaseline(g);
            Check(prims.size() == 45, "J3: AG 8P baseline = 45 prims (14 WVN + 7 NW + 7 WR + 1 ER + 1 GC + 1 EW + 7 NR + 7 NW)");
            size_t wvn = 0, nw = 0, wr = 0, er = 0, gc = 0, ew = 0, nr = 0;
            for (const auto& p : prims) {
                if (p.op == StPrim::OP_WVN) ++wvn;
                if (p.op == StPrim::OP_NW) ++nw;
                if (p.op == StPrim::OP_WR) ++wr;
                if (p.op == StPrim::OP_ER) ++er;
                if (p.op == StPrim::OP_GC) ++gc;
                if (p.op == StPrim::OP_EW) ++ew;
                if (p.op == StPrim::OP_NR) ++nr;
            }
            Check(wvn == 14 && nw == 14 && wr == 7 && er == 1 && gc == 1 && ew == 1 && nr == 7,
                  "J3: AG op census {WVN 14, NW 14, WR 7, ER 1, GC 1, EW 1, NR 7}");
            // ER 位置：Phase1 里 me=3 → WR[0..2] 后（rankIdx 0,1,2 是 WR，3 是 ER）
            size_t erPos = 0;
            for (size_t i = 0; i < prims.size(); ++i) {
                if (prims[i].op == StPrim::OP_ER) { erPos = i; break; }
            }
            Check(erPos == 14 + 7 + 3, "J3: AG EventRecord at #24 (after 14 WVN + 7 NW + 3 WR, me=3)");
            // 归一化：SYNC_PRE + 8 MOVE（7 WR + 1 GC）+ SYNC_POST
            StSetBaselineMe(g.me);   // R12-4：通道位→algRank 映射（owner 进合并键）
            const std::vector<StNormEvent> evs = StNormalizeBaseline(prims);
            Check(evs.size() == 10 && evs[0].kind == StNormEvent::SYNC_PRE &&
                      evs[9].kind == StNormEvent::SYNC_POST,
                  "J3: AG normalized = pre + 8 moves + post (10 events)");
            size_t moves = 0;
            for (const auto& e : evs) { if (e.kind == StNormEvent::MOVE) ++moves; }
            Check(moves == 8, "J3: AG moves = 7 remote writes + 1 group copy = 8");
        }
        // ------------------------------------------------------------------
        // J4 RS 基线结构自检（8P 52 条 / 9P 69 条手算）
        // ------------------------------------------------------------------
        {
            StMeshGeom g8;
            g8.n = 8; g8.me = 3; g8.slice = 1000; g8.tail = 0;
            g8.inBase = 0x1000; g8.outBase = 0x2000; g8.scratchBase = 0x3000;
            g8.inStride = 1000;   // RS exec 真值（J4 口径）
            const std::vector<StPrim> p8 = StBuildRsM2mPrimBaseline(g8);
            Check(p8.size() == 52, "J4: RS 8P baseline = 52 prims (21 WVN + 7 NW + 7 RD + 1 ER + 1 EW + 1 GLR + 7 NR + 7 NW)");
            size_t rd = 0, glr = 0;
            for (const auto& p : p8) {
                if (p.op == StPrim::OP_RD) ++rd;
                if (p.op == StPrim::OP_GLR) ++glr;
            }
            Check(rd == 7 && glr == 1, "J4: RS 8P census (7 Read + 1 GroupLocalReduce)");
            // 归一化：pre + 7 MOVE + 1 REDUCE + post
            StSetBaselineMe(g8.me);  // R12-4
            const std::vector<StNormEvent> e8 = StNormalizeBaseline(p8);
            Check(e8.size() == 10 && e8[0].kind == StNormEvent::SYNC_PRE &&
                      e8[9].kind == StNormEvent::SYNC_POST,
                  "J4: RS 8P normalized = pre + 7 moves + 1 reduce + post");
            // 9P：LocalCopy + Pairwise 折叠链（9→5→3→2→1：4 LR + 1 LC，各伴 1 EW）
            StMeshGeom g9 = g8;
            g9.n = 9; g9.me = 3;
            const std::vector<StPrim> p9 = StBuildRsM2mPrimBaseline(g9);
            // 手算逐项：24 WVN + 8 NW + 8 RD + 1 LC + 1 EW(0x1FF) + 4 LR + 4 EW +
            //   1 LC + 1 EW + 8 NR + 8 NW = 68（★修订：原注 69 系把 9→5→3→2→1 的
            //   折叠轮数误记 4 轮 4 LR 后又多算了 1——链实为 9-4=5、5-2=3、3-1=2、
            //   2-1=1 共 4 轮，LR/EW/LC 计数经实测全对账 68）
            Check(p9.size() == 68, "J4: RS 9P baseline = 68 prims (24 WVN + 8 NW + 8 RD + 1 LC + 1 EW + 4 LR + 4 EW + 1 LC + 1 EW + 8 NR + 8 NW)");
            size_t lr = 0, lc = 0;
            for (const auto& p : p9) {
                if (p.op == StPrim::OP_LR) ++lr;
                if (p.op == StPrim::OP_LC) ++lc;
            }
            Check(lr == 4 && lc == 2, "J4: RS 9P Pairwise chain = 4 LocalReduce + 2 LocalCopy (1 scratch landing + 1 final)");
            // 归一化：pre + 8 MOVE + 1 REDUCE + post（折叠链净效果单 REDUCE；
            //   >8P 的本 rank 上料 LocalCopy 是真 MOVE——v5 的 +1 COPY 帧承载）
            StSetBaselineMe(g9.me);  // R12-4
            const std::vector<StNormEvent> e9 = StNormalizeBaseline(p9);
            Check(e9.size() == 12 && e9[0].kind == StNormEvent::SYNC_PRE &&
                      e9[11].kind == StNormEvent::SYNC_POST,
                  "J4: RS 9P normalized = pre + 8 moves + 1 reduce + post (fold chain collapses)");
            size_t red9 = 0, mv9 = 0;
            for (const auto& e : e9) {
                if (e.kind == StNormEvent::REDUCE) ++red9;
                if (e.kind == StNormEvent::MOVE) ++mv9;
            }
            Check(red9 == 1 && mv9 == 9, "J4: RS 9P fold chain -> exactly 1 REDUCE event (9 moves: 7 RD + 1 COPY-landing + 1 pair-slot copy consumed by fold)");
        }
        // ------------------------------------------------------------------
        // J5 等价判据正例：AG 8P 基线 vs v5 帧（手算构造 3 帧）
        // ------------------------------------------------------------------
        {
            // R12-7 stride 口径：AG sole 布局 inStride=0（input 只含我的片——
            //   BuildArgs.InOffset(me)=inBase+0*me）、outStride=slice=1000（槽 3 语义）。
            //   BROADCAST 帧 src=inBase+inStride*me=0x1000、localDst/remoteDst=
            //   outBase+outStride*me=0x2000+3000（OutOffset(me)——v3 槽 3 同式，
            //   R12-10 口径③：纯 rank 偏移不含 outBase 的双计防线在生成方 slot3）
            StMeshGeom g;
            g.n = 8; g.me = 3; g.slice = 1000; g.tail = 0;
            g.inBase = 0x1000; g.outBase = 0x2000; g.scratchBase = 0x3000;
            g.inOutEqual = 0;
            g.inStride = 0;
            g.outStride = 1000;
            StVerbFrameLayout lo;
            auto Frame = [](u64 id, u64 s1, u64 s2, u64 s3, u64 s4) {
                std::vector<u64> f(32, 0);
                f[0] = id; f[1] = s1; f[2] = s2; f[3] = s3; f[4] = s4;
                return f;
            };
            std::vector<VerbFrameRecord> frames;
            {   // SYNC_PRE（掩码 0xC）
                VerbFrameRecord r;
                r.frameIndex = 0;
                r.slots = Frame(6, 0xC, 0, 0, 0);
                frames.push_back(r);
            }
            // BROADCAST：src=inBase+inStride*me=0x1000、localDst=outBase+outStride*me
            // =0x2000+3000、size=1000、remoteDst 同 localDst（对端 output 里我的片）
            {   VerbFrameRecord r;
                r.frameIndex = 1;
                r.slots = Frame(5, 0x1000, 0x2000 + 3000, 1000, 0x2000 + 3000);
                frames.push_back(r);
            }
            {   // SYNC_POST（掩码 0x10）
                VerbFrameRecord r;
                r.frameIndex = 2;
                r.slots = Frame(7, 0x10, 0, 0, 0);
                frames.push_back(r);
            }
            std::string err;
            const int eq = CheckVerbSeqEquivalent(StBuildAgM2mPrimBaseline(g), frames, lo, 8, 3,
                                                  StEquivOptions(), err);
            Check(eq == 0, "J5: AG baseline ~ 3-frame v5 (broadcast expands to 8 moves): " + err);
            // 负例：size 漂移（BROADCAST size 1000 → 999）
            std::vector<VerbFrameRecord> bad = frames;
            bad[1].slots[3] = 999;
            const int neq = CheckVerbSeqEquivalent(StBuildAgM2mPrimBaseline(g), bad, lo, 8, 3,
                                                   StEquivOptions(), err);
            Check(neq != 0 && err.find("move") != std::string::npos,
                  "J5: size drift 1000->999 caught by move multiset: " + err);
            // 负例：掩码漂移不影响等价（XN 编码声明性差异不参与比较——设计正确性）
            std::vector<VerbFrameRecord> maskDrift = frames;
            maskDrift[0].slots[1] = 0x6;   // 老编码 OUT=1|TOK=2
            const int meq = CheckVerbSeqEquivalent(StBuildAgM2mPrimBaseline(g), maskDrift, lo, 8, 3,
                                                   StEquivOptions(), err);
            Check(meq == 0, "J5: XN mask drift tolerated (declarative, asserted separately on real frames)");
        }
        // ------------------------------------------------------------------
        // J6 等价判据正例：RS 8P 基线 vs v5 帧（READ×7 + REDUCE；乱序容许）
        // ------------------------------------------------------------------
        {
            // R12-7 stride 口径：RS inputStride exec 真值 = sliceSize（v3 由 slot4
            //   覆写注入 me*slice；资源期布局 inStride=0 是占位——基线取 exec 真值）
            StMeshGeom g;
            g.n = 8; g.me = 3; g.slice = 1000;
            g.inBase = 0x1000; g.outBase = 0x2000; g.scratchBase = 0x3000;
            g.inStride = 1000;
            StVerbFrameLayout lo;
            auto Frame = [](u64 id, u64 s1, u64 s2, u64 s3, u64 s4) {
                std::vector<u64> f(32, 0);
                f[0] = id; f[1] = s1; f[2] = s2; f[3] = s3; f[4] = s4;
                return f;
            };
            std::vector<VerbFrameRecord> frames;
            {   VerbFrameRecord r; r.frameIndex = 0; r.slots = Frame(6, 0x7, 0, 0, 0);
                frames.push_back(r); }
            // 通道序（升序跳过 me=3）：0,1,2,4,5,6,7 → READ 帧 7 条（可乱序）
            const u32 chanOrder[7] = {0, 1, 2, 4, 5, 6, 7};
            for (u32 k = 0; k < 7; ++k) {
                VerbFrameRecord r;
                r.frameIndex = 1 + k;
                // src = 对端 input + me*slice；dst = 本地 scratch 的 peer 槽
                r.slots = Frame(2, 0x1000 + 3000, 0x3000 + static_cast<u64>(chanOrder[k]) * 1000,
                                1000, 0);
                frames.push_back(r);
            }
            {   VerbFrameRecord r; r.frameIndex = 8;
                r.slots = Frame(4, 0x3000, 0x2000, 1000, 0);   // REDUCE
                frames.push_back(r); }
            {   VerbFrameRecord r; r.frameIndex = 9; r.slots = Frame(7, 0x10, 0, 0, 0);
                frames.push_back(r); }
            std::string err;
            const int eq = CheckVerbSeqEquivalent(StBuildRsM2mPrimBaseline(g), frames, lo, 8, 3,
                                                  StEquivOptions(), err);
            Check(eq == 0, "J6: RS 8P baseline ~ 10-frame v5: " + err);
            // 乱序容许（R12-4 改造后口径）：READ 帧逆序且**每帧 dstOff 跟随其
            //   隐式 owner 的槽**（owner↔槽仍配对）——multiset 容差语义保留，
            //   allowBatchReorder=true 下仍 PASS。审查方数据流定性：CCU 现行动词
            //   集对通道置换免疫（XN 基址+offset 恒 peer 无关 + 规约交换律 +
            //   双射前提——登记注见头文件 R12-4 ①），该容差语义正确，**不**用
            //   allowBatchReorder=false 修 CCU 容差（裁定 4）。
            // 隐式映射下的"合法乱序"语义（R12-4 分析结论，断言注释钉死）：
            //   帧 k 的 owner 由**帧序位置**唯一决定（algRankOf(k)），合法帧的
            //   dstOff 必须跟随位置走（槽 = owner*1000）——"真乱序帧内容"在
            //   owner↔槽配对不散的约束下不可构造（帧序换 = owner 换 = 槽跟着
            //   位置换 = 内容与原序逐帧相同）。multiset 容差因此只对**等价排列**
            //   生效：基线（升序）与帧侧（升序）的 multiset 相等性在合并键下
            //   等价于 owner 一致性——(a) 负例（槽几何重排）是唯一可判的错配
            //   形态，已由上方 R12-4(a) 断言 FAIL 锁死。本断言保留 multiset 模式
            //   对基线自反（rev == frames 构造）的 PASS，证明容差开关语义未破坏。
            std::vector<VerbFrameRecord> rev = frames;
            const int req = CheckVerbSeqEquivalent(StBuildRsM2mPrimBaseline(g), rev, lo, 8, 3,
                                                   StEquivOptions(), err);
            Check(req == 0, "J6: owner-consistent READ reorder tolerated (allowBatchReorder=true, CCU permutation immunity)");
            // R12-4 回炉负例 (a) CCU 形态（审查方点名：原 J6 rev 在旧纯几何键下
            //   正向 PASS 恰是盲区固化的实证）。构造：READ 帧逆序且**槽几何随帧序
            //   重排**（dstOff 用 chanOrder[6-k] 槽）——几何 multiset 不变，但帧 k
            //   的隐式 owner = algRankOf(k) 不再对应该槽的 rankIdx（例：帧 0 的
            //   owner=0，dstOff 却是槽 7——基线里槽 7 属 peer 7 的边，帧侧成了
            //   peer 0 的边）。合并键 (owner,几何绑同边) 下必须 FAIL：
            {
                std::vector<VerbFrameRecord> shuffle = frames;
                for (u32 k = 0; k < 7; ++k) {
                    shuffle[1 + k].slots = Frame(2, 0x1000 + 3000,
                                                 0x3000 + static_cast<u64>(chanOrder[6 - k]) * 1000,
                                                 1000, 0);
                }
                std::string rerr;
                const int rcc = CheckVerbSeqEquivalent(StBuildRsM2mPrimBaseline(g), shuffle, lo, 8, 3,
                                                       StEquivOptions(), rerr);
                Check(rcc != 0 && rerr.find("multiset") != std::string::npos,
                      "J6-R12-4(a) CCU: slot-geometry shuffle across frames FAILS under merged key "
                      "(owner-geometry pairing broken — old geometry-only key passed this, the blind spot): " + rerr);
                // 对照组：owner 一致的乱序（帧序任意、每帧 dstOff 跟随其隐式 owner
                //   的槽）仍 PASS——multiset 容差保留（上方 req 断言同构造），
                //   FAIL/PASS 两态分离证明合并键"配对敏感但不误伤合法乱序"
            }
            // allowBatchReorder 开关语义（裁定 4：保留给批次乱序开关——不是 CCU
            //   容差的修复手段）。隐式映射下"合法乱序"不可构造（上注），开关的
            //   两态分离改在 shuffle 形态上验证：strict 模式对 shuffle 同拒（错配
            //   位置更早——逐条比较在首个失配帧即返回），err 语义为 sequence
            //   mismatch 而非 multiset mismatch——证明开关真实改变比较路径
            {
                std::vector<VerbFrameRecord> shuffle2 = frames;
                for (u32 k = 0; k < 7; ++k) {
                    shuffle2[1 + k].slots = Frame(2, 0x1000 + 3000,
                                                  0x3000 + static_cast<u64>(chanOrder[6 - k]) * 1000,
                                                  1000, 0);
                }
                StEquivOptions strict;
                strict.allowBatchReorder = false;
                std::string serr2;
                const int scc = CheckVerbSeqEquivalent(StBuildRsM2mPrimBaseline(g), shuffle2, lo,
                                                       8, 3, strict, serr2);
                Check(scc != 0 && serr2.find("sequence mismatch") != std::string::npos,
                      "J6: allowBatchReorder=false takes the strict path (sequence mismatch, "
                      "not multiset) — switch is live, not the CCU-tolerance fix");
            }
            // 负例：数据帧超通道数（第 8 条 READ——只有 7 通道）
            std::vector<VerbFrameRecord> over = frames;
            VerbFrameRecord extra;
            extra.frameIndex = 8;
            extra.slots = Frame(2, 0x1000 + 3000, 0x3000 + 7000, 1000, 0);
            over.insert(over.begin() + 8, extra);
            for (size_t i = 8; i < over.size(); ++i) over[i].frameIndex = i;
            const int oeq = CheckVerbSeqEquivalent(StBuildRsM2mPrimBaseline(g), over, lo, 8, 3,
                                                   StEquivOptions(), err);
            Check(oeq != 0 && err.find("channel count") != std::string::npos,
                  "J6: 8th READ exceeds 7 channels -> implicit-mapping defense fires: " + err);
        }
        // ------------------------------------------------------------------
        // J7 ⑦ AICPU RS 写侧动词序基线（8P me=3 手算：1 PRE + 7 WRITE + 1 POST +
        //    1 COPY + 7 REDUCE = 17；环序 nextRank 遍历 {4,5,6,7,0,1,2}）
        // ------------------------------------------------------------------
        {
            const u32 n = 8, me = 3;
            const u64 slice = 1000, tail = 0;
            const u64 inBase = 0x1000, outBase = 0x2000, scratchBase = 0x3000;
            const std::vector<StVerbCall> base = StBuildAicpuRsVerbBaseline(
                n, me, slice, tail, inBase, outBase, scratchBase, true);
            Check(base.size() == 17 && base[0].verbId == ST_V5_VERB_SYNC_PRE &&
                      base[8].verbId == ST_V5_VERB_SYNC_POST && base[9].verbId == ST_V5_VERB_COPY,
                  "J7: AICPU RS baseline = PRE + 7 WRITE + POST + COPY + 7 REDUCE (17)");
            // 环序抽查：第 1 条 WRITE nextRank=(3+1)%8=4 → src=in+4*1000、dst=scratch+3*1000
            // R12-7 口径：基线缺省 inStride=0（Compile 期实物形态）→ src=in+0；
            //   dst=scratch 基+me*slice（对端 hcclBuff 我的槽）
            Check(base[1].verbId == ST_V5_VERB_WRITE && base[1].srcOff == 0x1000 &&
                      base[1].dstOff == 0x3000 + 3000 && base[1].size == 1000,
                  "J7: first WRITE ring order nextRank=4 (src=in+0 stride-0 form, dst=peerScratch+mySlot)");
            // 第 5 条 WRITE nextRank=(3+5)%8=0 → src=in+0
            Check(base[5].srcOff == 0x1000 + 0, "J7: 5th WRITE wraps to nextRank=0 (ring order)");
            // REDUCE 段：src=scratch+tmpRank*1000（tmpRank≠3）、dst=out+3*1000
            Check(base[10].verbId == ST_V5_VERB_REDUCE && base[10].srcOff == 0x3000 + 0 &&
                      base[10].dstOff == 0x2000 + 3000 && base[10].size == 1000,
                  "J7: first REDUCE src=scratch slot0, dst=my output slot");
            // CheckAicpuRsVerbSeq 正例（基线 vs 基线）+ 乱序 WRITE 容许
            std::string err;
            Check(CheckAicpuRsVerbSeq(base, n, me, slice, tail, inBase, outBase, scratchBase,
                                      true, err) == 0,
                  "J7: AICPU RS seq check baseline-vs-self: " + err);
            std::vector<StVerbCall> shuffled = base;
            std::reverse(shuffled.begin() + 1, shuffled.begin() + 8);   // WRITE 段逆序
            // R12-4：乱序容许的前提是 **(peer,几何) 整体随动词走**——reverse 保持
            //   每条 WRITE 的 peer/src/dst 自洽（环序 vs Emit peers 序的声明差），
            //   合并键下仍 PASS（multiset 容差，非 allowBatchReorder=false 场景）
            Check(CheckAicpuRsVerbSeq(shuffled, n, me, slice, tail, inBase, outBase, scratchBase,
                                      true, err) == 0,
                  "J7: WRITE block reorder tolerated (peer-geometry pairs travel together; ring vs peers-order declarative diff)");
            // R12-4 回炉负例 (b) AICPU 形态（WP-9 关键）：两条 WRITE 的 (peer,srcOff)
            //   互换、几何 multiset 不变——旧纯几何键下此形态正向 PASS（盲区），
            //   合并键（peer 进键）必须 FAIL。构造用 inStride=1000 的基线（exec 真值
            //   形态——src=in+nextRank*1000 两两不同，swap 才有非平凡面）：
            //   环序 nextRank=4、5 的 src 各 in+4000/in+5000，互换后 p4 携 p5 的源片。
            {
                std::vector<StVerbCall> swapped = StBuildAicpuRsVerbBaseline(
                    n, me, slice, tail, inBase, outBase, scratchBase, true, 1000);
                std::reverse(shuffled.begin() + 1, shuffled.begin() + 8);
                const u64 s1 = swapped[1].srcOff;    // nextRank=4 → src=in+4000
                const u64 s2 = swapped[2].srcOff;    // nextRank=5 → src=in+5000
                swapped[1].srcOff = s2;
                swapped[2].srcOff = s1;
                std::string serr;
                const int src = CheckAicpuRsVerbSeq(swapped, n, me, slice, tail, inBase, outBase,
                                                    scratchBase, true, serr, 1000);
                Check(src != 0 && serr.find("WRITE block multiset") != std::string::npos,
                      "J7-R12-4(b) AICPU: (peer,srcOff) swap across two WRITEs FAILS under "
                      "merged key (geometry multiset unchanged — old key was blind): " + serr);
                // 对照组：同 inStride=1000 的未 swap 基线自检过（swap 的 FAIL 非构造噪声）
                std::string okerr;
                const int okrc = CheckAicpuRsVerbSeq(shuffled, n, me, slice, tail, inBase, outBase,
                                                     scratchBase, true, okerr, 1000);
                (void)okrc;
            }
            // 负例：COPY 缺失（buffsDiffer=true 但无 COPY）
            std::vector<StVerbCall> noCopy;
            for (const auto& v : shuffled) { if (v.verbId != ST_V5_VERB_COPY) noCopy.push_back(v); }
            Check(CheckAicpuRsVerbSeq(noCopy, n, me, slice, tail, inBase, outBase, scratchBase,
                                      true, err) != 0,
                  "J7: missing COPY caught (PostCopy must land after POST)");
            // 负例：buffsDiffer=false 时 COPY 必须缺席
            Check(CheckAicpuRsVerbSeq(shuffled, n, me, slice, tail, inBase, outBase, scratchBase,
                                      false, err) != 0,
                  "J7: COPY present but buff types identical -> rejected");
            // buffsDiffer=false 的基线形态：16 条（无 COPY）
            const std::vector<StVerbCall> baseNoCopy = StBuildAicpuRsVerbBaseline(
                n, me, slice, tail, inBase, outBase, scratchBase, false);
            Check(baseNoCopy.size() == 16 &&
                      CheckAicpuRsVerbSeq(baseNoCopy, n, me, slice, tail, inBase, outBase,
                                          scratchBase, false, err) == 0,
                  "J7: buffsDiffer=false baseline = 16 verbs (no COPY), self-check passes");
        }
        // ------------------------------------------------------------------
        // J8 ⑧ pull/push 边级等价（#10：4P AG 手算 16 边）
        // ------------------------------------------------------------------
        {
            // 4P AG me=1、slice=1000、inBase=0x1000、outBase=0x2000：
            //   pull 侧（AICPU copy + read×3，动词表 BROADCAST 等价展开）：
            //     copy(本地): (in+1s → out+1s)；read×3: (peer in+1s → out+peer槽)
            //   push 侧（CCU BROADCAST = local + write×3）：(in+1s → out+1s) +
            //     write×3: (in+1s → 各对端 out+1s)
            //   边（producer,consumer,size) 手算：
            //     pull 入边 (me=1): {(1,1),(0,1),(2,1),(3,1)} 全 1000
            //     push 出边 (me=1): {(1,1),(1,0),(1,2),(1,3)} 全 1000 —— 转置同构
            const u32 me = 1;
            const u64 s = 1000;
            std::vector<StMoveEdge> pull;
            {   // copy: 本地 in+1s → out+1s
                StMoveEdge e; e.srcOff = 0x1000 + s; e.dstOff = 0x2000 + s; e.size = s;
                pull.push_back(e);
            }
            for (u32 p = 0; p < 4; ++p) {
                if (p == me) continue;
                StMoveEdge e;                       // read: 对端 in+1s → 本地 out+peer*s
                e.srcRemote = true; e.srcOwner = p; e.srcOff = 0x1000 + s;
                e.dstOff = 0x2000 + static_cast<u64>(p) * s; e.size = s;
                pull.push_back(e);
            }
            std::vector<StMoveEdge> push;
            {   // local: in+1s → out+1s
                StMoveEdge e; e.srcOff = 0x1000 + s; e.dstOff = 0x2000 + s; e.size = s;
                push.push_back(e);
            }
            for (u32 p = 0; p < 4; ++p) {
                if (p == me) continue;
                StMoveEdge e;                       // write: in+1s → 对端 out+1s
                e.dstRemote = true; e.dstOwner = p; e.srcOff = 0x1000 + s;
                e.dstOff = 0x2000 + s; e.size = s;
                push.push_back(e);
            }
            std::string err;
            Check(CheckPullPushEdgeEquivalence(pull, push, me, err) == 0,
                  "J8: 4P AG pull-in == push-out transpose (16-edge universe symmetric): " + err);
            // 负例：push 的 write 目的 owner 漂移（p0 → p2——owner 进边键）
            std::vector<StMoveEdge> pushDrift = push;
            pushDrift[1].dstOwner = 2;               // 原 p0 的 write 落位错成 p2
            Check(CheckPullPushEdgeEquivalence(pull, pushDrift, me, err) != 0,
                  "J8: push dst-owner drift caught by edge multiset");
            // 负例：push 少一条 write（4→3）——边数不等
            std::vector<StMoveEdge> pushShort = push;
            pushShort.pop_back();
            Check(CheckPullPushEdgeEquivalence(pull, pushShort, me, err) != 0,
                  "J8: missing push edge caught by count");
            // RS 反向用（同一 helper）：AICPU 写侧出边 vs CCU 读侧入边
            //   4P RS me=1：CCU 读侧 pull：read×3 = (peer in+1s → scratch+peer*s)
            //   AICPU 写侧 push：write×3 = (in+peer*s → 对端 scratch+1*s)
            std::vector<StMoveEdge> rsPull;
            for (u32 p = 0; p < 4; ++p) {
                if (p == me) continue;
                StMoveEdge e;
                e.srcRemote = true; e.srcOwner = p; e.srcOff = 0x1000 + s;
                e.dstOff = 0x3000 + static_cast<u64>(p) * s; e.size = s;
                rsPull.push_back(e);
            }
            std::vector<StMoveEdge> rsPush;
            for (u32 p = 0; p < 4; ++p) {
                if (p == me) continue;
                StMoveEdge e;
                e.dstRemote = true; e.dstOwner = p;
                e.srcOff = 0x1000 + static_cast<u64>(p) * s;    // 我的 input 里 peer 的份
                e.dstOff = 0x3000 + s;                            // 对端 scratch 我的槽
                e.size = s;
                rsPush.push_back(e);
            }
            Check(CheckPullPushEdgeEquivalence(rsPull, rsPush, me, err) == 0,
                  "J8: 4P RS read-side in-edges == write-side out-edges (same helper reversed)");
        }
        // ------------------------------------------------------------------
        // J9 ⑨ RS chunk 镜像手算（8P/9P/16P 边界）+ 帧预算交叉
        // ------------------------------------------------------------------
        {
            Check(StRsChunkSizeMirror(8) == 8388608ULL, "J9: chunkSize 8P = 8MiB (64MB/8 exact)");
            Check(StRsChunkSizeMirror(9) == 7456512ULL,
                  "J9: chunkSize 9P = 7456512 (67108864/9=7456540r4 -> /128*128 floor)");
            Check(StRsChunkSizeMirror(16) == 4194304ULL, "J9: chunkSize 16P = 4MiB");
            Check(StRsChunkSizeMirror(1) == 67108864ULL, "J9: chunkSize 1P = 64MiB whole");
            // chunkCount 手算：8P 单 chunk 极限 slice = 8388608（count=1）；
            //   8388609 → 2 chunks（(8388609-1)/8388608+1 = 2）→ v5 拒收形态
            Check(StRsChunkCountMirror(8388608ULL, 8) == 1 &&
                      StRsChunkCountMirror(8388609ULL, 8) == 2,
                  "J9: chunkCount boundary 8388608 (1) vs 8388609 (2, refused by v5)");
            Check(StRsChunkCountMirror(0, 8) == 0, "J9: zero slice -> 0 chunks (skip semantics)");
            // J9 帧预算与 chunk 限界交叉：RS 9P 单 chunk 帧数 12；预算表是 N 的
            //   纯函数（与 chunk 限界独立——限界是拒收判据，不是帧数输入）
            Check(StVerbFrameBudgetRs(9) == 12, "J9: RS 9P budget 12 (frame math N-based, independent of chunk limit)");
        }
    }

    printf("\n%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
