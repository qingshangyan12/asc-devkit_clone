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

    printf("\n%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
