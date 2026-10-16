// R6 · WP-A1 · 动词→真原语接线单测（host shim 面，桩链接）
//
// 被测对象：l5/hcomm_verb_sink.h 的 HcommVerbSink（真实 IVerbSink 实现）
// 测试策略（任务书 §自检）：
//   ① 源级断言（真原语不可执行，harness 用桩）：桩侧记录每次原语调用的
//      (函数名, thread, channel/handle, 参数) 全序列，与"存量 SendRecvRead/
//      PreSync/PostSync/LocalCopy 的调用序"逐调用对照——期望序列手写（对照
//      wrapper.cc/ins_temp_all_gather_mesh_1D.cc 的行号语义），非从实现回推。
//   ② 同源断言：同一 Emit 序列喂 TraceSink（产 trace 字符串）与 HcommVerbSink
//      （产 HcommCallRecord 序列）——两者逐动词一致（verb/参数四元组）。
//   ③ 错误路径：未实现动词（WRITE/REDUCE/BROADCAST）HCCL_ERROR 拒绝不静默；
//      原语失败后停发真原语但记录完整。
//   ④ 资源边界：无主线程/无 slave/无 peer 通道/READ 超从线程数 → 拒绝。
//
// 期望值全部手写（禁自证：不引用被测实现的输出构造期望）。

#include "verb_sink.h"
#include "hcomm_verb_sink.h"
#include "action_collect_all.h"
#include "action_spec.h"
#include "backend_registry.h"
#include "l1_adapter.h"
// 后端实现（include 即触发 REGISTER_ACTION_BACKEND 静态注册——与既有测试同机制）
#include "backend_aicpu_collect_all.h"

#include <cstdio>
#include <string>
#include <vector>
using namespace mc2_ops_hccl::refactor;

// ============================================================================
// 桩：真原语的记录替身（extern "C" 覆盖 shim 声明；真链路下不参与编译）
// 每次调用记一行 "func|thread|ch|arg1|arg2|arg3"，供逐调用对照
// ============================================================================
struct StubCall {
    std::string func;
    u64 thread{0};
    u64 channel{0};
    u64 a1{0}, a2{0}, a3{0};
};
static std::vector<StubCall> g_stubCalls;
static int32_t g_stubRet = 0;      // 全局注错开关（0=全部成功）

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
// R12 · WP-9②：WRITE/REDUCE 桩（sink 新接线两原语——RS×AICPU 写侧）
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
// R15 · ②：NHR_RUN 执行面原语桩（sink DoNhrRun 消费）
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

// 8 卡 AG 资源束（对照现状 AG mesh：threadNum=N-1=7，主线程+6 从线程；
// 逐 peer 一通道，peer=1..7）
static HcommVerbRes MakeRes8Ranks()
{
    HcommVerbRes res;
    for (u32 i = 0; i < 7; ++i) {
        res.threads.push_back(0x1000 + i);   // 主=0x1000，从=0x1001..0x1006
    }
    for (u32 peer = 1; peer <= 7; ++peer) {
        ChannelInfo ch;
        ch.remoteRank = peer;
        ch.handle = 0x20000 + peer;          // 通道句柄逐 peer 唯一
        // R12 · WP-9②：对端 cclBuff 基址（WRITE dst 合成源——remoteCclMem，
        // alg_param.h:453；R12-2① 写侧几何的 sink 侧半边）
        ch.remoteCclMem.addr = reinterpret_cast<void*>(static_cast<uintptr_t>(0x20000 + peer * 0x100000));
        res.channels[peer] = {ch};
    }
    return res;
}

// 一条 READ 动词的期望五步序列（手写，对照 wrapper.cc:530-549 逐行）：
//   ①ChNotifyRecord(thread, ch, ACK=0)  ②ChNotifyWait(thread, ch, 0, 1836)
//   ③Read(thread, ch, dst, src, size)   ④ChNotifyRecord(thread, ch, DATA_SIGNAL=1)
//   ⑤ChNotifyWait(thread, ch, 1, 1836)
static std::vector<StubCall> ExpectReadFive(u64 thread, u64 ch, u64 dst, u64 src, u64 size)
{
    std::vector<StubCall> e;
    StubCall s1{"ChNotifyRecord", thread, ch, 0, 0, 0};
    StubCall s2{"ChNotifyWait", thread, ch, 0, 1836, 0};
    StubCall s3{"Read", thread, ch, dst, src, size};
    StubCall s4{"ChNotifyRecord", thread, ch, 1, 0, 0};
    StubCall s5{"ChNotifyWait", thread, ch, 1, 1836, 0};
    e.push_back(s1); e.push_back(s2); e.push_back(s3); e.push_back(s4); e.push_back(s5);
    return e;
}

static bool CallsEqual(const std::vector<StubCall>& got, const std::vector<StubCall>& expect, std::string& firstDiff)
{
    if (got.size() != expect.size()) {
        firstDiff = "count " + std::to_string(got.size()) + " != " + std::to_string(expect.size());
        return false;
    }
    for (size_t i = 0; i < got.size(); ++i) {
        if (got[i].func != expect[i].func || got[i].thread != expect[i].thread ||
            got[i].channel != expect[i].channel || got[i].a1 != expect[i].a1 ||
            got[i].a2 != expect[i].a2 || got[i].a3 != expect[i].a3) {
            firstDiff = "call[" + std::to_string(i) + "] " + got[i].func + " != " + expect[i].func +
                        " (thr " + std::to_string(got[i].thread) + "/" + std::to_string(expect[i].thread) +
                        ", ch " + std::to_string(got[i].channel) + "/" + std::to_string(expect[i].channel) + ")";
            return false;
        }
    }
    return true;
}

int main()
{
    printf("== ① READ：SendRecvRead 五步全套（逐调用对照手写期望） ==\n");
    {
        g_stubCalls.clear();
        g_stubRet = 0;
        HcommVerbRes res = MakeRes8Ranks();
        HcommVerbSink sink(res);
        // 一条 READ：peer3、src=0xA000（对端 cclBuff 基址+srcOff）、dst=0xB000、size=4096
        sink.Emit(VerbCall{VerbCall::READ, 0xA000, 0xB000, 4096, 3});
        Check(sink.ErrCode() == 0, "READ ok: " + sink.Err());
        Check(sink.Calls().size() == 1, "1 call record");
        // 期望（手写）：首条 READ → threads[0]（主线程承载首条 read，存量形态
        // :103/:216 threads[threadIdx]，threadIdx=0）、通道 0x20003
        const std::vector<StubCall> expect = ExpectReadFive(0x1000, 0x20003, 0xB000, 0xA000, 4096);
        std::string diff;
        Check(CallsEqual(g_stubCalls, expect, diff), "五步序列逐调用一致: " + diff);
        Check(g_stubCalls.size() == 5, "恰 5 次原语（不多不少——直调 HcommReadOnThread 单步形态即失败）");
    }

    printf("== ② 逐 peer READ：线程轮转 + 各自通道（AG mesh 序） ==\n");
    {
        g_stubCalls.clear();
        g_stubRet = 0;
        HcommVerbRes res = MakeRes8Ranks();
        HcommVerbSink sink(res);
        // 7 条 READ（peer 依 BuildArgs 序 1..7）。
        // 现状（对账后基线）：AG mesh threads.size()=7（主+6 从），7 条 read 由
        // threads[0..6] 承载（:103 循环 7 次、:216 threads[threadIdx]——主线程
        // threads[0] 承载首条 read）。sink 的 ThreadForRead 同式：第 i 条 → threads[i]。
        for (u32 peer = 1; peer <= 7; ++peer) {
            sink.Emit(VerbCall{VerbCall::READ, 0xA000 + peer, 0xB000 + peer, 1024, peer});
        }
        Check(sink.ErrCode() == 0, "7 READ ok: " + sink.Err());
        // 期望：第 i 条（i=0..6）→ threads[i]=0x1000..0x1006，通道 0x20001+i+1
        std::vector<StubCall> expect;
        for (u32 i = 0; i < 7; ++i) {
            const u32 peer = i + 1;
            std::vector<StubCall> five = ExpectReadFive(0x1000 + i, 0x20000 + peer,
                                                        0xB000 + peer, 0xA000 + peer, 1024);
            expect.insert(expect.end(), five.begin(), five.end());
        }
        std::string diff;
        Check(CallsEqual(g_stubCalls, expect, diff), "7×五步逐调用一致（主线程承载首条+从线程轮转+各自通道）: " + diff);
        Check(g_stubCalls.size() == 35, "7×5=35 原语调用");
        // 第 8 条 READ：threads[7] 越界 → 拒绝（不静默错线程）
        sink.Emit(VerbCall{VerbCall::READ, 0xA000, 0xB000, 1024, 7});
        Check(sink.ErrCode() != 0, "8th READ (beyond thread count) rejected: " + sink.Err());
        Check(g_stubCalls.size() == 35, "拒绝路径零新增原语调用");
    }

    printf("== ③ COPY：HcommLocalCopyOnThread 单步（size=0 早退） ==\n");
    {
        g_stubCalls.clear();
        g_stubRet = 0;
        HcommVerbRes res = MakeRes8Ranks();
        HcommVerbSink sink(res);
        sink.Emit(VerbCall{VerbCall::COPY, 0x1111, 0x2222, 512});
        Check(sink.ErrCode() == 0, "COPY ok");
        // 期望（手写，wrapper LocalCopy :731-733）：主线程 0x1000，dst/src/len 直传
        Check(g_stubCalls.size() == 1 && g_stubCalls[0].func == "LocalCopy" &&
                  g_stubCalls[0].thread == 0x1000 && g_stubCalls[0].a1 == 0x2222 &&
                  g_stubCalls[0].a2 == 0x1111 && g_stubCalls[0].a3 == 512,
              "LocalCopy(0x1000, dst=0x2222, src=0x1111, 512)");
        // POST_COPY 同一原语（词汇表映射一致）
        g_stubCalls.clear();
        sink.Emit(VerbCall{VerbCall::POST_COPY, 0x3333, 0x4444, 64});
        Check(sink.ErrCode() == 0 && g_stubCalls.size() == 1 && g_stubCalls[0].func == "LocalCopy",
              "POST_COPY → LocalCopy（同一原语）");
        // size=0：与 LocalCopy 的 size==0 早退同语义（wrapper :721-723），零调用
        g_stubCalls.clear();
        sink.Emit(VerbCall{VerbCall::COPY, 0x1111, 0x2222, 0});
        Check(sink.ErrCode() == 0 && g_stubCalls.empty(), "COPY size=0 → 零原语（早退同语义）");
    }

    printf("== ④ SYNC_PEER_PRE/POST：两段循环序（非逐对交错） ==\n");
    {
        g_stubCalls.clear();
        g_stubRet = 0;
        HcommVerbRes res = MakeRes8Ranks();
        HcommVerbSink sink(res);
        sink.Emit(VerbCall{VerbCall::SYNC_PEER_PRE});
        Check(sink.ErrCode() == 0, "SYNC_PRE ok");
        // 期望（手写，wrapper :848-857）：6 slave（threads[1..6]）
        //   第一段：6 次 ThreadNotifyRecord(main=0x1000 → sub_i, idx 0)——全 record 在前
        //   第二段：6 次 ThreadNotifyWait(sub_i, idx 0, 1836)——全 wait 在后
        Check(g_stubCalls.size() == 12, "SYNC_PRE = 12 原语（6 record + 6 wait）");
        bool preOrderOk = true;
        for (u32 i = 0; i < 6; ++i) {
            const StubCall& rec = g_stubCalls[i];
            if (rec.func != "ThreadNotifyRecord" || rec.thread != 0x1000 ||
                rec.channel != 0x1001 + i || rec.a1 != 0) {
                preOrderOk = false;
            }
        }
        for (u32 i = 0; i < 6; ++i) {
            const StubCall& w = g_stubCalls[6 + i];
            if (w.func != "ThreadNotifyWait" || w.thread != 0x1001 + i ||
                w.a1 != 0 || w.a2 != 1836) {
                preOrderOk = false;
            }
        }
        Check(preOrderOk, "SYNC_PRE 两段循环序：record 全下发(0x1000→sub,idx0) 再逐个 wait");
        // 反向病灶可判：若实现是逐对交错，第 2 个调用是 Wait 而非 Record——上面已锁

        g_stubCalls.clear();
        sink.Emit(VerbCall{VerbCall::SYNC_PEER_POST});
        Check(sink.ErrCode() == 0, "SYNC_POST ok");
        // 期望（手写，wrapper :881-890）：
        //   第一段：6 次 ThreadNotifyWait(main=0x1000, idx i, 1836)——wait 全在前
        //   第二段：6 次 ThreadNotifyRecord(sub_i → main, idx i)
        Check(g_stubCalls.size() == 12, "SYNC_POST = 12 原语（6 wait + 6 record）");
        bool postOrderOk = true;
        for (u32 i = 0; i < 6; ++i) {
            const StubCall& w = g_stubCalls[i];
            if (w.func != "ThreadNotifyWait" || w.thread != 0x1000 ||
                w.a1 != i || w.a2 != 1836) {
                postOrderOk = false;
            }
        }
        for (u32 i = 0; i < 6; ++i) {
            const StubCall& rec = g_stubCalls[6 + i];
            if (rec.func != "ThreadNotifyRecord" || rec.thread != 0x1001 + i ||
                rec.channel != 0x1000 || rec.a1 != i) {
                postOrderOk = false;
            }
        }
        Check(postOrderOk, "SYNC_POST 两段循环序：wait 全部(main,idx i) 再 record 全下发(sub→main,idx i)");
    }

    printf("== ⑤ 同源断言：同一 Emit 序列 TraceSink vs HcommVerbSink ==\n");
    {
        // 真实 Compile→BuildArgs→Emit 序列（8 卡 AG，rank3 视角），两个 sink 同喂
        OpParam op;
        op.opType = static_cast<int>(HcclCMDType::HCCL_CMD_ALLGATHER);
        op.DataDes.count = 512 * 1024;
        op.DataDes.dataType = HCCL_DATA_TYPE_FP16;
        op.engine = CommEngine::COMM_ENGINE_AICPU;
        op.userRank = 3;
        TopoInfoWithNetLayerDetails topo;
        topo.userRankSize = 8; topo.topoLevelNums = 1; topo.level0Topo = Level0Shape::MESH_1D;
        std::vector<u32> ranks;
        for (u32 i = 0; i < 8; ++i) ranks.push_back(i);
        Plan plan; std::string err;
        Check(L1Adapter::Compile(op, topo, {ranks}, "sole[mesh]", plan, err) == HCCL_SUCCESS,
              "compile ok: " + err);
        LoopSplitter splitter;
        CollectAllArgs args = CollectAll::BuildArgs(plan.segments[0], splitter);

        // TraceSink：产 trace 字符串（与 sink 同喂 seqOk）
        std::string traceStr;
        TraceSink traceSink(&traceStr);
        HcommVerbRes res;
        res.threads = {0x1000, 0x1001};   // 主+1 从：2 条 read 用 threads[1] 与…仍 1 slave
        // 为让真原语路径成功，序列限 1 条 READ（≤slave）
        std::vector<VerbCall> seqOk;
        seqOk.push_back(VerbCall{VerbCall::COPY, 0, 0x180000, 0x60000});
        seqOk.push_back(VerbCall{VerbCall::SYNC_PEER_PRE});
        seqOk.push_back(VerbCall{VerbCall::READ, 0x30000, 0x60000, 0x60000, 1});
        seqOk.push_back(VerbCall{VerbCall::SYNC_PEER_POST});
        ChannelInfo ch1;
        ch1.remoteRank = 1;
        ch1.handle = 0x20001;
        res.channels[1] = {ch1};

        for (const auto& c : seqOk) {
            traceSink.Emit(c);
        }
        g_stubCalls.clear();
        HcommVerbSink sink(res);
        for (const auto& c : seqOk) {
            sink.Emit(c);
        }
        Check(sink.ErrCode() == 0, "sink on 4-verb seq ok: " + sink.Err());
        Check(sink.Calls().size() == seqOk.size(), "call record count == verb count (同源基数)");
        // 逐动词同源：TraceSink 的 trace 串重放 == sink 的 Call 记录（verb/参数四元组）
        bool sameOrigin = true;
        std::string rebuilt;
        for (const HcommCallRecord& r : sink.Calls()) {
            VerbCall c;
            c.verb = r.verb; c.src = r.p0; c.dst = r.p1; c.size = r.p2; c.peer = r.peer;
            rebuilt += HcommVerbSink::RenderVerb(c) + " ";
        }
        sameOrigin = (rebuilt == traceStr);
        Check(sameOrigin, "同源：TraceSink(trace) == RenderVerb(sink.calls) 逐字符一致");
        if (!sameOrigin) {
            printf("    trace=[%s]\n    built=[%s]\n", traceStr.c_str(), rebuilt.c_str());
        }
        // Compile 域的同源性（8 卡）在两侧 Emit 序列层面再锁一次：
        // TraceSink 走真 Compile→BuildArgs→Emit，动词序结构 pre → read×7 → post
        EmitCtx ctxTrace;
        BackendRegistry::Get(ActionKind::CollectAll, Engine::AICPU)->Emit(plan.segments[0], &args, ctxTrace);
        Check(ctxTrace.trace.find("syncPeer:pre") < ctxTrace.trace.find("read") &&
                  ctxTrace.trace.rfind("syncPeer:post") > ctxTrace.trace.rfind("read"),
              "Compile 域 Emit 序：pre → read×7 → post（结构一致性）");
    }

    printf("== ⑥ 未实现动词显式拒绝（不静默）+ WP-9 WRITE/REDUCE 已接线形态 ==\n");
    {
        // R12 · WP-9②（2026-10-09）：WRITE/REDUCE 已从"未实现拒绝"转为真接线
        //（RS×AICPU 写侧——原 R6 断言的"not wired"口径退役，改断真原语序）。
        // BROADCAST 维持拒绝（CCU 声明式专属）。
        g_stubCalls.clear();
        HcommVerbRes res = MakeRes8Ranks();
        HcommVerbSink sink(res);
        // WRITE：五步序（R12-2③——与 READ 同构方向相反）+ threads[1..] 偏移 1
        //（R12-2②：threadNum=N，主线程不下发写）
        sink.Emit(VerbCall{VerbCall::WRITE, 0x1, 0x2, 0x100, 5});
        Check(sink.ErrCode() == 0, "WRITE wired (R12 WP-9): " + sink.Err());
        {
            // 期望序（手写，对照 wrapper DoSendRecvBatchTx :116-140 逐行）：
            //   ChNotifyRecord(th=threads[1], ch=peer5 的 handle, ACK=0)
            //   ChNotifyWait(th=threads[1], ch, ACK, 1836)
            //   Write(th, ch, dst=remoteCclMem+0x2, src=0x1, 0x100)
            //   ChNotifyRecord(th, ch, DATA_SIGNAL=1)
            //   ChNotifyWait(th, ch, 1, 1836)
            const u64 expectCh = 0x20000 + 5;   // MakeRes8Ranks: peer5 → 0x20005
            const u64 expectTh = 0x1001;        // 第 1 条 WRITE → threads[0+1]
            // dst 合成手算：remoteCclMem(peer5) = 0x20000+5*0x100000 = 0x520000，
            //   + 帧 dst 结构量 0x2 → 0x520002（R12-2①：remoteCclMem 合成）
            const u64 expectDst = 0x20000 + 5ULL * 0x100000 + 0x2;
            bool seqOk = g_stubCalls.size() == 5 &&
                g_stubCalls[0].func == "ChNotifyRecord" && g_stubCalls[0].thread == expectTh &&
                g_stubCalls[0].channel == expectCh && g_stubCalls[0].a1 == 0 &&
                g_stubCalls[1].func == "ChNotifyWait" && g_stubCalls[1].thread == expectTh &&
                g_stubCalls[1].channel == expectCh && g_stubCalls[1].a1 == 0 &&
                g_stubCalls[2].func == "Write" && g_stubCalls[2].thread == expectTh &&
                g_stubCalls[2].channel == expectCh && g_stubCalls[2].a1 == expectDst &&
                g_stubCalls[2].a2 == 0x1 && g_stubCalls[2].a3 == 0x100 &&
                g_stubCalls[3].func == "ChNotifyRecord" && g_stubCalls[3].a1 == 1 &&
                g_stubCalls[4].func == "ChNotifyWait" && g_stubCalls[4].a1 == 1;
            Check(seqOk, "WRITE 五步序 + threads[1] + remoteCclMem 合成 dst（R12-2①②③）");
        }
        // REDUCE：HcommLocalReduceOnThread（wrapper LocalReduce 语义）+ threads[0]
        g_stubCalls.clear();
        sink.Emit(VerbCall{VerbCall::REDUCE, 0x1, 0x2, 0x100, 0});
        Check(sink.ErrCode() == 0, "REDUCE wired (R12 WP-9)");
        // 真签名 6 参（hcomm_primitives.h:99）：count = size/dts = 0x100/2 = 0x80
        //（元素数非字节——R12 修正：sink DoReduce 按 dataTypeSize 换算）
        Check(g_stubCalls.size() == 1 && g_stubCalls[0].func == "LocalReduce" &&
                  g_stubCalls[0].thread == 0x1000 && g_stubCalls[0].a1 == 0x2 &&
                  g_stubCalls[0].a2 == 0x1 && g_stubCalls[0].a3 == 0x80,
              "REDUCE → HcommLocalReduceOnThread(threads[0])（真签名 count=元素数 0x80）");
        // BROADCAST：维持拒绝（CCU 声明式专属——kAicpuVerbMap N/A 行）
        g_stubCalls.clear();
        sink.Emit(VerbCall{VerbCall::BROADCAST, 0x1, 0x2, 0x100, 0});
        Check(sink.ErrCode() != 0 && sink.Err().find("not wired") != std::string::npos,
              "BROADCAST rejected (AICPU N/A 表行): " + sink.Err());
        Check(sink.Calls().size() == 3, "三动词均留调用记录（诊断完整）");
        Check(g_stubCalls.empty(), "BROADCAST 拒绝零原语下发");
    }

    printf("== ⑦ 原语失败：停发真原语但记录继续 ==\n");
    {
        g_stubCalls.clear();
        g_stubRet = 7;   // 全原语注错
        HcommVerbRes res = MakeRes8Ranks();
        HcommVerbSink sink(res);
        sink.Emit(VerbCall{VerbCall::COPY, 0x1, 0x2, 0x100});
        Check(sink.ErrCode() == 7, "first failure code captured");
        sink.Emit(VerbCall{VerbCall::READ, 0x1, 0x2, 0x100, 3});
        Check(sink.Calls().size() == 2, "second verb still recorded");
        Check(g_stubCalls.size() == 1, "failed state: no further primitive issued");
        g_stubRet = 0;
    }

    printf("== ⑧ 资源边界防御 ==\n");
    {
        // 无线程
        HcommVerbRes empty;
        HcommVerbSink s1(empty);
        s1.Emit(VerbCall{VerbCall::COPY, 0x1, 0x2, 0x100});
        Check(s1.ErrCode() != 0, "no threads → rejected");
        // 有主无从：SYNC 拒绝
        HcommVerbRes onlyMain;
        onlyMain.threads = {0x1000};
        HcommVerbSink s2(onlyMain);
        s2.Emit(VerbCall{VerbCall::SYNC_PEER_PRE});
        Check(s2.ErrCode() != 0, "SYNC_PRE without slaves → rejected");
        // READ 无该 peer 通道
        HcommVerbRes noChan;
        noChan.threads = {0x1000, 0x1001};
        ChannelInfo ch9;
        ch9.remoteRank = 9;
        noChan.channels[9] = {ch9};
        HcommVerbSink s3(noChan);
        s3.Emit(VerbCall{VerbCall::READ, 0x1, 0x2, 0x100, 5});
        Check(s3.ErrCode() != 0 && s3.Err().find("no channel") != std::string::npos,
              "READ without peer channel → rejected: " + s3.Err());
    }

    // ========================================================================
    // R16 · #7：线程策略表驱动对拍锁（表列 ↔ 实际线程分配逐动词一致）。
    // 期望值手写（threads 池 7 条：主 0x1000 + 从 0x1001..0x1006——同 MakeRes8Ranks）。
    // 锁点：kAicpuVerbMap.thread 列与 sink ThreadFor 查表行为的一致性——
    // 改表列不改 ThreadFor（或反之）此段必 FAIL（表实漂移即抓）。
    // ========================================================================
    printf("== R16 #7 线程策略表↔实际分配对拍 ==\n");
    {
        // a) 表列声明值逐动词核对（手写期望——与 sink 行为无关的独立声明面）
        Check(ThreadPolicyOf(VerbCall::COPY, false) == ThreadPolicy::MAIN, "表: COPY=MAIN");
        Check(ThreadPolicyOf(VerbCall::POST_COPY, false) == ThreadPolicy::MAIN, "表: POST_COPY=MAIN");
        Check(ThreadPolicyOf(VerbCall::READ, false) == ThreadPolicy::COUNTER_OFF_0, "表: READ=COUNTER_OFF_0");
        Check(ThreadPolicyOf(VerbCall::WRITE, false) == ThreadPolicy::COUNTER_OFF_1, "表: WRITE=COUNTER_OFF_1");
        Check(ThreadPolicyOf(VerbCall::REDUCE, false) == ThreadPolicy::MAIN, "表: REDUCE=MAIN");
        Check(ThreadPolicyOf(VerbCall::SYNC_PEER_PRE, false) == ThreadPolicy::MAIN_PLUS_SLAVES,
              "表: SYNC_PEER_PRE=MAIN_PLUS_SLAVES");
        Check(ThreadPolicyOf(VerbCall::SYNC_PEER_POST, false) == ThreadPolicy::MAIN_PLUS_SLAVES,
              "表: SYNC_PEER_POST=MAIN_PLUS_SLAVES");
        Check(ThreadPolicyOf(VerbCall::BROADCAST, false) == ThreadPolicy::NOT_WIRED, "表: BROADCAST=NOT_WIRED");
        Check(ThreadPolicyOf(VerbCall::NHR_RUN, false) == ThreadPolicy::BY_CHANNEL, "表: NHR_RUN=BY_CHANNEL");
        // CCU 表恒 NA（无线程概念——微码自驱动）
        bool ccuAllNa = true;
        const VerbCall::Verb all[] = {VerbCall::COPY, VerbCall::POST_COPY, VerbCall::READ,
                                      VerbCall::WRITE, VerbCall::REDUCE, VerbCall::BROADCAST,
                                      VerbCall::SYNC_PEER_PRE, VerbCall::SYNC_PEER_POST,
                                      VerbCall::NHR_RUN};
        for (VerbCall::Verb v : all) {
            ccuAllNa = ccuAllNa && ThreadPolicyOf(v, true) == ThreadPolicy::NA;
        }
        Check(ccuAllNa, "表: kCcuVerbMap 全 9 行=NA（CCU 微码自驱动无线程）");

        // b) 表→行为对拍（同一 sink 实例混发动词，逐条断原语记录的 thread 字面量）
        g_stubCalls.clear();
        g_stubRet = 0;
        HcommVerbRes res = MakeRes8Ranks();
        HcommVerbSink sink(res);
        // COPY → threads[0]=0x1000（MAIN）
        sink.Emit(VerbCall{VerbCall::COPY, 0x1, 0x2, 0x100});
        Check(sink.ErrCode() == 0 && g_stubCalls.size() == 1 && g_stubCalls[0].func == "LocalCopy" &&
                  g_stubCalls[0].thread == 0x1000,
              "对拍: COPY 落 threads[0]（表 MAIN）");
        // REDUCE → threads[0]=0x1000（MAIN）
        g_stubCalls.clear();
        sink.Emit(VerbCall{VerbCall::REDUCE, 0x1, 0x2, 0x100});
        Check(sink.ErrCode() == 0 && g_stubCalls.size() == 1 && g_stubCalls[0].func == "LocalReduce" &&
                  g_stubCalls[0].thread == 0x1000,
              "对拍: REDUCE 落 threads[0]（表 MAIN）");
        // READ×2 → threads[0]/threads[1]（COUNTER_OFF_0——主线程承载首条）
        g_stubCalls.clear();
        sink.Emit(VerbCall{VerbCall::READ, 0xA, 0xB, 0x100, 1});
        sink.Emit(VerbCall{VerbCall::READ, 0xA, 0xB, 0x100, 2});
        Check(sink.ErrCode() == 0 && g_stubCalls.size() == 10 &&
                  g_stubCalls[0].func == "ChNotifyRecord" && g_stubCalls[0].thread == 0x1000 &&
                  g_stubCalls[5].func == "ChNotifyRecord" && g_stubCalls[5].thread == 0x1001,
              "对拍: READ 首条 threads[0]、次条 threads[1]（表 COUNTER_OFF_0）");
        // WRITE → threads[1]=0x1001（COUNTER_OFF_1——主线程不下发写；READ 计数 2 不干扰）
        g_stubCalls.clear();
        sink.Emit(VerbCall{VerbCall::WRITE, 0x1, 0x2, 0x100, 5});
        Check(sink.ErrCode() == 0 && g_stubCalls.size() == 5 && g_stubCalls[0].func == "ChNotifyRecord" &&
                  g_stubCalls[0].thread == 0x1001,
              "对拍: WRITE 首条落 threads[1]（表 COUNTER_OFF_1，与 READ 计数独立）");
        // NOT_WIRED 走 Dispatch 的 BROADCAST 拒绝（既有 ⑥ 断言锁——此处零新增）
    }

    printf("\n%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
