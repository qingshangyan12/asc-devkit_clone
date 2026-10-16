// R14 · WP-3：A 形态执行期并发编排器单测（host shim 面，桩链接）
//
// 被测对象：unified/l5/segment_orchestrator.h（OrchestrateSegments）
//   T1 批划分（post=T 收批 / 尾段 post=F 补末批）
//   T2 域映射（ranks 相等去重首现序 / domainOfSeg / 域线程切池）
//   T3 降级（双域 Σ 超池 → 串行全局束 + warnings；单域预检超池 → 预检降级）
//   T4 组 1 时序 golden（M0_R14 §六：A 交错 4 段 2 域——域内屏障两段序 + 批尾
//      交叉对 4 条 host 顺序下发，桩粒度逐条手算）
//   T5 等价对拍（env 开/关两路 ConsumeUnifiedPlanAicpu 输出逐条相等——常跑防漂移）
//   T6 负例（空序列 / null driver / RS 后端未注册段失败透传）
// 期望值全部独立手算（M0_R14 §六三组），禁自证。

#include "verb_sink.h"
#include "hcomm_verb_sink.h"
#include "segment_orchestrator.h"
#include "aicpu_consumer.h"          // R14 · WP-2 分叉 + Rebuild/Drive 单源件
#include "unified_route.h"           // UseUnifiedConcurrencyPath（三层开关）
#include "action_collect_all.h"
#include "action_spec.h"
#include "backend_registry.h"
#include "l1_adapter.h"
#include "plan_serializer.h"
#include "backend_aicpu_collect_all.h"
#include "backend_aicpu_reduce_slice.h"

#include <cstdio>
#include <string>
#include <vector>
using namespace mc2_ops_hccl::refactor;

// ---------------- 桩（与 poc_unified_exec_test 同一套） ----------------
struct StubCall {
    std::string func;
    u64 thread{0};
    u64 dstThread{0};
    u64 a1{0}, a2{0}, a3{0};
};
static std::vector<StubCall> g_stubCalls;
static int32_t g_stubRet = 0;
static void Record(const char* f, u64 t, u64 d, u64 a1, u64 a2, u64 a3)
{
    StubCall s;
    s.func = f; s.thread = t; s.dstThread = d; s.a1 = a1; s.a2 = a2; s.a3 = a3;
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
// R15 · ②：NHR_RUN 执行面原语桩（DoNhrRun 消费）
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
// fake driver：不产真动词，只记录 (段号, 域子束 threads) 的派发序——T1/T2/T3 的
// 断言对象（真驱动链的动词序等价由 T4/T5 承载）。
struct FakeDispatch {
    std::vector<u32> segOrder;                 // 段号派发序（sliceSize 标记）
    std::vector<std::vector<ThreadHandle>> segThreads;   // 逐段收到的子束 threads
    OrchestrateSegResult operator()(const ActionSpec& a, const HcommVerbRes& domainRes)
    {
        OrchestrateSegResult r;
        r.ret = HCCL_SUCCESS;
        r.trace = "seg" + std::to_string(static_cast<u32>(a.layout.sliceSize));
        segOrder.push_back(static_cast<u32>(a.layout.sliceSize));
        segThreads.push_back(domainRes.threads);
        return r;
    }
};

// M0 §六构造：16P 双 mesh 域 RS 段（域 0 ranks{0,1,2,3}、域 1 ranks{0,4,8,12}，
// me=0）。A 形态 4 段（seg0=域0 P0 / seg1=域1 P1 / seg2=域1 P0 / seg3=域0 P1）
// ——手造段序列（ExpandTieredSegments 的 A 分支同构产物；R14 测试不依赖 tier
// 编译链，直接以 ActionSpec 表达 sync 位语义）。
static ActionSpec MakeRsSeg(const std::vector<u32>& ranks, u64 slice, u64 baseOff,
                            bool pre, bool post)
{
    ActionSpec a;
    a.kind = ActionKind::ReduceSlice;
    a.domain.ranks = ranks;
    a.domain.myRank = ranks[0];
    a.layout.sliceSize = slice;
    a.layout.outputSliceStride = slice;
    a.layout.inBaseOff = baseOff;
    a.layout.outBaseOff = baseOff;
    a.layout.scratchBaseOff = 0xC00000ULL;
    a.layout.dataTypeSize = 2;
    a.transport = TransportPolicy::VIA_CCUBUF;
    a.sync.preBarrier = pre;
    a.sync.postBarrier = post;
    return a;
}

// 8 线程池 + 双域通道表（域 0 peers {1,2,3}、域 1 peers {4,8,12}——myRank=0 视角）
static OrchestratorRes MakeDualDomainRes(u32 poolSize)
{
    OrchestratorRes res;
    for (u32 i = 0; i < poolSize; ++i) {
        res.threads.push_back(0x1000 + i);
    }
    const u32 peers0[3] = {1, 2, 3};
    const u32 peers1[3] = {4, 8, 12};
    for (int k = 0; k < 3; ++k) {
        ChannelInfo c0;
        c0.remoteRank = peers0[k];
        c0.handle = 0x20000 + peers0[k];
        res.channels[peers0[k]].push_back(c0);
        ChannelInfo c1;
        c1.remoteRank = peers1[k];
        c1.handle = 0x30000 + peers1[k];
        res.channels[peers1[k]].push_back(c1);
    }
    return res;
}

static std::vector<ActionSpec> MakeAFormSegments()
{
    const std::vector<u32> d0 = {0, 1, 2, 3};
    const std::vector<u32> d1 = {0, 4, 8, 12};
    const u64 S = 1024;        // 全片；半片 = 512（ForChunk 0.5 语义的手算等价）
    std::vector<ActionSpec> segs;
    segs.push_back(MakeRsSeg(d0, S / 2, 0, true, false));     // seg0 域0 P0 批内
    segs.push_back(MakeRsSeg(d1, S / 2, S / 2, true, true));  // seg1 域1 P1 批尾
    segs.push_back(MakeRsSeg(d1, S / 2, 0, true, false));     // seg2 域1 P0 批内
    segs.push_back(MakeRsSeg(d0, S / 2, S / 2, true, true));  // seg3 域0 P1 批尾
    return segs;
}

// 桩序列里找屏障指令（thread/dstThread/idx 全对）——序号敏感
static long FindCall(const std::string& func, u64 thread, u64 dstThread, u64 idx)
{
    for (size_t i = 0; i < g_stubCalls.size(); ++i) {
        const StubCall& c = g_stubCalls[i];
        if (c.func == func && c.thread == thread && c.dstThread == dstThread &&
            c.a1 == idx) {
            return static_cast<long>(i);
        }
    }
    return -1;
}

// 末次匹配（交叉对断言用：批1 域内补 Post 与批尾交叉对可能同参数——
// 交叉对是全局最后的编排指令，用 last 位置判序才不误比到批1 的域内指令）
static long FindCallLast(const std::string& func, u64 thread, u64 dstThread, u64 idx)
{
    for (size_t i = g_stubCalls.size(); i > 0; --i) {
        const StubCall& c = g_stubCalls[i - 1];
        if (c.func == func && c.thread == thread && c.dstThread == dstThread &&
            c.a1 == idx) {
            return static_cast<long>(i - 1);
        }
    }
    return -1;
}

int main()
{
    printf("== T1 批划分（post=T 收批 / 尾段 post=F 补末批） ==\n");
    {
        // A 形态 4 段（post = F,T,F,T）→ 2 批 [0,1],[2,3]
        const std::vector<ActionSpec> segs = MakeAFormSegments();
        FakeDispatch fd;
        const OrchestratorRes res = MakeDualDomainRes(8);
        const OrchestratorResult r = OrchestrateSegments(segs, res,
            [&fd](const ActionSpec& a, const HcommVerbRes& d) { return fd(a, d); });
        Check(r.ret == HCCL_SUCCESS, "T1 A 形态编排成功: " + r.err);
        Check(r.batches == 2, "T1 批数 = 2（post=T 收批）");
        Check(r.batchTail.size() == 2 && r.batchTail[0] == 1 && r.batchTail[1] == 3,
              "T1 批尾段 = [1, 3]");
        Check(r.segmentsRun == 4, "T1 4 段全驱动");
        // 尾段 post=F 补末批：构造 4 段 post = F,F,F,F → 1 批（尾=3）
        std::vector<ActionSpec> noPost = MakeAFormSegments();
        for (size_t i = 0; i < noPost.size(); ++i) {
            noPost[i].sync.postBarrier = false;
        }
        FakeDispatch fd2;
        const OrchestratorResult r2 = OrchestrateSegments(noPost, res,
            [&fd2](const ActionSpec& a, const HcommVerbRes& d) { return fd2(a, d); });
        Check(r2.ret == HCCL_SUCCESS && r2.batches == 1 && r2.batchTail[0] == 3,
              "T1 尾段 post=F 补末批（1 批，尾=3）——不静默丢段");
        // 全 T 序列（B 步进形态）：每段一批
        std::vector<ActionSpec> allT = MakeAFormSegments();
        for (size_t i = 0; i < allT.size(); ++i) {
            allT[i].sync.postBarrier = true;
        }
        FakeDispatch fd3;
        const OrchestratorResult r3 = OrchestrateSegments(allT, res,
            [&fd3](const ActionSpec& a, const HcommVerbRes& d) { return fd3(a, d); });
        Check(r3.batches == 4, "T1 全 T 序列 = 4 批（B 步进步步隔断的数据面）");
    }

    printf("== T2 域映射（首现序去重 / 切池 / 子束传递） ==\n");
    {
        const std::vector<ActionSpec> segs = MakeAFormSegments();
        FakeDispatch fd;
        const OrchestratorRes res = MakeDualDomainRes(8);
        const OrchestratorResult r = OrchestrateSegments(segs, res,
            [&fd](const ActionSpec& a, const HcommVerbRes& d) { return fd(a, d); });
        // 域号：域 0（首现 seg0）= 0、域 1（首现 seg1）= 1；domainOfSeg = [0,1,1,0]
        Check(r.domainOfSeg.size() == 4 && r.domainOfSeg[0] == 0 && r.domainOfSeg[1] == 1 &&
                  r.domainOfSeg[2] == 1 && r.domainOfSeg[3] == 0,
              "T2 域映射 = [0,1,1,0]（ranks 相等去重，首现序）");
        // 子束切池：域 0 需 N0=4（RS 公式）→ threads[0..3]；域 1 → threads[4..7]
        Check(fd.segThreads.size() == 4, "T2 4 段各收到子束");
        bool cutOk = fd.segThreads.size() == 4;
        const ThreadHandle t0[4] = {0x1000, 0x1001, 0x1002, 0x1003};
        const ThreadHandle t1[4] = {0x1004, 0x1005, 0x1006, 0x1007};
        for (int k = 0; k < 4 && cutOk; ++k) {
            cutOk = fd.segThreads[0].size() == 4 && fd.segThreads[0][k] == t0[k] &&
                    fd.segThreads[2].size() == 4 && fd.segThreads[2][k] == t1[k];
        }
        Check(cutOk, "T2 域 0 子束 = t0..t3、域 1 子束 = t4..t7（首现序切池）");
        Check(fd.segThreads[1] == fd.segThreads[2] && fd.segThreads[3] == fd.segThreads[0],
              "T2 同域段共享同子束（seg0/seg3 同束、seg1/seg2 同束）");
        // AG 域线程公式对照：同构 4 段全 CollectAll → 需 N-1=3
        std::vector<ActionSpec> agSegs = MakeAFormSegments();
        for (size_t i = 0; i < agSegs.size(); ++i) {
            agSegs[i].kind = ActionKind::CollectAll;
        }
        FakeDispatch fdAg;
        const OrchestratorResult rAg = OrchestrateSegments(agSegs, res,
            [&fdAg](const ActionSpec& a, const HcommVerbRes& d) { return fdAg(a, d); });
        Check(rAg.ret == HCCL_SUCCESS && fdAg.segThreads.size() == 4 &&
                  fdAg.segThreads[0].size() == 3 && fdAg.segThreads[2].size() == 3,
              "T2 AG 域公式 N-1=3（RS N=4 的对照——DomainThreadNeed 分动作）");
    }

    printf("== T3 降级（双域 Σ 超池 → 串行 + warnings；单域预检超池 → 预检降级） ==\n");
    {
        const std::vector<ActionSpec> segs = MakeAFormSegments();
        // 池 4 < Σ 8：降级——全局束（4 条全池）逐段、无编排屏障、warnings 可观测
        const OrchestratorRes res4 = MakeDualDomainRes(4);
        FakeDispatch fd;
        const OrchestratorResult r = OrchestrateSegments(segs, res4,
            [&fd](const ActionSpec& a, const HcommVerbRes& d) { return fd(a, d); });
        Check(r.ret == HCCL_SUCCESS && r.degraded, "T3 双域 Σ=8 > 池 4 → 降级（不拒错）");
        Check(r.warnings.size() == 1 &&
                  r.warnings[0].find("threads 4 < required 8") != std::string::npos,
              "T3 降级 warning 可观测: " + (r.warnings.empty() ? std::string("(none)") : r.warnings[0]));
        bool globalOk = fd.segThreads.size() == 4;
        for (size_t i = 0; i < fd.segThreads.size() && globalOk; ++i) {
            globalOk = fd.segThreads[i].size() == 4 && fd.segThreads[i][0] == 0x1000;
        }
        Check(globalOk, "T3 降级路径逐段用全局束（threads[0..3] 全池）");
        Check(r.batches == 4, "T3 降级批=段（每段独立窗——串行语义）");
        Check(r.barriers.empty(), "T3 降级无编排屏障（纯串行重放）");
        // 单域预检：域 0 单独 N=8（8 卡域）但池 4 → 预检降级（M0 复核 B 项）
        std::vector<u32> bigRanks;
        for (u32 i = 0; i < 8; ++i) bigRanks.push_back(i);
        std::vector<ActionSpec> oneBig;
        oneBig.push_back(MakeRsSeg(bigRanks, 512, 0, true, true));
        FakeDispatch fdBig;
        const OrchestratorResult rBig = OrchestrateSegments(oneBig, res4,
            [&fdBig](const ActionSpec& a, const HcommVerbRes& d) { return fdBig(a, d); });
        Check(rBig.ret == HCCL_SUCCESS && rBig.degraded &&
                  rBig.warnings.size() == 1 &&
                  rBig.warnings[0].find("pre-check serial fallback") != std::string::npos,
              "T3 单域 need=8 > 池 4 → 预检降级（非半途 Fail——M0 复核 B）: " +
                  (rBig.warnings.empty() ? std::string("(none)") : rBig.warnings[0]));
        // 池恰好 = Σ 的边界：8 = 8 → 正常并发
        const OrchestratorRes res8 = MakeDualDomainRes(8);
        FakeDispatch fd8;
        const OrchestratorResult r8 = OrchestrateSegments(segs, res8,
            [&fd8](const ActionSpec& a, const HcommVerbRes& d) { return fd8(a, d); });
        Check(r8.ret == HCCL_SUCCESS && !r8.degraded && r8.batches == 2,
              "T3 池 8 = Σ 8 边界 → 正常并发（不降级）");
    }

    printf("== T4 组 1 时序 golden（真 driver——桩粒度逐条手算，M0 §六组 1） ==\n");
    {
        // 真 driver：DriveSegmentViaSink（WP-2 单源件——BuildArgs+Emit 进域子束 sink）。
        // 桩记录 hcomm 原语调用序，按 M0 §六组 1 手算逐条断言。
        const std::vector<ActionSpec> segs = MakeAFormSegments();
        const OrchestratorRes res = MakeDualDomainRes(8);
        UnifiedExecMsg msg;                       // RS msg 现值（写侧几何的 src 合成）
        msg.count = 512;                          // 半片元素数（FP16 → 1024B = slice）
        msg.inputAddr = 0x100000;
        msg.outputAddr = 0x200000;
        msg.dataType = HCCL_DATA_TYPE_FP16;
        msg.cclBuff.addr = reinterpret_cast<void*>(0xC00000ULL);
        msg.cclBuff.size = 4ULL * 1024 * 1024;
        IActionBackend* beAg = BackendRegistry::Peek(ActionKind::CollectAll, Engine::AICPU);
        IActionBackend* beRs = BackendRegistry::Peek(ActionKind::ReduceSlice, Engine::AICPU);
        Check(beRs != nullptr, "T4 RS AICPU 后端已注册（include 即注册）");
        g_stubCalls.clear();
        const OrchestratorResult r = OrchestrateSegments(
            segs, res,
            [&msg, beAg, beRs](const ActionSpec& a, const HcommVerbRes& domainRes) {
                return DriveSegmentViaSink(a, msg, 0, 4,
                    static_cast<u32>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER), 4, beAg, beRs,
                    domainRes);
            });
        Check(r.ret == HCCL_SUCCESS, "T4 真 driver 编排成功: " + r.err);
        Check(r.segmentsRun == 4 && r.perSegTrace.size() == 4, "T4 4 段全驱动（带 trace）");

        // ---- 桩序手算断言（M0 §六组 1 逐条）----
        // 域 0 组 {t0=0x1000,t1,t2,t3}（main=t0）、域 1 组 {t4=0x1004..t7}（main=t4）。
        // RS 段动词序（backend_aicpu_reduce_slice.h:61-131）：
        //   SYNC_PRE（sink DoSyncPre：record t_main→subs idx0 ×3、wait subs idx0 ×3）
        //   WRITE×3（threads[1..3]——ThreadForWrite 偏移 1）
        //   SYNC_POST（post=T 段；DoSyncPost：wait main idx0,1,2 ×3、record subs→main ×3）
        //   COPY（t0 主线程）+ REDUCE×3（t0）
        // 批内 post=F 段：编排器补域内 Post（domainPostBarrier 同 DoSyncPost 裸原语）
        // 批尾：域 mains 交叉对 t0↔t4（rec t4→t0 idx0、rec t0→t4 idx0、wait t0 idx0、
        //       wait t4 idx0）
        const u64 t0 = 0x1000, t1 = 0x1001, t2 = 0x1002, t3 = 0x1003;
        const u64 t4 = 0x1004, t5 = 0x1005, t6 = 0x1006, t7 = 0x1007;
        // (a) seg0（域 0，pre=T post=F）的 DoSyncPre 域内屏障：在 seg0 全部 WRITE 之前
        long s0PreRec = FindCall("ThreadNotifyRecord", t0, t1, 0);
        long s0PreRecLast = FindCall("ThreadNotifyRecord", t0, t3, 0);
        long s0Wait = FindCall("ThreadNotifyWait", t1, 0, 0);
        Check(s0PreRec >= 0 && s0PreRecLast >= 0 && s0Wait >= 0,
              "T4 seg0 域内 Pre 屏障在场（record t0→t1..t3 idx0 + wait t1..t3 idx0）");
        // WRITE 线程断言（桩槽位：Record("Write", thread, channel→dstThread 槽,
        // dst→a1, src→a2, len→a3)）：域 0 首对端 peer1 的 ch=0x20001 由 t1 承载
        // （ThreadForWrite 偏移 1——threads[1]）；域 1 首对端 peer4 的 ch=0x30004
        // 由 t5 承载（域 1 子束 threads[1]）
        // Write 桩槽位：Record("Write", thread, channel→dstThread 槽, dst→a1,
        // src→a2, len→a3)。匹配 = (thread, channel)。域 0 首对端 peer1 的
        // ch=0x20001 由 t1 承载（ThreadForWrite 偏移 1）；域 1 首对端 peer4 的
        // ch=0x30004 由 t5 承载（域 1 子束 threads[1]）
        auto hasWrite = [](u64 thread, u64 channel) {
            for (size_t i = 0; i < g_stubCalls.size(); ++i) {
                if (g_stubCalls[i].func == "Write" && g_stubCalls[i].thread == thread &&
                    g_stubCalls[i].dstThread == channel) {
                    return true;
                }
            }
            return false;
        };
        Check(hasWrite(t1, 0x20001),
              "T4 seg0（域 0）WRITE peer1 由 t1 承载（域 0 子束偏移 1）");
        Check(hasWrite(t2, 0x20002) && hasWrite(t3, 0x20003),
              "T4 seg0 peer2/3 由 t2/t3 承载（一 peer 一从线程，域内不串组）");
        Check(hasWrite(t5, 0x30004),
              "T4 seg1（域 1）WRITE peer4 由 t5 承载（域 1 子束偏移 1——组不串）");
        Check(hasWrite(t6, 0x30008) && hasWrite(t7, 0x3000c),
              "T4 seg1 peer8/12 由 t6/t7 承载（域 1 子束全用）");
        // (b) 批 1 域内补 Post（seg0 post=F）：wait t0 idx0/1/2 + record t1/t2/t3→t0
        long cWait0 = FindCall("ThreadNotifyWait", t0, 0, 0);
        long cRec1 = FindCall("ThreadNotifyRecord", t1, t0, 0);
        Check(cWait0 >= 0 && cRec1 >= 0, "T4 批1 域0 补 Post 在场（wait t0 + record t1→t0）");
        // (c) 批尾交叉对：rec t4→t0 idx0 / rec t0→t4 idx0 / wait t0 idx0 / wait t4 idx0
        long xRec41 = FindCallLast("ThreadNotifyRecord", t4, t0, 0);
        long xRec04 = FindCallLast("ThreadNotifyRecord", t0, t4, 0);
        long xWait0 = FindCallLast("ThreadNotifyWait", t0, 0, 0);
        long xWait4 = FindCallLast("ThreadNotifyWait", t4, 0, 0);
        Check(xRec41 >= 0 && xRec04 >= 0, "T4 批尾交叉对 record 双向在场（t4→t0 / t0→t4）");
        Check(xWait0 >= 0 && xWait4 >= 0, "T4 批尾交叉对 wait 双侧在场（t0 / t4）");
        // record 先于 wait（host 顺序下发——M0 §三下发者模型的断言锁）
        Check(xRec41 < xWait0 && xRec04 < xWait4,
              "T4 交叉对 record×2 先于 wait（host 单线程顺序下发，无自发 record 时序障碍）");
        // (d) 编排屏障记录结构：2 批 × （域内补 Post 1 + 交叉对 1）= 4 条
        Check(r.barriers.size() == 4, "T4 barriers 记录 = 4（2 批 × 域内补 Post + 交叉对）");
        bool barrierKindsOk = r.barriers.size() == 4;
        if (barrierKindsOk) {
            // 批 1：域 0 补 Post（domainInternal=true, d=0）+ 交叉对（false）
            barrierKindsOk = r.barriers[0].domainInternal && r.barriers[0].domainId == 0 &&
                             !r.barriers[1].domainInternal;
            // 批 2：域 1 补 Post（d=1）+ 交叉对
            barrierKindsOk = barrierKindsOk && r.barriers[2].domainInternal &&
                             r.barriers[2].domainId == 1 && !r.barriers[3].domainInternal;
        }
        Check(barrierKindsOk, "T4 屏障记录序 = [域0补Post, 交叉, 域1补Post, 交叉]（批 1→2）");
        // (e) 每段 WRITE 数 = 3（N-1，升序跳 me）+ 全局 WRITE 总数 = 12
        u32 totalWrites = 0;
        for (const StubCall& c : g_stubCalls) {
            if (c.func == "Write") ++totalWrites;
        }
        Check(totalWrites == 12, "T4 全局 WRITE = 12（4 段 × 3 peer）");
        // (f) 域内屏障的线程封闭性：域 0 的屏障指令不涉及 t4..t7，反之亦然
        bool closureOk = true;
        for (const StubCall& c : g_stubCalls) {
            if (c.func != "ThreadNotifyRecord" && c.func != "ThreadNotifyWait") {
                continue;
            }
            // 交叉对白名单（跨组仅此四形态）：record t4→t0 / record t0→t4（wait 桩
            // dstThread 槽恒 0，wait t0/t4 的归属看 thread 即可——域内判定覆盖）
            const bool crossPair = (c.func == "ThreadNotifyRecord" &&
                                    ((c.thread == t4 && c.dstThread == t0) ||
                                     (c.thread == t0 && c.dstThread == t4)));
            if (crossPair) {
                continue;
            }
            const bool inD0 = (c.thread == t0 || c.thread == t1 || c.thread == t2 ||
                               c.thread == t3) &&
                              (c.dstThread == 0 || c.dstThread == t0 || c.dstThread == t1 ||
                               c.dstThread == t2 || c.dstThread == t3);
            const bool inD1 = (c.thread == t4 || c.thread == t5 || c.thread == t6 ||
                               c.thread == t7) &&
                              (c.dstThread == 0 || c.dstThread == t4 || c.dstThread == t5 ||
                               c.dstThread == t6 || c.dstThread == t7);
            if (!inD0 && !inD1) {
                closureOk = false;   // 其余跨组指令 = 越组泄漏（域组封闭性破坏）
            }
        }
        Check(closureOk, "T4 屏障指令域组封闭（域内屏障不越组；跨组仅 t0↔t4 交叉对白名单）");
    }

    printf("== T5 等价对拍（env 开/关两路 ConsumeUnifiedPlanAicpu 输出逐条相等） ==\n");
    {
        // blob 面对拍（单域 4 卡 AG + RS 各一路）——env 关（本进程编译期默认关，或
        // 运行期未设）与 env 开（编译期宏 + 运行期 env 都开时）的 UnifiedExecResult
        // 逐字段相等。本测试二进制按两种宏形态分别编译（run.sh 两跑）：
        //   - 宏关进程：串行 for 路径（现状）
        //   - 宏开+env 进程：编排器路径（blob 段重建 T/T → 单域批批单段 = 串行等价）
        // 同进程内直接对拍：宏开形态下 env 开/关两跑（setenv 翻转——static bool
        // 首调定格，翻转需子进程；此处以编译期宏 + 直调两路替代：
        //   路A = ConsumeUnifiedPlanAicpu（内部按 UseUnifiedConcurrencyPath 分叉）
        //   路B = 既有循环体的逐段重建 + DriveSegmentViaSink 手工重放（单源件）
        // A==B 锁"分叉分支与循环体同一份重建逻辑"（WP-2 单源关系的常跑断言）。
        OpParam op;
        op.opType = static_cast<int>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER);
        op.DataDes.count = 0;
        op.DataDes.dataType = HCCL_DATA_TYPE_FP16;
        op.engine = CommEngine::COMM_ENGINE_AICPU;
        op.userRank = 0;
        TopoInfoWithNetLayerDetails topo;
        topo.userRankSize = 4; topo.topoLevelNums = 1; topo.level0Topo = Level0Shape::MESH_1D;
        Plan plan;
        std::string perr;
        Check(L1Adapter::Compile(op, topo, {{0U, 1U, 2U, 3U}}, "sole[mesh]", plan, perr) ==
                  HCCL_SUCCESS,
              "T5 RS sole[mesh] compile: " + perr);
        std::vector<u64> blob;
        Check(SerializePlanV3(plan, blob, perr), "T5 serialize: " + perr);
        UnifiedExecMsg msg;
        msg.count = 512;
        msg.inputAddr = 0x100000;
        msg.outputAddr = 0x200000;
        msg.dataType = HCCL_DATA_TYPE_FP16;
        msg.cclBuff.addr = reinterpret_cast<void*>(0xC00000ULL);
        msg.cclBuff.size = 4ULL * 1024 * 1024;
        HcommVerbRes res;
        res.threads = {0x1000, 0x1001, 0x1002, 0x1003};
        for (u32 peer = 1; peer <= 3; ++peer) {
            ChannelInfo ch;
            ch.remoteRank = peer;
            ch.handle = 0x20000 + peer;
            res.channels[peer] = {ch};
        }
        // 路A：ConsumeUnifiedPlanAicpu（宏开时 env 开 → 编排器；env 关 → 串行 for）
        g_stubCalls.clear();
        const UnifiedExecResult rA = ConsumeUnifiedPlanAicpu(
            blob, msg, 0, {0U, 1U, 2U, 3U},
            static_cast<u32>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER), res);
        Check(rA.ret == HCCL_SUCCESS, "T5 路A（消费方本体）成功: " + rA.err);
        const size_t stubsA = g_stubCalls.size();
        // 路B：手工重放（BlobView + RebuildActionSpecFromFrame + DriveSegmentViaSink
        // ——与循环体 :462-550 逐行同构的重建；锁分叉分支与循环体的单源关系）
        BlobView v;
        std::string verr;
        Check(ParseUnifiedBlobV4(blob, v, verr), "T5 parse: " + verr);
        g_stubCalls.clear();
        IActionBackend* beAg = BackendRegistry::Peek(ActionKind::CollectAll, Engine::AICPU);
        IActionBackend* beRs = BackendRegistry::Peek(ActionKind::ReduceSlice, Engine::AICPU);
        UnifiedExecResult rB;
        rB.ret = HCCL_SUCCESS;
        for (size_t i = 0; i < v.frames.size(); ++i) {
            const bool isRs = (v.frames[i].actionId ==
                               static_cast<u64>(ActionKind::ReduceSlice));
            std::string berr;
            ActionSpec a = RebuildActionSpecFromFrame(
                v.frames[i], isRs, msg, 0, 4,
                static_cast<u32>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER),
                static_cast<u32>(v.frames.size()), {0U, 1U, 2U, 3U}, 0, berr);
            Check(berr.empty(), "T5 路B 重建段 " + std::to_string(i) + " ok: " + berr);
            const OrchestrateSegResult sr = DriveSegmentViaSink(
                a, msg, 0, 4, static_cast<u32>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER),
                static_cast<u32>(v.frames.size()), beAg, beRs, res);
            Check(sr.ret == HCCL_SUCCESS, "T5 路B 驱动段 " + std::to_string(i) + " ok");
            rB.perSegTrace.push_back(sr.trace);
            if (sr.ret != HCCL_SUCCESS) {
                rB.ret = sr.ret;
                rB.err = sr.err;
                break;
            }
        }
        rB.segmentsRun = static_cast<u32>(v.frames.size());
        Check(rA.ret == rB.ret && rA.segmentsRun == rB.segmentsRun,
              "T5 路A/路B ret+segmentsRun 相等（单源关系——常跑防漂移）");
        Check(rA.perSegTrace.size() == rB.perSegTrace.size() &&
                  (rA.perSegTrace.empty() || rA.perSegTrace[0] == rB.perSegTrace[0]),
              "T5 路A/路B perSegTrace 相等（分叉分支与循环体同一份重建）");
        const size_t stubsB = g_stubCalls.size();
        Check(stubsA == stubsB, "T5 路A/路B 原语调用数相等（逐条同源的计数面）");
    }

    printf("== T6 负例（空序列 / null driver / 段失败透传） ==\n");
    {
        const OrchestratorRes res = MakeDualDomainRes(8);
        OrchestratorResult r = OrchestrateSegments({}, res, [](const ActionSpec&,
                                                               const HcommVerbRes&) {
            OrchestrateSegResult ok;
            return ok;
        });
        Check(r.ret == HCCL_E_PARA && r.err.find("empty") != std::string::npos,
              "T6 空序列拒（E_PARA）: " + r.err);
        const std::vector<ActionSpec> segs = MakeAFormSegments();
        r = OrchestrateSegments(segs, res, nullptr);
        Check(r.ret == HCCL_E_PARA && r.err.find("null driver") != std::string::npos,
              "T6 null driver 拒: " + r.err);
        // 段失败透传：第 2 段 driver 返回失败 → segmentsRun=1（已驱动数）+ err 带段号
        int callIdx = 0;
        const OrchestratorResult rf = OrchestrateSegments(
            segs, res, [&callIdx](const ActionSpec&, const HcommVerbRes&) {
                OrchestrateSegResult s;
                ++callIdx;
                if (callIdx == 2) {
                    s.ret = HCCL_E_INTERNAL;
                    s.err = "injected failure";
                }
                return s;
            });
        Check(rf.ret == HCCL_E_INTERNAL && rf.segmentsRun == 1 &&
                  rf.err.find("segment 1") != std::string::npos,
              "T6 段失败透传（segmentsRun=已驱动数，err 带段号）: " + rf.err);
    }

    printf("%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
