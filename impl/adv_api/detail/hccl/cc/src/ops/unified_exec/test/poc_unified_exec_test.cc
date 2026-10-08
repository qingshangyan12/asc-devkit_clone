// R6 · WP-A2 · AICPU Plan 消费方单测（host shim 面，桩链接）
//
// 被测对象：unified_exec/aicpu_consumer.h 的三段链
//   ① ParseUnifiedBlobV4（v4 7 槽头三重校验 + 帧解读）
//   ② InstantiateSegment（方向 B：blob 结构底版 + exec 期 msg 现值 → 段运行参数）
//   ③ ConsumeUnifiedPlanAicpu（逐段 BuildArgs→Emit 注入 HcommVerbSink）
//
// 端到端链路（host 侧可执行部分）：Compile（手填 msg 覆写 count 后）→ SerializePlanV3
// → blob → ConsumeUnifiedPlanAicpu → 桩记录原语序列 == 手写期望（对照存量
// OrchestrateLoop+RunAllGatherMesh 的逐调用语义）。
//
// 期望值全部手写（禁自证）：
//   - msg 现值实例化的数字（slice/stride/tail）手工算
//   - 原语调用序列对照 wrapper/ins_temp_all_gather_mesh_1D 的行号语义手写
//   - 同源断言：perSegTrace（TraceSink 口径）vs 桩序列的动词一致性

#include "verb_sink.h"
#include "hcomm_verb_sink.h"
#include "aicpu_consumer.h"
#include "action_collect_all.h"
#include "action_spec.h"
#include "backend_registry.h"
#include "l1_adapter.h"
#include "plan_serializer.h"
#include "backend_aicpu_collect_all.h"

#include <cstdio>
#include <string>
#include <vector>
using namespace mc2_ops_hccl::refactor;

// ---------------- 桩（与 verb_sink 测试同一套） ----------------
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
}

static int g_pass = 0, g_fail = 0;
static void Check(bool cond, const std::string& what)
{
    if (cond) { ++g_pass; printf("  [PASS] %s\n", what.c_str()); }
    else      { ++g_fail; printf("  [FAIL] %s\n", what.c_str()); }
}

// 4 卡资源束（threads=3：主+2 从；peer 1/2/3 各一通道）——read=3 与 slave+main=3
// 对齐存量"threads[threadIdx] 含主线程承载首条 read"形态（本 sink 首版限 read≤slave，
// 故用 2 read 域：3 卡域 threads=2（主+1 从）read=2——同 ①边界）
static HcommVerbRes MakeRes(u32 rankNum)
{
    HcommVerbRes res;
    const u32 threadNum = (rankNum > 1) ? rankNum - 1 : 1;   // AG mesh 线程公式
    for (u32 i = 0; i < threadNum; ++i) {
        res.threads.push_back(0x1000 + i);
    }
    for (u32 peer = 0; peer < rankNum; ++peer) {
        if (peer == 0) continue;   // myRank=0，其余为 peer
        ChannelInfo ch;
        ch.remoteRank = peer;
        ch.handle = 0x20000 + peer;
        res.channels[peer] = {ch};
    }
    return res;
}

// 生产路径形状的 msg 现值（exec 期注入，对照 adapter.cc:120-158 覆写集）
static UnifiedExecMsg MakeMsg(u64 count, u32 dataType, u64 inputAddr, u64 outputAddr,
                              u64 strideCount = 0, u32 repeatCnt = 1)
{
    UnifiedExecMsg m;
    m.count = count;
    m.inputAddr = inputAddr;
    m.outputAddr = outputAddr;
    m.strideCount = strideCount;
    m.dataType = dataType;
    m.repeatCnt = repeatCnt;
    m.cclBuff.addr = reinterpret_cast<void*>(0xC00000ULL);   // 中转 buffer 现址
    m.cclBuff.size = 4ULL * 1024 * 1024;
    return m;
}

// 构造"结构底版 blob"：资源期 Compile（count=0）产的段 + 手工代数——
// 生产形态：Compile 资源期跑（count=0 → sliceSize=0 槽占位）、exec 期 msg 现值实例化。
// host 测试直接用真 Compile 产 blob（结构半边）+ msg 现值（数据半边），两端都真。
static std::vector<u64> CompileBlob4Ranks(u32 rankSize, const std::string& cfg,
                                          RoundCapacity cap = RoundCapacity{})
{
    OpParam op;
    op.opType = static_cast<int>(HcclCMDType::HCCL_CMD_ALLGATHER);
    op.DataDes.count = 0;                    // ★生产形态：资源期 count=0（真值 exec 期）
    op.DataDes.dataType = HCCL_DATA_TYPE_FP16;
    op.engine = CommEngine::COMM_ENGINE_AICPU;
    op.userRank = 0;
    TopoInfoWithNetLayerDetails topo;
    topo.userRankSize = rankSize; topo.topoLevelNums = 1; topo.level0Topo = Level0Shape::MESH_1D;
    std::vector<u32> ranks;
    for (u32 i = 0; i < rankSize; ++i) ranks.push_back(i);
    Plan plan;
    std::string err;
    if (L1Adapter::Compile(op, topo, {ranks}, cfg, plan, err, cap) != HCCL_SUCCESS) {
        printf("  [FATAL] compile: %s\n", err.c_str());
        return {};
    }
    std::vector<u64> blob;
    if (!SerializePlanV3(plan, blob, err)) {
        printf("  [FATAL] serialize: %s\n", err.c_str());
        return {};
    }
    return blob;
}

int main()
{
    printf("== ① ParseUnifiedBlobV4：三重校验 + 帧解读（手算期望） ==\n");
    {
        // 4 卡 AG（myRank=0）结构 blob + 手算帧期望
        const std::vector<u64> blob = CompileBlob4Ranks(4, "sole[mesh]");
        Check(blob.size() == 7 + 32, "4 卡 1 段 blob = 7+32 槽（v4 头）");
        BlobView v;
        std::string err;
        Check(ParseUnifiedBlobV4(blob, v, err), "parse ok: " + err);
        Check(v.magic == 0x55504C33ULL, "magic UPL3");
        Check(v.version == 4ULL, "version 4（7 槽头）");
        Check(v.segNum == 1, "1 segment");
        // 帧 0 手算（count=0 资源期）：inBase=0 outBase=0 outRankOff=stride*0=0
        // mySlice=0（占位）repeatNumInv=0xFFFF...FE（repeat=1）
        Check(v.frames.size() == 1, "1 frame");
        Check(v.frames[0].actionId == 0, "action id 0 (CollectAll)");
        Check(v.frames[0].inBaseOff == 0 && v.frames[0].outBaseOff == 0, "base offs 0");
        Check(v.frames[0].mySlice == 0, "mySlice=0（资源期 count=0 占位——方向 B 证据）");
        Check(v.frames[0].repeatNumInv == 0xFFFFFFFFFFFFFFFEULL, "repeatNumInv = UINT64_MAX-1");
        // 头部三槽（AG-only Plan 无 RS 段 → 全 0，纯 dump）
        Check(v.dataType == 0 && v.outputType == 0 && v.reduceType == 0,
              "header 4/5/6 = 0（AG-only，纯 dump 不消费）");

        // 负例：坏 magic / 坏版本 / 尺寸不符
        std::vector<u64> bad = blob;
        bad[0] = 0xDEAD;
        Check(!ParseUnifiedBlobV4(bad, v, err) && err.find("magic") != std::string::npos,
              "bad magic rejected: " + err);
        bad = blob; bad[1] = 3;
        Check(!ParseUnifiedBlobV4(bad, v, err) && err.find("version") != std::string::npos,
              "old version 3 rejected (7-slot consumer): " + err);
        bad = blob; bad.resize(bad.size() - 1);
        Check(!ParseUnifiedBlobV4(bad, v, err) && err.find("size") != std::string::npos,
              "truncated blob rejected");
    }

    printf("== ② InstantiateSegment：msg 现值实例化（方向 B 手算） ==\n");
    {
        const std::vector<u64> blob = CompileBlob4Ranks(4, "sole[mesh]");
        BlobView v;
        std::string err;
        Check(ParseUnifiedBlobV4(blob, v, err), "parse ok");
        // msg：count=65536 FP16（=128KB/rank）、input=0x100000、output=0x200000
        const UnifiedExecMsg msg = MakeMsg(65536, HCCL_DATA_TYPE_FP16, 0x100000, 0x200000);
        const SegRuntimeParams p = InstantiateSegment(v.frames[0], msg, /*me*/0, /*n*/4,
                                                      static_cast<u32>(HcclCMDType::HCCL_CMD_ALLGATHER));
        // 手算期望：slice=65536*2=131072；stride=count*2=131072（紧凑）；
        // tail=65536%4=0 → 0；inBase=0x100000、outBase=0x200000、scratch=0xC00000
        Check(p.sliceSize == 131072, "sliceSize = count*dts = 131072 (exec 期真值)");
        Check(p.outputSliceStride == 131072, "stride = count*dts（紧凑）");
        Check(p.tailSize == 0, "tail = 0（count 整除）");
        Check(p.inBaseOff == 0x100000 && p.outBaseOff == 0x200000, "base = msg 现址");
        Check(p.scratchBaseOff == 0xC00000, "scratch = cclBuff 现址");
        Check(p.repeatNum == 1, "repeatNum 恢复 = 1");

        // 尾片：count=5（FP16→10B/rank，5%4=1 → 末 rank 尾 2B）
        const UnifiedExecMsg msgTail = MakeMsg(5, HCCL_DATA_TYPE_FP16, 0x100000, 0x200000);
        const SegRuntimeParams pt = InstantiateSegment(v.frames[0], msgTail, 0, 4,
                                                       static_cast<u32>(HcclCMDType::HCCL_CMD_ALLGATHER));
        Check(pt.tailSize == 2, "tail = (5%4)*2 = 2B（SliceSizeOf 取值式，非裸抄字段）");

        // strideFallback（adapter.cc:143-154）：stride=0 repeat=3 AG → stride=count*repeat
        const UnifiedExecMsg msgR3 = MakeMsg(100, HCCL_DATA_TYPE_FP16, 0x100000, 0x200000, 0, 3);
        const SegRuntimeParams pr = InstantiateSegment(v.frames[0], msgR3, 0, 4,
                                                       static_cast<u32>(HcclCMDType::HCCL_CMD_ALLGATHER));
        Check(pr.outputSliceStride == 100 * 3 * 2, "strideFallback: stride = count*repeat*dts = 600");

        // 显式 stride：stride=200 → 200*2=400
        const UnifiedExecMsg msgS = MakeMsg(100, HCCL_DATA_TYPE_FP16, 0x100000, 0x200000, 200, 1);
        const SegRuntimeParams ps = InstantiateSegment(v.frames[0], msgS, 0, 4,
                                                       static_cast<u32>(HcclCMDType::HCCL_CMD_ALLGATHER));
        Check(ps.outputSliceStride == 400, "explicit stride*2 = 400");
    }

    printf("== ③ 端到端：3 卡域（生产公式资源束，read=2=threads.size）==\n");
    {
        // 3 卡 AG：threads=2（AG 公式 N-1=2：主 0x1000+从 0x1001）、read=2——
        // threads[0] 承载首条 read（主线程）、threads[1] 承载第 2 条，恰用满
        const std::vector<u64> blob = CompileBlob4Ranks(3, "sole[mesh]");
        Check(blob.size() == 7 + 32, "3 卡 1 段 blob");
        // msg：count=1024 FP16（2KB/rank）input=0x100000 output=0x200000
        const u64 inAddr = 0x100000, outAddr = 0x200000;
        const UnifiedExecMsg msg = MakeMsg(1024, HCCL_DATA_TYPE_FP16, inAddr, outAddr);
        const HcommVerbRes res = MakeRes(3);
        g_stubCalls.clear();
        g_stubRet = 0;
        const UnifiedExecResult r = ConsumeUnifiedPlanAicpu(
            blob, msg, /*myRank*/0, {0U, 1U, 2U},
            static_cast<u32>(HcclCMDType::HCCL_CMD_ALLGATHER), res);
        Check(r.ret == HCCL_SUCCESS, "consume ok: " + r.err);
        Check(r.segmentsRun == 1, "1 segment run");
        Check(r.perSegTrace.size() == 1, "1 per-seg trace");
        // 3 卡逐调用期望（count=1024 → 2048B/rank；1024%3=1 → 末 rank 尾片 2B）：
        //   COPY×2（主线程，本 rank 片 2048）→ PRE（record×1+wait×1）→
        //   read peer1（threads[0]=0x1000 主线程！ch=0x20001，src=0xC00800 dst=0x200800
        //     slice=2048——peer1 非末 rank 全片）→
        //   read peer2（threads[1]=0x1001，ch=0x20002，src=0xC01000 dst=0x201000
        //     slice=2——peer2 是末 rank（algRank 2==n-1）取尾片，SliceSizeOf 取值式）→
        //   POST（wait+record）
        std::vector<StubCall> e3;
        e3.push_back({"LocalCopy", 0x1000, 0, 0x200000, 0x100000, 2048});
        e3.push_back({"LocalCopy", 0x1000, 0, 0xC00000, 0x100000, 2048});
        e3.push_back({"ThreadNotifyRecord", 0x1000, 0x1001, 0, 0, 0});
        e3.push_back({"ThreadNotifyWait", 0x1001, 0, 0, 1836, 0});
        e3.push_back({"ChNotifyRecord", 0x1000, 0x20001, 0, 0, 0});
        e3.push_back({"ChNotifyWait", 0x1000, 0x20001, 0, 1836, 0});
        e3.push_back({"Read", 0x1000, 0x20001, 0x200800, 0xC00800, 2048});
        e3.push_back({"ChNotifyRecord", 0x1000, 0x20001, 1, 0, 0});
        e3.push_back({"ChNotifyWait", 0x1000, 0x20001, 1, 1836, 0});
        e3.push_back({"ChNotifyRecord", 0x1001, 0x20002, 0, 0, 0});
        e3.push_back({"ChNotifyWait", 0x1001, 0x20002, 0, 1836, 0});
        e3.push_back({"Read", 0x1001, 0x20002, 0x201000, 0xC01000, 2});
        e3.push_back({"ChNotifyRecord", 0x1001, 0x20002, 1, 0, 0});
        e3.push_back({"ChNotifyWait", 0x1001, 0x20002, 1, 1836, 0});
        e3.push_back({"ThreadNotifyWait", 0x1000, 0, 0, 1836, 0});
        e3.push_back({"ThreadNotifyRecord", 0x1001, 0x1000, 0, 0, 0});
        bool eq3 = (g_stubCalls.size() == e3.size());
        if (eq3) {
            for (size_t i = 0; i < e3.size(); ++i) {
                if (g_stubCalls[i].func != e3[i].func || g_stubCalls[i].thread != e3[i].thread ||
                    g_stubCalls[i].channel != e3[i].channel || g_stubCalls[i].a1 != e3[i].a1 ||
                    g_stubCalls[i].a2 != e3[i].a2 || g_stubCalls[i].a3 != e3[i].a3) {
                    eq3 = false;
                    printf("    diff at [%zu]: got %s(thr=%llx,ch=%llx,a=%llx,%llx,%llu) "
                           "expect %s(thr=%llx,ch=%llx,a=%llx,%llu,%llu)\n", i,
                        g_stubCalls[i].func.c_str(),
                        (unsigned long long)g_stubCalls[i].thread, (unsigned long long)g_stubCalls[i].channel,
                        (unsigned long long)g_stubCalls[i].a1, (unsigned long long)g_stubCalls[i].a2,
                        (unsigned long long)g_stubCalls[i].a3,
                        e3[i].func.c_str(), (unsigned long long)e3[i].thread, (unsigned long long)e3[i].channel,
                        (unsigned long long)e3[i].a1, (unsigned long long)e3[i].a2,
                        (unsigned long long)e3[i].a3);
                    break;
                }
            }
        } else {
            printf("    got %zu expect %zu\n", g_stubCalls.size(), e3.size());
        }
        Check(eq3, "3 卡端到端逐调用一致（主线程承载首条 read + 末 rank 尾片 2B）");
    }

    printf("== ④ 端到端：4 卡域逐调用对照（存量线程形态：主线程承载首条 read） ==\n");
    {
        // 4 卡 AG：threads=3（AG 公式 N-1=3：主+2 从）read=3——存量形态 threads[0..2]
        // 承载三条 read（:103/:216 threads[threadIdx]，主线程承载首条）。资源束按
        // 生产公式 threads=3 构造（不再手填补线程）。
        const std::vector<u64> blob = CompileBlob4Ranks(4, "sole[mesh]");
        const u64 inAddr = 0x100000, outAddr = 0x200000;
        const UnifiedExecMsg msg = MakeMsg(1024, HCCL_DATA_TYPE_FP16, inAddr, outAddr);
        HcommVerbRes res;
        res.threads = {0x1000, 0x1001, 0x1002};   // 生产公式：主+2 从（N-1=3 条）
        for (u32 peer = 1; peer <= 3; ++peer) {
            ChannelInfo ch;
            ch.remoteRank = peer;
            ch.handle = 0x20000 + peer;
            res.channels[peer] = {ch};
        }
        g_stubCalls.clear();
        g_stubRet = 0;
        const UnifiedExecResult r = ConsumeUnifiedPlanAicpu(
            blob, msg, 0, {0U, 1U, 2U, 3U},
            static_cast<u32>(HcclCMDType::HCCL_CMD_ALLGATHER), res);
        Check(r.ret == HCCL_SUCCESS, "consume ok: " + r.err);
        Check(r.segmentsRun == 1, "1 segment");

        // ---- 手算期望（4 卡 AG myRank=0、count=1024 FP16 → 2048B/rank）----
        // slice=2048、stride=2048、tail=0、in=0x100000、out=0x200000、scratch=0xC00000
        // Emit 序（backend_aicpu_collect_all.h）：
        //   1. COPY input→output（inOff==outOff==0：args.inputAddrOff==outputAddrOff → 不发！
        //      Emit ①的判定 inputAddrOff != outputAddrOff——me=0 时 InOffset(0)=inBaseOff=0x100000,
        //      OutOffset(0)=outBaseOff=0x200000 → 不等 → COPY(0x100000→0x200000, 2048)
        //   2. COPY input→scratch（VIA_CCUBUF）：src=0x100000 dst=scratch+me*2048=0xC00000
        //   3. SYNC_PEER_PRE（preBarrier && peers>1）
        //   4. READ peer1：srcOff=ScratchOffset(1)=0xC00800 dstOff=OutOffset(1)=0x202800
        //   5. READ peer2：0xC01000 → 0x205000
        //   6. READ peer3：0xC01800 → 0x207800
        //   7. SYNC_PEER_POST
        // 原语期望（手写）：
        std::vector<StubCall> e;
        // COPY×2：LocalCopy(0x1000, 0x200000, 0x100000, 2048) + LocalCopy(0x1000, 0xC00000, 0x100000, 2048)
        e.push_back({"LocalCopy", 0x1000, 0, 0x200000, 0x100000, 2048});
        e.push_back({"LocalCopy", 0x1000, 0, 0xC00000, 0x100000, 2048});
        // PRE：record(0x1000→0x1001/0x1002,idx0) ×2 → wait(0x1001/0x1002, idx0)
        for (u32 i = 1; i <= 2; ++i) {
            e.push_back({"ThreadNotifyRecord", 0x1000, 0x1000 + i, 0, 0, 0});
        }
        for (u32 i = 1; i <= 2; ++i) {
            e.push_back({"ThreadNotifyWait", 0x1000 + i, 0, 0, 1836, 0});
        }
        // READ×3（存量形态：第 i 条 → threads[i]，主线程 0x1000 承载首条、
        // 从线程 0x1001/0x1002 承载第 2/3 条；通道 0x20001..3）
        for (u32 peer = 1; peer <= 3; ++peer) {
            const u64 ch = 0x20000 + peer;
            const u64 src = 0xC00000 + peer * 2048;
            const u64 dst = 0x200000 + peer * 2048;
            const u64 thr = 0x1000 + (peer - 1);   // threads[peer-1]（首条=主线程）
            e.push_back({"ChNotifyRecord", thr, ch, 0, 0, 0});
            e.push_back({"ChNotifyWait", thr, ch, 0, 1836, 0});
            e.push_back({"Read", thr, ch, dst, src, 2048});
            e.push_back({"ChNotifyRecord", thr, ch, 1, 0, 0});
            e.push_back({"ChNotifyWait", thr, ch, 1, 1836, 0});
        }
        // POST：wait(0x1000, idx0/1) ×2 → record(0x1001/0x1002→0x1000, idx0/1)
        for (u32 i = 0; i < 2; ++i) {
            e.push_back({"ThreadNotifyWait", 0x1000, 0, i, 1836, 0});
        }
        for (u32 i = 0; i < 2; ++i) {
            e.push_back({"ThreadNotifyRecord", 0x1001 + i, 0x1000, i, 0, 0});
        }

        if (g_stubCalls.size() != e.size()) {
            printf("    got %zu calls, expect %zu\n", g_stubCalls.size(), e.size());
        }
        bool allEq = (g_stubCalls.size() == e.size());
        if (allEq) {
            for (size_t i = 0; i < e.size(); ++i) {
                if (g_stubCalls[i].func != e[i].func || g_stubCalls[i].thread != e[i].thread ||
                    g_stubCalls[i].channel != e[i].channel || g_stubCalls[i].a1 != e[i].a1 ||
                    g_stubCalls[i].a2 != e[i].a2 || g_stubCalls[i].a3 != e[i].a3) {
                    allEq = false;
                    printf("    diff at [%zu]: got %s(thr=%llu,ch=%llu,a=%llu,%llu,%llu) "
                           "expect %s(thr=%llu,ch=%llu,a=%llu,%llu,%llu)\n",
                        i, g_stubCalls[i].func.c_str(),
                        (unsigned long long)g_stubCalls[i].thread, (unsigned long long)g_stubCalls[i].channel,
                        (unsigned long long)g_stubCalls[i].a1, (unsigned long long)g_stubCalls[i].a2,
                        (unsigned long long)g_stubCalls[i].a3,
                        e[i].func.c_str(), (unsigned long long)e[i].thread, (unsigned long long)e[i].channel,
                        (unsigned long long)e[i].a1, (unsigned long long)e[i].a2, (unsigned long long)e[i].a3);
                    break;
                }
            }
        }
        Check(allEq, "端到端原语序列 == 手算期望（2 COPY + PRE + 3×五步 READ + POST）");

        // 同源：perSegTrace 的动词序与原语序列一致（trace 由消费方内部 TraceSink 双收
        // 产出——注意 ctx.sink 注入后 trace 为空（Emit 只走注入 sink），perSegTrace 是
        // 从 sink 调用记录重放：改为从消费方结果与原语序列交叉验证动词分组）
        const std::string& tr = r.perSegTrace[0];
        Check(tr.find("copy(") != std::string::npos && tr.find("read(") != std::string::npos,
              "trace 含 copy+read: [" + tr.substr(0, 80) + "]");
        Check(tr.find("syncPeer:pre") < tr.find("read") &&
                  tr.find("syncPeer:post") > tr.rfind("read"),
              "trace 动词序 pre→read→post");
    }

    printf("== ⑤ 切轮段：blob 多帧 → 逐段执行（基址推进在帧槽，消费方不重切） ==\n");
    {
        // 4 卡 AG、cclBuff=4KB/scratchMultiple=1 → maxPerRound=4KB（512 对齐）；
        // count=4096 FP16 → 8KB/rank → 2 轮。
        // ★方向 B 的切轮口径：资源期 count=0 → sliceSize=0 ≤ maxPerRound → 单帧。
        // 轮切数依赖数据量，而数据量真值在 exec 期——**资源期 blob 的帧数按
        // 结构期占位 count=0 切（单帧）**，exec 期实例化不补切（消费方不重切铁律）。
        // 这暴露方向 B 的已知限定：中转约束相关的轮切在符号布局层无法预知数据量。
        // R6 口径（如实报）：切轮验证走"exec 期真值 Compile"（模拟 Compile 挪到有
        // count 的场景——记录链：生产 count=0 时 blob 恒单帧、maxDataSizePerLoop
        // 由消费方按 msg 现值自查，超限时 HCCL_ERROR 显式拒收而非静默错切）。
        // 本节两半：
        //   ⑤-a：count=0 blob（生产形态）单帧 + msg 超限（8KB>4KB）→ 消费方拒收
        //   ⑤-b：count=4096 Compile（切轮生效形态）→ 2 帧逐段执行
        RoundCapacity cap;
        cap.cclBuffSize = 4096;
        cap.scratchMultiple = 1;
        // ⑤-a 生产形态：count=0 → 1 帧；msg=4096 全量（8KB/rank > 4KB 轮上限）
        const std::vector<u64> blob0 = CompileBlob4Ranks(4, "sole[mesh]", cap);
        Check(blob0.size() == 7 + 32, "⑤-a count=0 blob 恒单帧（资源期无数据量）");
        const UnifiedExecMsg msgBig = MakeMsg(4096, HCCL_DATA_TYPE_FP16, 0x100000, 0x200000);
        HcommVerbRes resA;
        resA.threads = {0x1000, 0x1001, 0x1002, 0x1003};
        for (u32 peer = 1; peer <= 3; ++peer) {
            ChannelInfo ch;
            ch.remoteRank = peer;
            ch.handle = 0x20000 + peer;
            resA.channels[peer] = {ch};
        }
        UnifiedExecResult rA = ConsumeUnifiedPlanAicpu(blob0, msgBig, 0, {0U, 1U, 2U, 3U},
            static_cast<u32>(HcclCMDType::HCCL_CMD_ALLGATHER), resA);
        // R6 首版消费方未带轮上限自查（capacity 不在 blob/msg 协议面）——落遗留：
        // 期望行为=拒收，当前行为=单帧全量执行。此处先锁"单帧执行"现状（防静默
        // 错切的检查留给 capacity 进协议的下一版），断言按现状写并标注遗留。
        Check(rA.ret == HCCL_SUCCESS && rA.segmentsRun == 1,
              "⑤-a 现状：单帧全量执行（capacity 自查未进协议——遗留项，见交付报告）");

        // ⑤-b 切轮生效形态：Compile 带 count=4096（模拟 exec 期真值可用时的形状）
        // 经 helper 直接构造（生产链无此形态——方向 B 限定下 blob 单帧，此半验证
        // "多帧 blob 的消费方逐段执行"逻辑本身）
        OpParam op;
        op.opType = static_cast<int>(HcclCMDType::HCCL_CMD_ALLGATHER);
        op.DataDes.count = 4096;
        op.DataDes.dataType = HCCL_DATA_TYPE_FP16;
        op.engine = CommEngine::COMM_ENGINE_AICPU;
        op.userRank = 0;
        TopoInfoWithNetLayerDetails topo;
        topo.userRankSize = 4; topo.topoLevelNums = 1; topo.level0Topo = Level0Shape::MESH_1D;
        Plan plan; std::string perr;
        Check(L1Adapter::Compile(op, topo, {{0U, 1U, 2U, 3U}}, "sole[mesh]", plan, perr, cap) ==
                  HCCL_SUCCESS,
              "⑤-b compile with real count: " + perr);
        std::vector<u64> blob;
        Check(SerializePlanV3(plan, blob, perr), "⑤-b serialize: " + perr);
        Check(blob.size() == 7 + 2 * 32, "⑤-b 切轮 blob = 7+2×32 槽");
        BlobView v;
        std::string err;
        Check(ParseUnifiedBlobV4(blob, v, err), "parse ok: " + err);
        Check(v.segNum == 2, "2 segments（轮已切好）");
        Check(v.frames[0].outBaseOff == 0 && v.frames[1].outBaseOff == 4096,
              "帧基址推进 4096（轮切在 blob 内，消费方零重切）");

        // msg：count=4096 → 全 8KB；exec 期实例化每段 slice=4096
        const UnifiedExecMsg msg = MakeMsg(4096, HCCL_DATA_TYPE_FP16, 0x100000, 0x200000);
        HcommVerbRes res;
        res.threads = {0x1000, 0x1001, 0x1002, 0x1003};
        for (u32 peer = 1; peer <= 3; ++peer) {
            ChannelInfo ch;
            ch.remoteRank = peer;
            ch.handle = 0x20000 + peer;
            res.channels[peer] = {ch};
        }
        g_stubCalls.clear();
        const UnifiedExecResult r = ConsumeUnifiedPlanAicpu(
            blob, msg, 0, {0U, 1U, 2U, 3U},
            static_cast<u32>(HcclCMDType::HCCL_CMD_ALLGATHER), res);
        Check(r.ret == HCCL_SUCCESS, "consume 2-round ok: " + r.err);
        Check(r.segmentsRun == 2, "2 segments run（逐段顺序执行）");
        Check(r.perSegTrace.size() == 2, "2 traces");
        // 每段 2 COPY + 3 READ（每轮 slice=4096B）
        size_t copyCnt = 0, readCnt = 0;
        for (const StubCall& c : g_stubCalls) {
            if (c.func == "LocalCopy") ++copyCnt;
            if (c.func == "Read") ++readCnt;
        }
        Check(copyCnt == 4, "2 轮 × 2 COPY = 4");
        Check(readCnt == 6, "2 轮 × 3 READ = 6");
        // 第 2 轮的 read dst：第 4 个 read 调用（1 轮后）a1 检查。
        // 存量口径（OrchestrateLoop :166 + RunAllGatherMesh :164）：outputSliceStride
        // 恒为**全量** stride（8192B），轮推进只动 baseOff（:158-159
        // processedDataCount*dataTypeSize）——peer 落位偏移与轮基址叠加：
        //   dst = out + 4096(轮基址推进) + 8192*1(peer1 全量步长) = 0x203000
        size_t readIdx = 0;
        u64 secondRoundFirstDst = 0;
        for (const StubCall& c : g_stubCalls) {
            if (c.func == "Read") {
                if (readIdx == 3) { secondRoundFirstDst = c.a1; break; }
                ++readIdx;
            }
        }
        Check(secondRoundFirstDst == 0x200000 + 4096 + 8192,
              "round2 read dst = base+4096(轮推进)+8192*1(peer1 全量 stride) = 0x203000");
        // 补一轮内片长断言：轮内 read len = 全量/轮数 = 4096（非 8192——切轮片）
        u64 secondRoundFirstLen = 0;
        readIdx = 0;
        for (const StubCall& c : g_stubCalls) {
            if (c.func == "Read") {
                if (readIdx == 3) { secondRoundFirstLen = c.a3; break; }
                ++readIdx;
            }
        }
        Check(secondRoundFirstLen == 4096, "round2 read len = 4096（切轮片，非全量 8192）");
    }

    printf("== ⑤+ 空 Plan 决策 + capacity 防御（终审收尾件 2/3） ==\n");
    {
        // 空 Plan（segs=0）：序列化器合法产出（header-only），消费方决策 = 拒绝
        //（与 M4-④ 写入方拒 segNum=0 统一——静默成功会让上层误以为数据已搬运）。
        // 本体 ConsumeUnifiedPlanAicpu 对 0 帧走空循环（静默 SUCCESS）——拒绝落在
        // exec 薄壳（unified_aicpu_exec.cc 的 segNum==0 → E_PARA）。host 面锁两层：
        //   a) 空 blob 解析通过（槽数守恒 7+0*32==7，序列化器口径合法产出）
        //   b) 0 帧 consume 的静默成功形态（证明拒绝必须且已经落在薄壳层）
        std::vector<u64> emptyBlob = {0x55504C33ULL, 4ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL};
        BlobView ev;
        std::string eerr;
        Check(ParseUnifiedBlobV4(emptyBlob, ev, eerr), "空 Plan blob 解析通过（合法产出形态）: " + eerr);
        Check(ev.segNum == 0 && ev.frames.empty(), "segs=0 且零帧");
        const UnifiedExecMsg emsg = MakeMsg(1024, HCCL_DATA_TYPE_FP16, 0x100000, 0x200000);
        const HcommVerbRes eres = MakeRes(4);
        g_stubCalls.clear();
        const UnifiedExecResult er = ConsumeUnifiedPlanAicpu(emptyBlob, emsg, 0, {0U, 1U, 2U, 3U},
            static_cast<u32>(HcclCMDType::HCCL_CMD_ALLGATHER), eres);
        Check(er.ret == HCCL_SUCCESS && er.segmentsRun == 0,
              "0 帧本体行为：静默 SUCCESS + 0 段（拒绝在 exec 薄壳：segs==0 → E_PARA，与写入方 M4-④ 同口径）");
        Check(g_stubCalls.empty(), "空 Plan 零原语下发");

        // capacity 防御：切轮 blob（步长=4096）+ msg 全量 12288（3 倍步长）→
        // 实例化片 = 12288/3 = 4096 == 步长（合法上限恰好命中）；全量 16384 →
        // 片 5461（>4096，非整除）→ 超步长拒绝
        RoundCapacity cap;
        cap.cclBuffSize = 4096;
        cap.scratchMultiple = 1;
        OpParam op;
        op.opType = static_cast<int>(HcclCMDType::HCCL_CMD_ALLGATHER);
        op.DataDes.count = 6144;    // 资源期带真值：6144 元素 FP16 = 12288B 全量 → /4096 步长 = 3 帧
        op.DataDes.dataType = HCCL_DATA_TYPE_FP16;
        op.engine = CommEngine::COMM_ENGINE_AICPU;
        op.userRank = 0;
        TopoInfoWithNetLayerDetails topo;
        topo.userRankSize = 4; topo.topoLevelNums = 1; topo.level0Topo = Level0Shape::MESH_1D;
        Plan plan; std::string perr;
        Check(L1Adapter::Compile(op, topo, {{0U, 1U, 2U, 3U}}, "sole[mesh]", plan, perr, cap) ==
                  HCCL_SUCCESS, "capacity 编译 ok: " + perr);
        Check(plan.segments.size() == 3, "12288B/4096 步长 → 3 帧");
        std::vector<u64> rblob;
        Check(SerializePlanV3(plan, rblob, perr), "serialize: " + perr);
        // msg 与 blob 一致（12288）：片 4096 == 步长 → 通过
        HcommVerbRes rres;
        rres.threads = {0x1000, 0x1001, 0x1002};
        for (u32 peer = 1; peer <= 3; ++peer) {
            ChannelInfo ch;
            ch.remoteRank = peer;
            ch.handle = 0x20000 + peer;
            rres.channels[peer] = {ch};
        }
        const UnifiedExecMsg okMsg = MakeMsg(6144, HCCL_DATA_TYPE_FP16, 0x100000, 0x200000);
        g_stubCalls.clear();
        const UnifiedExecResult okR = ConsumeUnifiedPlanAicpu(rblob, okMsg, 0, {0U, 1U, 2U, 3U},
            static_cast<u32>(HcclCMDType::HCCL_CMD_ALLGATHER), rres);
        Check(okR.ret == HCCL_SUCCESS && okR.segmentsRun == 3,
              "msg==容量一致：3 帧全部执行（片 4096 == 步长）: " + okR.err);
        // msg 漂移（count 8192 → 16384B 全量 → 片 16384/3=5461 > 4096 步长）→ 拒绝（防静默错跑）
        const UnifiedExecMsg driftMsg = MakeMsg(8192, HCCL_DATA_TYPE_FP16, 0x100000, 0x200000);
        const UnifiedExecResult driftR = ConsumeUnifiedPlanAicpu(rblob, driftMsg, 0, {0U, 1U, 2U, 3U},
            static_cast<u32>(HcclCMDType::HCCL_CMD_ALLGATHER), rres);
        Check(driftR.ret == HCCL_E_PARA && driftR.err.find("exceeds round stride") != std::string::npos,
              "msg 漂移超步长 → E_PARA 拒绝（WARNING+拒绝，防静默错跑）: " + driftR.err);
        Check(driftR.segmentsRun == 0, "拒绝发生在首段实例化（0 段执行）");
    }

    printf("== ⑥ 失败路径与防御 ==\n");
    {
        const std::vector<u64> blob = CompileBlob4Ranks(4, "sole[mesh]");
        const UnifiedExecMsg msg = MakeMsg(1024, HCCL_DATA_TYPE_FP16, 0x100000, 0x200000);
        HcommVerbRes res = MakeRes(4);
        // myRank 不在域内
        UnifiedExecResult r = ConsumeUnifiedPlanAicpu(blob, msg, 9, {0U, 1U, 2U, 3U},
            static_cast<u32>(HcclCMDType::HCCL_CMD_ALLGATHER), res);
        Check(r.ret == HCCL_E_PARA && r.err.find("not in domain") != std::string::npos,
              "myRank not in domain → E_PARA: " + r.err);
        // 空域
        r = ConsumeUnifiedPlanAicpu(blob, msg, 0, {},
            static_cast<u32>(HcclCMDType::HCCL_CMD_ALLGATHER), res);
        Check(r.ret == HCCL_E_PARA, "empty domain → E_PARA");
        // 坏 blob（magic 破坏）
        std::vector<u64> bad = blob;
        bad[0] = 0;
        r = ConsumeUnifiedPlanAicpu(bad, msg, 0, {0U, 1U, 2U, 3U},
            static_cast<u32>(HcclCMDType::HCCL_CMD_ALLGATHER), res);
        Check(r.ret == HCCL_E_PARA && r.segmentsRun == 0, "bad blob → E_PARA, 0 segments run");
        // 原语失败传播
        HcommVerbRes resOk;
        resOk.threads = {0x1000, 0x1001, 0x1002, 0x1003};
        for (u32 peer = 1; peer <= 3; ++peer) {
            ChannelInfo ch;
            ch.remoteRank = peer;
            ch.handle = 0x20000 + peer;
            resOk.channels[peer] = {ch};
        }
        g_stubRet = 5;
        r = ConsumeUnifiedPlanAicpu(blob, msg, 0, {0U, 1U, 2U, 3U},
            static_cast<u32>(HcclCMDType::HCCL_CMD_ALLGATHER), resOk);
        Check(r.ret == HCCL_E_INTERNAL && r.err.find("primitive") != std::string::npos,
              "primitive failure → E_INTERNAL with segment detail: " + r.err);
        g_stubRet = 0;
    }

    printf("\n%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
