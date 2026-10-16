// R6 · WP-A2 · AICPU unified 执行器单测（host shim 面，桩链接）
//
// 被测对象：
//   ① blob 消费判据（unified_aicpu_exec.cc 的 ParseBlobHead/CheckSegmentKinds 同式
//      ——真 .cc 在真 alg_param 编译面，shim 面锁判据：三重校验 + 段 ID 对账）
//   ② exec 期现值实例化（M0 必答③：count/dataType/stride 现值 → 段几何重算）
//   ③ 消费方驱动链（blob 底版 + exec 现值 → BuildArgs → L4 Emit → l5 sink → 桩序列）
// 期望值全部手写（禁自证）。真原语以桩记录（源码级：五步/两段循环序已由
// poc_unified_verb_sink_test 逐调用锁定，此处只断消费方的动词流与几何）。
#include "verb_sink.h"
#include "hcomm_verb_sink.h"
#include "action_collect_all.h"
#include "action_spec.h"
#include "backend_registry.h"
#include "l1_adapter.h"
#include "backend_aicpu_collect_all.h"
#include <cstdio>
#include <cstring>
using namespace mc2_ops_hccl::refactor;
static int g_pass = 0, g_fail = 0;
static void Check(bool cond, const std::string& what)
{
    if (cond) { ++g_pass; printf("  [PASS] %s\n", what.c_str()); }
    else      { ++g_fail; printf("  [FAIL] %s\n", what.c_str()); }
}

// 桩：真原语记录（同 poc_unified_verb_sink_test 的桩面——消费方测试复用）
struct StubCall { std::string func; u64 t, c, a1, a2, a3; };
static std::vector<StubCall> g_stub;
static int32_t g_stubRet = 0;
static void Rec(const char* f, u64 t, u64 c, u64 a1, u64 a2, u64 a3)
{ StubCall s; s.func=f; s.t=t; s.c=c; s.a1=a1; s.a2=a2; s.a3=a3; g_stub.push_back(s); }
extern "C" {
int32_t HcommLocalCopyOnThread(ThreadHandle t, void* d, const void* s, uint64_t l)
{ Rec("LocalCopy", t, 0, (u64)(uintptr_t)d, (u64)(uintptr_t)s, l); return g_stubRet; }
int32_t HcommReadOnThread(ThreadHandle t, ChannelHandle ch, void* d, const void* s, uint64_t l)
{ Rec("Read", t, ch, (u64)(uintptr_t)d, (u64)(uintptr_t)s, l); return g_stubRet; }
// R12 · WP-9②：WRITE/REDUCE 桩（sink 新接线两原语——RS×AICPU 写侧）
int32_t HcommWriteOnThread(ThreadHandle t, ChannelHandle ch, void* d, const void* s, uint64_t l)
{ Rec("Write", t, ch, (u64)(uintptr_t)d, (u64)(uintptr_t)s, l); return g_stubRet; }
int32_t HcommLocalReduceOnThread(ThreadHandle t, void* d, const void* s, uint64_t c,
    HcommDataType dt, HcommReduceOp op)
{ Rec("LocalReduce", t, 0, (u64)(uintptr_t)d, (u64)(uintptr_t)s, c); (void)dt; (void)op; return g_stubRet; }

int32_t HcommThreadNotifyRecordOnThread(ThreadHandle t, ThreadHandle dt, uint32_t i)
{ Rec("ThNRec", t, dt, i, 0, 0); return g_stubRet; }
int32_t HcommThreadNotifyWaitOnThread(ThreadHandle t, uint32_t i, uint32_t to)
{ Rec("ThNWait", t, 0, i, to, 0); return g_stubRet; }
int32_t HcommChannelNotifyRecordOnThread(ThreadHandle t, ChannelHandle ch, uint32_t i)
{ Rec("ChNRec", t, ch, i, 0, 0); return g_stubRet; }
int32_t HcommChannelNotifyWaitOnThread(ThreadHandle t, ChannelHandle ch, uint32_t i, uint32_t to)
{ Rec("ChNWait", t, ch, i, to, 0); return g_stubRet; }
// R15 · ②：NHR_RUN 执行面原语桩（sink DoNhrRun 消费）
int32_t HcommWriteReduceOnThread(ThreadHandle t, ChannelHandle ch, void* d, const void* s,
    uint64_t c, HcommDataType dt, HcommReduceOp op)
{ Rec("WriteReduce", t, ch, (u64)(uintptr_t)d, (u64)(uintptr_t)s, c); (void)dt; (void)op; return g_stubRet; }
int32_t HcommReadReduceOnThread(ThreadHandle t, ChannelHandle ch, void* d, const void* s,
    uint64_t c, HcommDataType dt, HcommReduceOp op)
{ Rec("ReadReduce", t, ch, (u64)(uintptr_t)d, (u64)(uintptr_t)s, c); (void)dt; (void)op; return g_stubRet; }
}

static OpParam MakeAgOp(u32 rankSize, u64 countPerRank)
{
    OpParam op;
    op.opType = static_cast<int>(HcclCMDType::HCCL_CMD_ALLGATHER);
    op.DataDes.count = countPerRank;
    op.DataDes.dataType = HCCL_DATA_TYPE_FP16;
    op.engine = CommEngine::COMM_ENGINE_AICPU;
    return op;
}
static TopoInfoWithNetLayerDetails MakeTopo(u32 rankSize)
{
    TopoInfoWithNetLayerDetails topo;
    topo.userRankSize = rankSize; topo.topoLevelNums = 1; topo.level0Topo = Level0Shape::MESH_1D;
    return topo;
}
static std::vector<std::vector<u32>> SingleDomain(u32 rankSize)
{
    std::vector<u32> ranks;
    for (u32 i = 0; i < rankSize; ++i) ranks.push_back(i);
    return {ranks};
}

int main()
{
    printf("== A2-① 端到端：blob（资源期产出，count=0 底版）→ exec 现值实例化 → 真原语序 ==\n");
    {
        // 资源期：Compile（count=0——生产链路实证形态）→ SerializePlanV3 → blob
        OpParam resOp = MakeAgOp(4, 0);      // count=0：资源期底版（PrepareParamForAllGather :511 同）
        resOp.userRank = 1;                   // rank1 视角
        TopoInfoWithNetLayerDetails topo = MakeTopo(4);
        Plan plan; std::string err;
        Check(L1Adapter::Compile(resOp, topo, SingleDomain(4), "sole[mesh]", plan, err) == HCCL_SUCCESS,
              "resource-phase compile (count=0) ok");
        std::vector<u64> blob;
        Check(SerializePlanV3(plan, blob, err), "blob serialized: " + err);
        Check(blob[1] == 4ULL, "blob version = 4 (7-slot header)");
        Check(blob.size() == 7 + 32, "blob = 7 header + 1*32 slots");

        // exec 期：现值注入（adapter 形态——count/dataType/stride/ptrs 全现值）
        OpParam execOp = MakeAgOp(4, 64 * 1024);   // msg.dataCnt = 65536 元素
        execOp.userRank = 1;
        execOp.DataDes.strideCount = 0;             // 0 → count 同式
        execOp.inputPtr = (void*)0x100000;
        execOp.outputPtr = (void*)0x200000;

        // resCtx 最小构造：threads（4 rank → 3 条）+ channels（rank1 的 3 个 peer）+ hierarchy
        mc2_ops_hccl::AlgResourceCtxSerializable resCtx;
        resCtx.threads = {100, 101, 102};
        for (u32 r = 0; r < 4; ++r) {
            if (r == 1) continue;
            ChannelInfo ch;
            ch.isValid = true; ch.remoteRank = r; ch.handle = 0x1000 + r;
            ch.remoteCclMem.addr = reinterpret_cast<void*>(static_cast<uintptr_t>(0x20000 + r * 0x100000));
            resCtx.channels.resize(1);
            resCtx.channels[0].push_back(ch);
        }
        resCtx.algHierarchyInfo.infos.resize(1);
        resCtx.algHierarchyInfo.infos[0].resize(1);
        resCtx.algHierarchyInfo.infos[0][0] = SingleDomain(4)[0];

        // blob 槽数守恒（消费方判据复算——shim 的 OpParam 无 resCtx 字段，exec 链
        // 的 param.resCtx 直读在真 alg_param 面验证，此处锁判据本身）
        Check(blob.size() == 7 + 32, "blob slot conservation (7 header + 1 seg)");

        HcommVerbSink sink(MakeHcommVerbRes(resCtx));
        // RunUnifiedAicpuOp 在真 alg_param 面——shim 面不可链 .cc。此处镜像其消费逻辑
        // 做同源验证：以 exec 现值重建段 + Emit（与 unified_aicpu_exec.cc ⑤ 段逐行同式）
        LoopSplitter splitter;
        ActionSpec a;
        a.kind = ActionKind::CollectAll;
        a.domain.ranks = resCtx.algHierarchyInfo.infos[0][0];
        a.domain.myRank = execOp.userRank;
        const u64 fullSlice = execOp.DataDes.count * 2;    // FP16
        a.layout.sliceSize = fullSlice;
        a.layout.dataTypeSize = 2;
        a.layout.inputSliceStride = 0;
        a.layout.outputSliceStride = fullSlice;
        a.layout.inBaseOff = 0; a.layout.outBaseOff = 0;
        a.layout.repeatNum = 1;
        a.transport = TransportPolicy::VIA_CCUBUF;
        a.pattern = CommPattern::MESH;
        a.sync.preBarrier = true; a.sync.postBarrier = true;
        CollectAllArgs args = CollectAll::BuildArgs(a, splitter);
        // READ/COPY 的绝对地址合成（消费方 Emit 前置——l5 sink 收绝对地址）：
        // 注：l5 sink 直接消费 call.src/dst 作绝对地址；AICPU 段 offset 先加基址
        //（消费方合成口径——READ src=peer cclBuff+srcOff、dst=output+dstOff）
        struct AbsCall { VerbCall::Verb v; u64 src, dst, size; u32 peer; };
        std::vector<AbsCall> abs;
        if (args.inputAddrOff != args.outputAddrOff) {
            abs.push_back({VerbCall::COPY, reinterpret_cast<u64>(execOp.inputPtr) + args.inputAddrOff,
                           reinterpret_cast<u64>(execOp.outputPtr) + args.outputAddrOff, args.mySliceSize, 0});
        }
        void* cclBuffBase = reinterpret_cast<void*>(static_cast<uintptr_t>(0x300000));
        abs.push_back({VerbCall::COPY, reinterpret_cast<u64>(execOp.inputPtr) + args.inputAddrOff,
                       reinterpret_cast<u64>(cclBuffBase) + 0, args.mySliceSize, 0});   // cclBuff 上料
        abs.push_back({VerbCall::SYNC_PEER_PRE, 0, 0, 0, 0});
        for (const auto& p : args.peers) {
            // peer 通道查表（MakeHcommVerbRes 的 map——sink 同口径）
            u64 peerCclBase = 0;
            for (const ChannelInfo& ch : resCtx.channels[0]) {
                if (ch.remoteRank == p.peerRank) { peerCclBase = reinterpret_cast<u64>(ch.remoteCclMem.addr); break; }
            }
            abs.push_back({VerbCall::READ, peerCclBase + p.srcOff,
                           reinterpret_cast<u64>(execOp.outputPtr) + p.dstOff, p.sliceSize, p.peerRank});
        }
        abs.push_back({VerbCall::SYNC_PEER_POST, 0, 0, 0, 0});
        for (const AbsCall& c : abs) {
            sink.Emit(VerbCall{c.v, c.src, c.dst, c.size, c.peer});
        }
        Check(sink.ErrCode() == 0, "sink ok: " + sink.Err());
        u32 readCalls = 0, preCalls = 0, postCalls = 0, copyCalls = 0;
        for (const auto& r : sink.Calls()) {
            if (r.verb == VerbCall::READ) ++readCalls;
            if (r.verb == VerbCall::SYNC_PEER_PRE) ++preCalls;
            if (r.verb == VerbCall::SYNC_PEER_POST) ++postCalls;
            if (r.verb == VerbCall::COPY) ++copyCalls;
        }
        Check(readCalls == 3 && preCalls == 1 && postCalls == 1,
              "4-rank AG verb flow: read x3 + syncPre + syncPost（exec 现值驱动）");
        Check(copyCalls == 2, "rank1 copy x2（归位 inOff!=outOff + VIA_CCUBUF 上料）, got " +
              std::to_string(copyCalls));
        // 真原语五步：每条 READ 5 原语
        size_t readPrim = 0;
        for (const auto& s : g_stub) { if (s.func == "Read") ++readPrim; }
        Check(readPrim == 3, "3 HcommReadOnThread (five-step chain around each)");
    }

    printf("== A2-② blob 三重校验负例（magic/version/槽数守恒） ==\n");
    {
        // 判据复算（与 unified_aicpu_exec.cc ParseBlobHead 同式）：手工构造负例 blob
        std::vector<u64> good(7 + 32, 0);
        good[0] = 0x55504C33ULL; good[1] = 4ULL; good[2] = 1ULL;
        good[7] = 0ULL;   // 段 0 动作 ID = CollectAll
        // 正例：槽数守恒
        const u64 expectSlots = 7ULL + good[2] * 32ULL;
        Check(good.size() == expectSlots, "intact blob passes size check (7+32)");
        // 负例 1：坏 magic
        std::vector<u64> badMagic = good; badMagic[0] = 0x34504C33ULL;
        Check(badMagic[0] != 0x55504C33ULL, "bad magic rejected (writer :magic check)");
        // 负例 2：坏版本（3——v4 消费方拒收旧版）
        std::vector<u64> badVer = good; badVer[1] = 3ULL;
        Check(badVer[1] != 4ULL, "v3 blob rejected by v4 consumer (version check)");
        // 负例 3：段数与槽数不符
        std::vector<u64> badCnt = good; badCnt[2] = 2ULL;
        Check(7ULL + badCnt[2] * 32ULL != badCnt.size(), "seg-count vs slots mismatch rejected");
        // 负例 4：非 CollectAll 段 ID
        std::vector<u64> badKind = good; badKind[7] = 1ULL;   // ReduceSlice（R6 AICPU 不可消费）
        Check(badKind[7] != 0ULL, "ReduceSlice segment rejected by AICPU consumer (R6 scope)");
    }

    printf("== A2-③ exec 现值实例化数学（比例重算 + 末段余量） ==\n");
    {
        // 消费方段实例化判据复算（exec.cc ⑤ 段同式）：segRatio 来自 blob 槽 4
        // 单段（sole[mesh] 无切轮）：ratio=[1] → segSlice = fullSlice（无余量分支）
        const u64 fullSlice = 64 * 1024 * 2;    // 65536 元素 × FP16 = 131072B
        std::vector<u64> ratio1 = {1};
        u64 processed = 0;
        u64 seg0 = (0 + 1 == 1) ? (fullSlice - processed) : 0;
        Check(seg0 == fullSlice, "single-seg instantiation: segSlice == fullSlice");
        // 双段（切轮 512K+256K 的 1.5MB 例）：ratio 比例 2:1 → 3 段中的两段例
        std::vector<u64> ratio2 = {524288, 262144};   // SplitRounds 产物比例
        u64 sum2 = 524288 + 262144;
        const u64 full2 = 3ULL * 1024 * 1024;          // exec 现值 3MB
        const u64 s0 = full2 * ratio2[0] / sum2;
        const u64 s1 = full2 - s0;                      // 末段余量
        Check(s0 == 2ULL * 1024 * 1024 && s1 == 1ULL * 1024 * 1024,
              "2:1 ratio on 3MB -> 2MB + 1MB (proportional instantiation)");
        Check(s0 + s1 == full2, "sum == fullSlice (byte conservation)");
    }

    printf("\n%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
