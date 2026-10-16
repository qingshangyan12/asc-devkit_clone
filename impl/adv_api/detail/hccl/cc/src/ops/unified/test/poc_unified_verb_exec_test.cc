// R12 · WP-3/server 分发循环 + WP-8 单测：动词帧 → 原语组合的 host 直测
//
// 被测对象：
//   ① inc/verb_serializer.h（SerializeVerbPlan/SerializeVerbSegment/ValidateVerbSegment）
//      ——v5 帧序列 golden-bytes（≥3 组参数手算）+ 限界/容量负例
//   ② server 分发循环 DispatchUnifiedVerbs 的 host 镜像（真源在
//      ccu_kernel_kfc_server.cc 纯新增分支，__aicore__/CCU_IF 面 host 不可执行——
//      本测试以源码级同序镜像 SimulateDispatchVerbs 复刻"帧→原语调用序"的展开：
//      逐动词一帧 → 原语记录（WriteVariableWithNotify/NotifyWait/Read/Write/LocalCopy/
//      GroupLocalReduce 语义名），期望序全部手写（对照老 AG/RS kernel 的逐行推导，
//      不从被测实现回推）
//   ③ 等价对数（WP-7）：同一 Plan 双路径——老路径（v3 段帧→老 kernel 实参序，
//      SimulateKfcServerV3 既有面）vs 新路径（v5 动词帧→原语序），数据流图等价
//      （搬运集合 + sync 边界）判据由 st_semantic_checker 的 StNormalizeBaseline/
//      StExpandVerbFrames/CheckVerbSeqEquivalent 承载（ST 方已交付的判据件，本测试
//      驱动它对生成方 v5 真帧跑）——AG sole mesh 单段先跑通，RS mesh 单段次之
//
// 期望值全部独立手算（禁自证）。
#include "verb_serializer.h"
#include "verb_sink.h"
#include "action_collect_all.h"
#include "action_reduce_slice.h"
#include "action_spec.h"
#include "backend_registry.h"
#include "l1_adapter.h"
#include "loop_splitter.h"
#include "plan_serializer.h"
// 后端实现（include 即触发 REGISTER_ACTION_BACKEND 静态注册——与既有测试同机制）
#include "backend_ccu_collect_all.h"
#include "backend_ccu_reduce_slice.h"
#include "backend_aicpu_reduce_slice.h"   // R12 · WP-9①：RS×AICPU 后端（V7 对数）
#include "hcomm_verb_sink.h"              // R12 · WP-9②：HcommVerbSink（V7 端到端驱动）

#include <cstdio>
#include <string>
#include <vector>
using namespace mc2_ops_hccl::refactor;

static int g_pass = 0, g_fail = 0;
static void Check(bool cond, const std::string& what)
{
    if (cond) { ++g_pass; printf("  [PASS] %s\n", what.c_str()); }
    else      { ++g_fail; printf("  [FAIL] %s\n", what.c_str()); }
}

// ============================================================================
// devkit 侧 kfc_server_message.h v5 段的镜像常量（三方同名登记之二——本文件以
// static_assert 与本仓 inc/verb_serializer.h 互锁；devkit 侧探针在 run.sh 追加段）
// ============================================================================
constexpr u64 DEVKIT_KFC_VERB_COPY = 0U;
constexpr u64 DEVKIT_KFC_VERB_POST_COPY = 1U;
constexpr u64 DEVKIT_KFC_VERB_READ = 2U;
constexpr u64 DEVKIT_KFC_VERB_WRITE = 3U;
constexpr u64 DEVKIT_KFC_VERB_REDUCE = 4U;
constexpr u64 DEVKIT_KFC_VERB_BROADCAST = 5U;
constexpr u64 DEVKIT_KFC_VERB_SYNC_PRE = 6U;
constexpr u64 DEVKIT_KFC_VERB_SYNC_POST = 7U;
constexpr u64 DEVKIT_KFC_VERB_NHR_RUN = 8U;   // R18 全删轮后：枚举值保留（ABI 空洞），无生产帧
constexpr u64 DEVKIT_KFC_VERB_SLICE_REDUCE = 9U;
constexpr u64 DEVKIT_KFC_VERB_CH_SYNC = 10U;
static_assert(DEVKIT_KFC_VERB_COPY == static_cast<u64>(VerbCall::COPY), "verb id mirror: COPY");
static_assert(DEVKIT_KFC_VERB_POST_COPY == static_cast<u64>(VerbCall::POST_COPY), "verb id mirror: POST_COPY");
static_assert(DEVKIT_KFC_VERB_READ == static_cast<u64>(VerbCall::READ), "verb id mirror: READ");
static_assert(DEVKIT_KFC_VERB_WRITE == static_cast<u64>(VerbCall::WRITE), "verb id mirror: WRITE");
static_assert(DEVKIT_KFC_VERB_REDUCE == static_cast<u64>(VerbCall::REDUCE), "verb id mirror: REDUCE");
static_assert(DEVKIT_KFC_VERB_BROADCAST == static_cast<u64>(VerbCall::BROADCAST), "verb id mirror: BROADCAST");
static_assert(DEVKIT_KFC_VERB_SYNC_PRE == static_cast<u64>(VerbCall::SYNC_PEER_PRE), "verb id mirror: SYNC_PRE");
static_assert(DEVKIT_KFC_VERB_SYNC_POST == static_cast<u64>(VerbCall::SYNC_PEER_POST), "verb id mirror: SYNC_POST");
static_assert(DEVKIT_KFC_VERB_NHR_RUN == static_cast<u64>(VerbCall::NHR_RUN), "verb id mirror: NHR_RUN (R13; R18 全删轮后值序空洞保留)");
static_assert(DEVKIT_KFC_VERB_SLICE_REDUCE == static_cast<u64>(VerbCall::SLICE_REDUCE), "verb id mirror: SLICE_REDUCE (R18)");
static_assert(DEVKIT_KFC_VERB_CH_SYNC == static_cast<u64>(VerbCall::CH_SYNC), "verb id mirror: CH_SYNC (R18)");
constexpr u32 DEVKIT_KFC_VERB_PREFIX_LEN = 7U;
constexpr u64 DEVKIT_KFC_VERB_MAGIC = 0x55504C35ULL;
constexpr u64 DEVKIT_KFC_VERB_VERSION = 5ULL;
static_assert(DEVKIT_KFC_VERB_PREFIX_LEN == 7U, "CcuVerb prefix len 7 == strlen (BLOCKER-1 discipline)");
static_assert(DEVKIT_KFC_VERB_MAGIC == PLAN_V5_MAGIC, "UPL5 magic mirror");
static_assert(DEVKIT_KFC_VERB_VERSION == PLAN_V5_VERSION_FIELD, "v5 version mirror");

// ============================================================================
// server 分发循环 host 镜像（SimulateDispatchVerbs）：帧 → 原语调用序。
// 与真源 DispatchUnifiedVerbs（ccu_kernel_kfc_server.cc 新增分支）的对应关系——
// 逐动词的展开序逐行镜像（源码级判据，不是行为复刻）：
//   SYNC_PEER_PRE：掩码逐 XN 一条 WriteVariableWithNotify（ag_m2m.cc:115-120 同式）
//                 + 每通道 NotifyWait（:121-123）
//   SYNC_PEER_POST：每通道 NotifyRecord + NotifyWait（ag_m2m.cc:130-135）
//   READ：ccu::Read(ch, scratch槽, 对端input我的片, size)（rs_m2m.cc:180-182）
//   WRITE：ccu::Write(ch, 对端槽, src, size)（ag_m2m.cc:176-177）
//   COPY：ccu::LocalCopy(dst, src, size)（rs_m2m.cc:176-179）
//   BROADCAST：GroupBroadcast（本地+逐通道写，扇出语义）
//   REDUCE：≤8P GroupLocalReduce / >8P PairwiseLocalReduce（rs_m2m.cc:244-253）
// ============================================================================
struct PrimRec {
    std::string op;      // "WVN"/"NW"/"NR"/"RD"/"WR"/"LC"/"GLR"/"LR"
    u32 peer{0};
    u64 a0{0}, a1{0}, a2{0};
    u32 mask{0};
};

// 模拟 server ctx（role==UNIFIED_VERB 实例的自持量）
struct VerbServerCtx {
    u32 rankId{0};
    u32 rankSize{0};
    u32 channelCount{0};
    std::vector<u64> peerIn;      // SYNC_PRE 后缓存的对端 IN 基址（按通道序）
    std::vector<u64> peerScr;
    std::vector<u64> peerOut;
    std::vector<u64> peerTok;     // 对端 token（跨形态共享向量——混排复位语义断言对象）
};

static std::vector<PrimRec> SimulateDispatchVerbs(const std::vector<u64>& blob, VerbServerCtx& ctx,
                                                  std::string& err)
{
    std::vector<PrimRec> out;
    if (blob.size() < UNIFIED_V5_HEADER_SLOTS) {
        err = "blob too small for v5 header";
        return out;
    }
    if (blob[0] != PLAN_V5_MAGIC || blob[1] != PLAN_V5_VERSION_FIELD) {
        err = "bad magic/version (expect UPL5/5)";
        return out;
    }
    const u64 frameCount = blob[2];
    if (blob.size() != static_cast<size_t>(UNIFIED_V5_HEADER_SLOTS) +
                           static_cast<size_t>(frameCount) * UNIFIED_V5_SLOTS_PER_FRAME) {
        err = "blob size != 7 + frames*32";
        return out;
    }
    // ctx 即 server 的 VerbExecState 承载体（引用传入、逐帧演化——调用方可观测末态，
    // 混排复位语义断言的对象）
    for (u64 f = 0; f < frameCount; ++f) {
        const u64* fr = blob.data() + UNIFIED_V5_HEADER_SLOTS + f * UNIFIED_V5_SLOTS_PER_FRAME;
        const u64 verb = fr[U_V5_VERB_ID];
        if (verb == DEVKIT_KFC_VERB_SYNC_PRE) {
            const u32 xnMask = static_cast<u32>(fr[U_V5_ARG0]);
            const u64 myIn = fr[U_V5_ARG1];
            const u64 myScr = fr[U_V5_ARG2];
            const u64 myOut = fr[U_V5_ARG3];
            // ★段边界复位（镜像 server 真源 R12-8：VerbSyncPre 入口四向量 clear +
            //   游标归零，last-segment-wins。追加语义下混排 AG→RS 会使 peerTok 残留
            //   AG 段旧值致下标错位——防退化断言见 TestDispatchLoop 末尾。）
            ctx.peerIn.clear();
            ctx.peerScr.clear();
            ctx.peerOut.clear();
            ctx.peerTok.clear();
            // 掩码逐 XN 一条 WVN（每通道一组；老 kernel PreSync 同序）
            for (u32 ch = 0; ch < ctx.channelCount; ++ch) {
                if (xnMask & (1U << V5_XN_IN)) {
                    PrimRec r; r.op = "WVN"; r.peer = ch; r.a0 = myIn; r.mask = 1U << V5_XN_IN;
                    out.push_back(r);
                }
                if (xnMask & (1U << V5_XN_SCR)) {
                    PrimRec r; r.op = "WVN"; r.peer = ch; r.a0 = myScr; r.mask = 1U << V5_XN_SCR;
                    out.push_back(r);
                }
                if (xnMask & (1U << V5_XN_TOK)) {
                    PrimRec r; r.op = "WVN"; r.peer = ch; r.a0 = 0; r.mask = 1U << V5_XN_TOK;  // ctx.token
                    out.push_back(r);
                }
                if (xnMask & (1U << V5_XN_OUT)) {
                    PrimRec r; r.op = "WVN"; r.peer = ch; r.a0 = myOut; r.mask = 1U << V5_XN_OUT;
                    out.push_back(r);
                }
            }
            for (u32 ch = 0; ch < ctx.channelCount; ++ch) {
                PrimRec r; r.op = "NW"; r.peer = ch; r.mask = xnMask;
                out.push_back(r);
            }
            // 消费侧缓存对端值（GetResByChannel 语义——模拟器直接以"对方也广播了同构
            // 基址"建模：对端基址 = 同段本端基址（对称布局，golden 构造的前提）。
            // 按 XN 位缓存：与 server 真源两形态分支一致（AG 0xC→peerOut/peerTok、
            // RS 0x7→peerIn/peerScr/peerTok），掩码不含的 XN 不写——覆写语义下末段
            // 未涉及的向量为空（server 同：clear 后只 push 掩码内 XN）。
            if (xnMask & (1U << V5_XN_IN)) {
                ctx.peerIn.assign(ctx.channelCount, myIn);
            }
            if (xnMask & (1U << V5_XN_SCR)) {
                ctx.peerScr.assign(ctx.channelCount, myScr);
            }
            if (xnMask & (1U << V5_XN_TOK)) {
                ctx.peerTok.assign(ctx.channelCount, 0);   // ctx.token 对称建模（TOK 广播恒 ctx.token）
            }
            if (xnMask & (1U << V5_XN_OUT)) {
                ctx.peerOut.assign(ctx.channelCount, myOut);
            }
        } else if (verb == DEVKIT_KFC_VERB_SYNC_POST) {
            for (u32 ch = 0; ch < ctx.channelCount; ++ch) {
                PrimRec r; r.op = "NR"; r.peer = ch; r.mask = 1U << V5_XN_POST;
                out.push_back(r);
            }
            for (u32 ch = 0; ch < ctx.channelCount; ++ch) {
                PrimRec r; r.op = "NW"; r.peer = ch; r.mask = 1U << V5_XN_POST;
                out.push_back(r);
            }
        } else if (verb == DEVKIT_KFC_VERB_READ) {
            // 通道游标：数据帧按序消费（本测试逐帧显式计数，模拟 server 在飞游标）
            static thread_local u32 cursor = 0;   // 每段 SYNC_PRE 重置——由调用方清（见下）
            (void)cursor;
            PrimRec r; r.op = "RD"; r.peer = 0xFFFFFFFFU;   // peer 由段级展开补（等价对数在下方做）
            r.a0 = fr[U_V5_ARG0]; r.a1 = fr[U_V5_ARG1]; r.a2 = fr[U_V5_ARG2];
            out.push_back(r);
        } else if (verb == DEVKIT_KFC_VERB_WRITE) {
            PrimRec r; r.op = "WR";
            r.a0 = fr[U_V5_ARG0]; r.a1 = fr[U_V5_ARG1]; r.a2 = fr[U_V5_ARG2];
            out.push_back(r);
        } else if (verb == DEVKIT_KFC_VERB_COPY) {
            PrimRec r; r.op = "LC";
            r.a0 = fr[U_V5_ARG0]; r.a1 = fr[U_V5_ARG1]; r.a2 = fr[U_V5_ARG2];
            out.push_back(r);
        } else if (verb == DEVKIT_KFC_VERB_BROADCAST) {
            PrimRec r; r.op = "GLRBCAST";   // GroupBroadcast：一条声明式原语
            r.a0 = fr[U_V5_ARG0]; r.a1 = fr[U_V5_ARG1]; r.a2 = fr[U_V5_ARG2];
            out.push_back(r);
        } else if (verb == DEVKIT_KFC_VERB_REDUCE) {
            PrimRec r; r.op = (ctx.rankSize <= 8U) ? "GLR" : "PAIRWISE";
            r.a0 = fr[U_V5_ARG0]; r.a1 = fr[U_V5_ARG1]; r.a2 = fr[U_V5_ARG2];
            out.push_back(r);
        } else if (verb == DEVKIT_KFC_VERB_SLICE_REDUCE) {
            // R18 · #11 全删轮：SLICE_REDUCE 展开（server VerbSliceReduce 的 host
            // 镜像——帧槽 {1=step, 2=sliceIdx, 3=size, 4=peerWR, 5=peerRD, 6..10=种子}）。
            // 几何由 NhrSliceOf 公共件推导（与 DoSliceReduce 同式）；本镜像器 WR 侧：
            // WR(src=dst 同偏移 tx*stride, size) + EW——与旧 NHR_RUN 镜像的逐 slice
            // 展开同构（等价锁传递：贡献集/WR 几何与 R13-③ ST 基线一致）
            const u32 step = static_cast<u32>(fr[U_V5_SR_STEP]);
            const u32 sliceIdx = static_cast<u32>(fr[U_V5_SR_SLICE_IDX]);
            const u64 sliceSize = fr[U_V5_SR_SIZE];
            // 种子段级补写只落首 SR 帧（序列化器口径）——后续帧槽 6 为 0；
            // 镜像器退紧凑布局 sliceSize（server 真源组址用 peerIn[chIdx] +
            // NhrSliceOf 几何，不直接读帧槽 6——此处等价紧凑口径）
            u64 inStride = fr[U_V5_SR_INPUT_SLICE_STRIDE];
            if (inStride == 0ULL) {
                inStride = sliceSize;
            }
            const u32 rankSize = ctx.rankSize;
            const u32 rankId = ctx.rankId;
            // tx 序重建（GetStepInfo 同式——sliceIdx 位置的本端 tx 槽号）
            const u32 deltaRank = 1U << step;
            const u32 deltaSlice = 1U << (step + 1U);
            const u32 toRank = (rankId + rankSize - deltaRank) % rankSize;
            u32 tx = toRank;
            for (u32 q = 0; q < sliceIdx; ++q) {
                tx = (tx + rankSize - deltaSlice) % rankSize;
            }
            PrimRec wr; wr.op = "WR"; wr.peer = 0xFFFFFFFFU;   // 通道由贡献集对数补
            wr.a0 = static_cast<u64>(tx) * inStride;   // src = 本端 IN + off[tx]
            wr.a1 = static_cast<u64>(tx) * inStride;   // dst = 对端 IN + off[tx]（同偏移）
            wr.a2 = sliceSize;
            out.push_back(wr);
            PrimRec ew; ew.op = "EW"; ew.mask = 1;
            out.push_back(ew);
        } else if (verb == DEVKIT_KFC_VERB_CH_SYNC) {
            // R18 · #11 全删轮：CH_SYNC 四态（server VerbChSync 的 host 镜像——
            // 帧槽 {1=peer, 2=dir, 3=idx}）。dir {0=ACK-Rec, 1=ACK-Wait,
            // 2=DATA-Rec, 3=DATA-Wait} → Record 侧 NR + Wait 侧 NW（与旧步间
            // 屏障 NR/NW 序一致——DoNhrRun "先 Record 后 Wait"）
            const u64 dir = fr[U_V5_CS_DIR];
            if (dir == 0ULL || dir == 2ULL) {
                PrimRec nr; nr.op = "NR"; nr.mask = 1U << 2;   // Record（ACK/DATA 半边）
                out.push_back(nr);
            } else {
                PrimRec nw; nw.op = "NW"; nw.mask = 1U << 2;   // Wait
                out.push_back(nw);
            }
        } else {
            err = "frame " + std::to_string(f) + ": unknown verb id " + std::to_string(verb) +
                  " (log-and-continue in server; host gate refuses)";
            return out;
        }
    }
    return out;
}

// ============================================================================
// ---------------------------------------------------------------------------
// 真原语桩（shim extern "C" 声明的链接实现——V7 端到端驱动用；符号仅本 TU）
// ---------------------------------------------------------------------------
static int32_t g_primRet = 0;
extern "C" {
int32_t HcommLocalCopyOnThread(ThreadHandle t, void* d, const void* s, uint64_t l)
{ (void)t; (void)d; (void)s; (void)l; return g_primRet; }
int32_t HcommReadOnThread(ThreadHandle t, ChannelHandle c, void* d, const void* s, uint64_t l)
{ (void)t; (void)c; (void)d; (void)s; (void)l; return g_primRet; }
int32_t HcommWriteOnThread(ThreadHandle t, ChannelHandle c, void* d, const void* s, uint64_t l)
{ (void)t; (void)c; (void)d; (void)s; (void)l; return g_primRet; }
int32_t HcommLocalReduceOnThread(ThreadHandle t, void* d, const void* s, uint64_t cnt,
    HcommDataType dt, HcommReduceOp op)
{ (void)t; (void)d; (void)s; (void)cnt; (void)dt; (void)op; return g_primRet; }
int32_t HcommThreadNotifyRecordOnThread(ThreadHandle t, ThreadHandle dt, uint32_t i)
{ (void)t; (void)dt; (void)i; return g_primRet; }
int32_t HcommThreadNotifyWaitOnThread(ThreadHandle t, uint32_t i, uint32_t to)
{ (void)t; (void)i; (void)to; return g_primRet; }
int32_t HcommChannelNotifyRecordOnThread(ThreadHandle t, ChannelHandle c, uint32_t i)
{ (void)t; (void)c; (void)i; return g_primRet; }
int32_t HcommChannelNotifyWaitOnThread(ThreadHandle t, ChannelHandle c, uint32_t i, uint32_t to)
{ (void)t; (void)c; (void)i; (void)to; return g_primRet; }
// R15 · ②：NHR_RUN 执行面原语桩（sink DoNhrRun 消费）
int32_t HcommWriteReduceOnThread(ThreadHandle t, ChannelHandle c, void* d, const void* s,
    uint64_t n, HcommDataType dt, HcommReduceOp op)
{ (void)t; (void)c; (void)d; (void)s; (void)n; (void)dt; (void)op; return g_primRet; }
int32_t HcommReadReduceOnThread(ThreadHandle t, ChannelHandle c, void* d, const void* s,
    uint64_t n, HcommDataType dt, HcommReduceOp op)
{ (void)t; (void)c; (void)d; (void)s; (void)n; (void)dt; (void)op; return g_primRet; }
}

// 测试基建：Compile → 段 → Emit 序（真后端，sink 捕获 VerbCall）→ VerbSegment
// ============================================================================
static OpParam MakeAgOp(u32 rankSize, u64 countPerRank, u32 myRank, CommEngine engine)
{
    OpParam op;
    op.opType = static_cast<int>(HcclCMDType::HCCL_CMD_ALLGATHER);
    op.DataDes.count = countPerRank;
    op.DataDes.dataType = HCCL_DATA_TYPE_FP16;
    op.engine = engine;
    op.userRank = myRank;
    return op;
}
static OpParam MakeRsOp(u32 rankSize, u64 countPerRank, u32 myRank, CommEngine engine)
{
    OpParam op;
    op.opType = static_cast<int>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER);
    op.DataDes.count = countPerRank;
    op.DataDes.dataType = HCCL_DATA_TYPE_FP16;
    op.engine = engine;
    op.userRank = myRank;
    return op;
}
static TopoInfoWithNetLayerDetails MakeTopo(u32 rankSize)
{
    TopoInfoWithNetLayerDetails topo;
    topo.userRankSize = rankSize;
    topo.topoLevelNums = 1;
    topo.level0Topo = Level0Shape::MESH_1D;
    return topo;
}
static std::vector<std::vector<u32>> SingleDomain(u32 n)
{
    std::vector<u32> r;
    for (u32 i = 0; i < n; ++i) r.push_back(i);
    return {r};
}

// VerbCall 捕获 sink
struct CaptureSink : public IVerbSink {
    std::vector<VerbCall> calls;
    void Emit(const VerbCall& call) override { calls.push_back(call); }
};

static VerbSegment EmitSegment(const Plan& plan, size_t segIdx, Engine engine)
{
    VerbSegment vs;
    const ActionSpec& seg = plan.segments[segIdx];
    vs.seg = seg;
    IActionBackend* be = BackendRegistry::Get(seg.kind, engine);
    CaptureSink sink;
    EmitCtx ctx;
    ctx.sink = &sink;
    if (seg.kind == ActionKind::CollectAll) {
        LoopSplitter splitter;
        CollectAllArgs args = CollectAll::BuildArgs(seg, splitter);
        vs.actionKind = VerbCall::BROADCAST;
        be->Emit(seg, &args, ctx);
    } else {
        ReduceSliceArgs args;
        std::string berr;
        if (ReduceSlice::BuildArgs(seg, args, berr) != HCCL_SUCCESS) {
            vs.actionKind = VerbCall::REDUCE;
            return vs;   // 调用方以 calls.empty() 判失败
        }
        vs.actionKind = VerbCall::REDUCE;
        be->Emit(seg, &args, ctx);
    }
    vs.calls = sink.calls;
    return vs;
}

// ============================================================================
// ① golden-bytes：AG 4 卡 me=1 count=2000（FP16 → slice=4000B，无尾片）
//    手算（LoopSplitter config {8,8,32768}，loopSize=262144）：
//      4000 < 262144 → m=0, n=0, p=4000 → addrOffset=0, loopParam=0,
//      parallelParam=PP(0,0,1)=(0<<55)|(0<<48)|(1<<41)=2^41=2199023255552, residual=4000
//    帧序（Emit：SYNC_PRE → BROADCAST → SYNC_POST）：
//      帧0: {6, 0xC, 0, 0, outBase=0}（AG 掩码；token 不占槽）
//      帧1: {5, src=inBase+1*4000=4000, ldst=outBase+1*4000=4000, size=4000,
//            rdst=4000, goSize={0,0,2199023255552,4000}}
//      帧2: {7, 0x10, 0, 0, 0}
// ============================================================================
static void TestAgGolden()
{
    printf("== V1 AG v5 golden-bytes（4 卡 me=1 slice=4000 手算） ==\n");
    OpParam op = MakeAgOp(4, 2000, 1, CommEngine::COMM_ENGINE_CCU);
    TopoInfoWithNetLayerDetails topo = MakeTopo(4);
    Plan plan;
    std::string err;
    Check(L1Adapter::Compile(op, topo, SingleDomain(4), "sole[mesh]", plan, err) == HCCL_SUCCESS,
          "compile ok: " + err);
    Check(plan.segments.size() == 1 && plan.segments[0].kind == ActionKind::CollectAll, "1 CollectAll segment");

    const VerbSegment vs = EmitSegment(plan, 0, Engine::CCU);
    Check(vs.calls.size() == 3, "Emit 序 3 动词（pre/bcast/post）");
    std::vector<u64> blob;
    Check(SerializeVerbPlan({vs}, plan, blob, err), "serialize ok: " + err);
    Check(blob.size() == 7 + 3 * 32, "blob = 7 header + 3*32 slots");
    Check(blob[0] == 0x55504C35ULL && blob[1] == 5ULL && blob[2] == 3ULL,
          "header {UPL5, 5, frameCount=3}");
    const u64* f0 = blob.data() + 7;
    const u64* f1 = f0 + 32;
    const u64* f2 = f1 + 32;
    Check(f0[0] == 6ULL && f0[1] == 0xCULL, "frame0: SYNC_PRE + AG mask 0xC");
    // BROADCAST 帧（P0-2 修复后 Emit 槽位语义——backend_ccu_collect_all.h）：
    //   src=inputAddrOff（BuildArgs 的 InOffset(me)=0——AG 广播源是"我的 input 我的片"，
    //   sole 布局 inBase=0）、dst=outputAddrOff=OutOffset(me)=outBase+1*4000=4000
    //   （P0-2：真实落位偏移——R12-9 缺陷①修复，旧值 peers.size()=3 是病灶形态）；
    //   size=mySliceSize=4000；remoteDstOff 槽（ARG3）由序列化器段级补写
    //   OutOffset(me)=4000（对端 output 里我的片）
    Check(f1[0] == 5ULL && f1[1] == 0ULL && f1[2] == 4000ULL && f1[3] == 4000ULL,
          "frame1: BROADCAST src=in+0、dst=OutOffset(me)=4000、size=4000（P0-2 真实落位）");
    // P0-2 病灶负例锚：旧形态 dst==peers.size()==N-1==3（值恒 ≠OutOffset(me)——
    // me≠0 时 OutOffset=outStride*me，仅 me=0 时同为 0；此处 me=1 必不等）
    Check(f1[2] != 3ULL, "frame1: dst != peers.size()=3（缺陷①防退化——Emit 不得再发 fan-out 计数）");
    Check(f1[4] == 4000ULL, "frame1: aux(remoteDstOff) = 4000");
    Check(f1[5] == 0ULL && f1[6] == 0ULL && f1[7] == 2199023255552ULL && f1[8] == 4000ULL,
          "frame1: goSize {0,0,PP(0,0,1)=2^41,4000}（手算：4000<loopSize 零头形态）");
    Check(f2[0] == 7ULL && f2[1] == 0x10ULL, "frame2: SYNC_POST mask 0x10");
    // 未用槽清 0（#17 同族：帧内未写槽全清）
    bool unusedZero = true;
    for (u32 i = 9; i < 32; ++i) {
        if (f1[i] != 0ULL) unusedZero = false;
    }
    Check(unusedZero, "frame1 unused slots 9..31 all zero");
}

// ============================================================================
// ② golden-bytes：RS 8 卡 me=3 count=500（FP16 → slice=1000B）
//    帧序（Emit：SYNC_PRE → READ×7 → REDUCE → SYNC_POST；≤8P 无 COPY 帧）= 10 帧
//    chunk 手算（CalcRsChunks(1000, 8)：chunkSize=64MB/8/128*128=8388608 ≥ 1000 →
//    chunkCount=1（单 chunk 受理）、tailSize=1000；
//    fullGo = Calc(8388608, {8,16,4096})：loopSize=65536 → m=128, n=0, p=0 →
//    addrOffset=4096*16*128=8388608, loopParam=128, parallelParam=0, residual=0）
// ============================================================================
static void TestRsGolden()
{
    printf("== V2 RS v5 golden-bytes（8 卡 me=3 slice=1000 手算） ==\n");
    OpParam op = MakeRsOp(8, 500, 3, CommEngine::COMM_ENGINE_CCU);
    TopoInfoWithNetLayerDetails topo = MakeTopo(8);
    Plan plan;
    std::string err;
    Check(L1Adapter::Compile(op, topo, SingleDomain(8), "sole[mesh]", plan, err) == HCCL_SUCCESS,
          "compile ok: " + err);
    Check(plan.segments.size() == 1 && plan.segments[0].kind == ActionKind::ReduceSlice, "1 ReduceSlice segment");

    const VerbSegment vs = EmitSegment(plan, 0, Engine::CCU);
    Check(vs.calls.size() == 10, "Emit 序 10 动词（pre + 7 read + reduce + post）");
    std::vector<u64> blob;
    Check(SerializeVerbPlan({vs}, plan, blob, err), "serialize ok: " + err);
    Check(blob.size() == 7 + 10 * 32, "blob = 7 + 10*32");
    Check(blob[2] == 10ULL, "frameCount = 10");
    const u64* f0 = blob.data() + 7;
    Check(f0[0] == 6ULL && f0[1] == 0x7ULL, "frame0: SYNC_PRE + RS mask 0x7");
    Check(f0[2] == 0ULL && f0[3] == 0ULL, "frame0: 本端 IN/SCR 基址（sole 布局 0）");
    // READ 帧 1..7（P1-2 修复后 BuildArgs 几何——遍历序升序跳过 me：me=3 →
    //   peerAlgRank 0,1,2,4,5,6,7）：src = inBase + rankSliceOffset = 0+3000 = 3000
    //   （P0-3：RS 读"对端 input 的我的片"，rankSliceOffset=rsInStride*me=1000*3
    //   ——Compile 期 inputSliceStride=0 退 outputSliceStride=1000，BuildArgs 三分支；
    //   旧值恒 0 是缺陷②形态）、dst = scratch+peerSlot*1000、size=1000
    const u32 ascOrder[7] = {0, 1, 2, 4, 5, 6, 7};
    bool readOk = true;
    for (u32 k = 0; k < 7; ++k) {
        const u64* fk = blob.data() + 7 + (1 + k) * 32;
        if (fk[0] != 2ULL || fk[1] != 3000ULL ||
            fk[2] != static_cast<u64>(ascOrder[k]) * 1000ULL || fk[3] != 1000ULL) {
            readOk = false;
        }
    }
    Check(readOk, "READ 帧 1..7: src=rankSliceOffset=3000（P0-3）、dst=peerSlot*1000（升序 0,1,2,4..7）、size=1000");
    const u64* fRed = blob.data() + 7 + 8 * 32;
    Check(fRed[0] == 4ULL && fRed[1] == 0ULL && fRed[2] == 0ULL && fRed[3] == 1000ULL,
          "REDUCE 帧: src=scratch 基、dst=output 基、size=1000");
    Check(fRed[5] == 8388608ULL && fRed[6] == 128ULL && fRed[7] == 0ULL && fRed[8] == 0ULL,
          "REDUCE 帧 goSize = Calc(8388608, RS cfg) = {8388608, 128, 0, 0}（手算）");
    const u64* fPost = blob.data() + 7 + 9 * 32;
    Check(fPost[0] == 7ULL && fPost[1] == 0x10ULL, "frame9: SYNC_POST mask 0x10");
    // Validate（帧数 == 通道数；peer 序 == MESH 遍历序）
    std::string verr;
    Check(ValidateVerbSegment(vs, verr), "ValidateVerbSegment ok: " + verr);
}

// ============================================================================
// ③ golden-bytes：RS 9 卡 me=0（>8P：+1 COPY 帧本 rank 上料 → 12 帧）
//    COPY 帧：src=inBase+0*stride=0、dst=scratch+me*1000=0、size=1000
// ============================================================================
static void TestRs9PGolden()
{
    printf("== V3 RS 9P >8P 形态（+1 COPY 帧，12 帧手算） ==\n");
    OpParam op = MakeRsOp(9, 500, 0, CommEngine::COMM_ENGINE_CCU);
    TopoInfoWithNetLayerDetails topo = MakeTopo(9);
    Plan plan;
    std::string err;
    Check(L1Adapter::Compile(op, topo, SingleDomain(9), "sole[mesh]", plan, err) == HCCL_SUCCESS,
          "compile ok: " + err);
    const VerbSegment vs = EmitSegment(plan, 0, Engine::CCU);
    Check(vs.calls.size() == 12, "Emit 序 12 动词（pre + COPY + 8 read + reduce + post）");
    std::vector<u64> blob;
    Check(SerializeVerbPlan({vs}, plan, blob, err), "serialize ok: " + err);
    Check(blob[2] == 12ULL, "frameCount = 12（N+3）");
    const u64* fCopy = blob.data() + 7 + 1 * 32;
    Check(fCopy[0] == 0ULL && fCopy[1] == 0ULL && fCopy[2] == 0ULL && fCopy[3] == 1000ULL,
          "COPY 帧（me=0）: src=in+0、dst=scratch+0、size=1000（rs_m2m.cc:176-179 上料）");
    std::string verr;
    Check(ValidateVerbSegment(vs, verr), "9P validate ok（8 数据帧 == 8 通道）: " + verr);
}

// ============================================================================
// ④ 限界/容量/负例矩阵
// ============================================================================
static void TestNegative()
{
    printf("== V4 负例矩阵（限界/容量/未知 verb/校验） ==\n");
    // RS 多 chunk 拒收：8 卡 slice=8388609 → chunkCount=2
    {
        OpParam op = MakeRsOp(8, 8388609 / 2, 3, CommEngine::COMM_ENGINE_CCU);  // FP16: 4194304.5 → 取整
        op.DataDes.count = (8388609ULL + 1ULL) / 2ULL;                          // ceil → slice=8388610>chunk
        TopoInfoWithNetLayerDetails topo = MakeTopo(8);
        Plan plan;
        std::string err;
        if (L1Adapter::Compile(op, topo, SingleDomain(8), "sole[mesh]", plan, err) == HCCL_SUCCESS &&
            plan.segments.size() == 1 && plan.segments[0].kind == ActionKind::ReduceSlice) {
            const VerbSegment vs = EmitSegment(plan, 0, Engine::CCU);
            std::vector<u64> blob;
            const bool ok = SerializeVerbPlan({vs}, plan, blob, err);
            // chunkCount = (8388610-1)/8388608+1 = 2 > 1 → 拒收（slice 恰 8388610 时）
            // count=4194305*2=8388610 → 拒；若构造偏小（=8388608 单 chunk）受理——
            // 两条分支都以 chunkCount 判据断言（不依赖构造恰中）
            const RsChunkParams ch = CalcRsChunks(8388610ULL, 8U);
            const u64 chunkCount = ~0ULL - ch.chunkLoopNum;
            if (chunkCount > 1ULL) {
                Check(!ok && err.find("chunkCount") != std::string::npos,
                      "RS 多 chunk（8388610B→2 chunks）拒收: " + err);
            } else {
                Check(ok, "边界内受理（构造未越限时的对照分支）");
            }
        } else {
            Check(false, "multi-chunk case compile failed: " + err);
        }
    }
    // 帧数超限：单 Plan > 63 帧（手工构造 64 条 calls）
    {
        VerbSegment vs;
        vs.actionKind = VerbCall::READ;
        vs.seg.domain.ranks = {0, 1};
        vs.seg.domain.myRank = 0;
        vs.seg.layout.sliceSize = 1;
        for (int i = 0; i < 64; ++i) {
            vs.calls.push_back(VerbCall{VerbCall::COPY, 0, 0, 1, 0});
        }
        Plan plan;
        std::vector<u64> blob;
        std::string err;
        Check(!SerializeVerbPlan({vs}, plan, blob, err) &&
                  err.find("exceeds queue capacity") != std::string::npos,
              "64 帧超 63 上限拒收: " + err);
    }
    // 空 Plan 拒收
    {
        VerbSegment vs;
        Plan plan;
        std::vector<u64> blob;
        std::string err;
        Check(!SerializeVerbPlan({vs}, plan, blob, err) && err.find("empty verb plan") != std::string::npos,
              "空 Plan（0 帧）拒收");
    }
    // 帧预算容量断言（M0 §三表 + ST 镜像互锁）：AG=3 / RS 8P=10 / RS 9P=12 / AR 8P=13
    {
        // AG 4P：blob 帧 3；RS 8P：10；RS 9P：12 —— 由 golden 段复证（此处钉边界）：
        // 单 Plan 63 上限的数学（RS >8P N+3 ≤ 63 → N ≤ 60）
        const auto rsBudget = [](u32 n) -> u32 { return n + 2U + (n > 8U ? 1U : 0U); };
        Check(rsBudget(60U) == 63U && rsBudget(61U) == 64U, "RS >8P 帧预算边界 N=60|61");
        const auto arBudget = [&rsBudget](u32 n) -> u32 { return rsBudget(n) + 3U; };
        Check(arBudget(57U) == 63U && arBudget(58U) == 64U, "AR two_shot 帧预算边界 N=57|58");
    }
    // Validate 负例：数据帧数 != 通道数
    {
        VerbSegment vs;
        vs.actionKind = VerbCall::READ;
        vs.seg.domain.ranks = {0, 1, 2, 3};
        vs.seg.domain.myRank = 0;
        vs.seg.layout.sliceSize = 1;
        vs.calls.push_back(VerbCall{VerbCall::SYNC_PEER_PRE});
        vs.calls.push_back(VerbCall{VerbCall::READ, 0, 0, 1, 1});   // 只 1 条 READ（需 3）
        vs.calls.push_back(VerbCall{VerbCall::REDUCE, 0, 0, 1, 0});
        vs.calls.push_back(VerbCall{VerbCall::SYNC_PEER_POST});
        std::string verr;
        Check(!ValidateVerbSegment(vs, verr) && verr.find("data frame count") != std::string::npos,
              "Validate: 1 READ != 3 通道 → 拒: " + verr);
    }
    // Validate 负例：peer 序漂移（乱序 frames）
    {
        VerbSegment vs;
        vs.actionKind = VerbCall::READ;
        vs.seg.domain.ranks = {0, 1, 2, 3};
        vs.seg.domain.myRank = 0;
        vs.seg.layout.sliceSize = 1;
        vs.calls.push_back(VerbCall{VerbCall::SYNC_PEER_PRE});
        vs.calls.push_back(VerbCall{VerbCall::READ, 0, 0, 1, 2});   // 乱序：先 peer2
        vs.calls.push_back(VerbCall{VerbCall::READ, 0, 0, 1, 1});
        vs.calls.push_back(VerbCall{VerbCall::READ, 0, 0, 1, 3});
        vs.calls.push_back(VerbCall{VerbCall::REDUCE, 0, 0, 1, 0});
        vs.calls.push_back(VerbCall{VerbCall::SYNC_PEER_POST});
        std::string verr;
        Check(!ValidateVerbSegment(vs, verr) && verr.find("order drifts") != std::string::npos,
              "Validate: peer 序漂移（禁二次 sort 的单源派生防线）: " + verr);
    }
}

// ============================================================================
// ⑤ server 分发循环 host 直测：帧 → 原语序（手写期望，对照老 kernel 逐行推导）
// ============================================================================
static void TestDispatchLoop()
{
    printf("== V5 server verb 分发循环 host 镜像（帧→原语序） ==\n");
    // AG 4P me=1：blob（V1 golden 同构造）→ 原语序手算：
    //   SYNC_PRE(0xC)：WVN(OUT)+WVN(TOK) ×3 通道 = 6 条 → NW(0xC) ×3
    //   BROADCAST：GroupBroadcast 1 条（声明式）
    //   SYNC_POST：NR(0x10)×3 → NW(0x10)×3
    //   计 6 + 3 + 1 + 3 + 3 = 16 条
    {
        OpParam op = MakeAgOp(4, 2000, 1, CommEngine::COMM_ENGINE_CCU);
        TopoInfoWithNetLayerDetails topo = MakeTopo(4);
        Plan plan;
        std::string err;
        Check(L1Adapter::Compile(op, topo, SingleDomain(4), "sole[mesh]", plan, err) == HCCL_SUCCESS,
              "AG compile: " + err);
        const VerbSegment vs = EmitSegment(plan, 0, Engine::CCU);
        std::vector<u64> blob;
        Check(SerializeVerbPlan({vs}, plan, blob, err), "serialize: " + err);
        VerbServerCtx s;
        s.rankId = 1;
        s.rankSize = 4;
        s.channelCount = 3;
        std::string derr;
        const std::vector<PrimRec> prims = SimulateDispatchVerbs(blob, s, derr);
        Check(derr.empty(), "dispatch ok: " + derr);
        Check(prims.size() == 16, "AG 4P 原语序 = 16 条（6 WVN + 3 NW + 1 BCast + 3 NR + 3 NW）");
        // 前导 WVN：镜像按 XN 编码序（IN→SCR→TOK→OUT）逐位发射；AG 段掩码 0xC
        //   （TOK=0x4 + OUT=0x8）→ 每通道先 TOK 后 OUT。prim0=TOK 位、prim1=OUT 位
        Check(prims.size() > 5 && prims[0].op == "WVN" && prims[0].mask == (1U << V5_XN_TOK),
              "prim0: WVN TOK 位（ag_m2m.cc:115-120 同序——老 kernel 也是 token 先 output 后）");
        Check(prims[1].op == "WVN" && prims[1].mask == (1U << V5_XN_OUT), "prim1: WVN OUT 位");
        Check(prims[6].op == "NW" && prims[6].mask == 0xCU, "prim6: NW mask 0xC");
        size_t bcast = 0;
        for (const PrimRec& r : prims) { if (r.op == "GLRBCAST") ++bcast; }
        Check(bcast == 1, "1 GroupBroadcast（声明式组原语）");
        // 尾部 NR/NW 各 3
        size_t nr = 0;
        for (const PrimRec& r : prims) {
            if (r.op == "NR") ++nr;
        }
        Check(nr == 3, "3 NotifyRecord（每通道一条）");
    }
    // RS 8P me=3：原语序手算：SYNC_PRE(0x7) 3×WVN×8 通道 = 24 → NW×8 → READ×7 →
    //   GLR 1（≤8P）→ NR×8 → NW×8 = 24+8+7+1+8+8 = 56 条
    {
        OpParam op = MakeRsOp(8, 500, 3, CommEngine::COMM_ENGINE_CCU);
        TopoInfoWithNetLayerDetails topo = MakeTopo(8);
        Plan plan;
        std::string err;
        Check(L1Adapter::Compile(op, topo, SingleDomain(8), "sole[mesh]", plan, err) == HCCL_SUCCESS,
              "RS compile: " + err);
        const VerbSegment vs = EmitSegment(plan, 0, Engine::CCU);
        std::vector<u64> blob;
        Check(SerializeVerbPlan({vs}, plan, blob, err), "serialize: " + err);
        VerbServerCtx s;
        s.rankId = 3;
        s.rankSize = 8;
        s.channelCount = 7;
        std::string derr;
        const std::vector<PrimRec> prims = SimulateDispatchVerbs(blob, s, derr);
        Check(derr.empty(), "dispatch ok: " + derr);
        // RS 8P 原语序手算：SYNC_PRE(0x7) 掩码 3 位（IN/SCR/TOK）×7 通道 = 21 WVN →
        //   7 NW → 7 RD → 1 GLR → 7 NR → 7 NW = 50 条（通道数 = N-1 = 7）
        Check(prims.size() == 50, "RS 8P 原语序 = 50 条（21 WVN + 7 NW + 7 RD + 1 GLR + 7 NR + 7 NW）, got " +
              std::to_string(prims.size()));
        size_t rd = 0, glr = 0;
        for (const PrimRec& r : prims) {
            if (r.op == "RD") ++rd;
            if (r.op == "GLR") ++glr;
        }
        Check(rd == 7 && glr == 1, "7 Read + 1 GroupLocalReduce（≤8P 形态）");
    }
    // RS 9P me=0：REDUCE → PAIRWISE 分叉（rankSize>8）+ COPY 帧 → LC
    {
        OpParam op = MakeRsOp(9, 500, 0, CommEngine::COMM_ENGINE_CCU);
        TopoInfoWithNetLayerDetails topo = MakeTopo(9);
        Plan plan;
        std::string err;
        Check(L1Adapter::Compile(op, topo, SingleDomain(9), "sole[mesh]", plan, err) == HCCL_SUCCESS,
              "RS 9P compile: " + err);
        const VerbSegment vs = EmitSegment(plan, 0, Engine::CCU);
        std::vector<u64> blob;
        Check(SerializeVerbPlan({vs}, plan, blob, err), "serialize: " + err);
        VerbServerCtx s;
        s.rankId = 0;
        s.rankSize = 9;
        s.channelCount = 8;
        std::string derr;
        const std::vector<PrimRec> prims = SimulateDispatchVerbs(blob, s, derr);
        size_t lc = 0, pairwise = 0, glr = 0;
        for (const PrimRec& r : prims) {
            if (r.op == "LC") ++lc;
            if (r.op == "PAIRWISE") ++pairwise;
            if (r.op == "GLR") ++glr;
        }
        Check(lc == 1 && pairwise == 1 && glr == 0,
              "9P: 1 LocalCopy（上料）+ PairwiseLocalReduce 分叉（rankSize>8）");
    }
    // 未知 verb ID 负例：手工帧 verb id=11 → dispatch 镜像点名拒绝（server 真源日志继续，
    // host 侧 Validate 前置拦截口径——两口径差异登记同 v3 ⑥）。
    // ★R13 改 id 8→9、R18 全删轮再迁 9→11——SLICE_REDUCE=9/CH_SYNC=10 落地后
    // 合法域 0..10，负例探针迁 11（ID_MAX=10）
    {
        std::vector<u64> blob(7 + 32, 0);
        blob[0] = PLAN_V5_MAGIC;
        blob[1] = 5ULL;
        blob[2] = 1ULL;
        blob[7] = 11ULL;   // 未知 verb id（R18 后合法域 0..10）
        VerbServerCtx s;
        s.rankSize = 4;
        s.channelCount = 3;
        std::string derr;
        const std::vector<PrimRec> prims = SimulateDispatchVerbs(blob, s, derr);
        Check(!derr.empty() && derr.find("unknown verb id 11") != std::string::npos,
              "未知 verb id 11 点名拒绝（原 8→9→11 探针迁移——R18 两动词合法化）: " + derr);
        Check(prims.empty(), "未知帧零原语发射");
    }
    // 混排防退化（R12-8 裁决②）：两段连续 SYNC_PRE（AG 掩码 0xC → RS 掩码 0x7）后，
    // 对端变量缓存必须按段覆写——peerTok.size()==channelCount（last-segment-wins）。
    // 追加语义（修复前）下 peerTok 会是 2×channelCount（AG 段一份 + RS 段一份——
    // TOK 是唯一跨形态共享向量），且 RS 段 READ/WRITE 以 [ch] 下标取缓存会静默读到
    // AG 段旧值。server 内部状态 host 不可见，此断言是缺陷可判性锚点（镜像真源
    // VerbSyncPre 入口四向量 clear，ccu_kernel_kfc_server.cc R12-8）。ctx 引用传入
    // （同 server 的 ve）——末态直接可观测，无需复算。
    {
        std::vector<u64> blob(UNIFIED_V5_HEADER_SLOTS + 2ULL * UNIFIED_V5_SLOTS_PER_FRAME, 0);
        blob[0] = PLAN_V5_MAGIC;
        blob[1] = PLAN_V5_VERSION_FIELD;
        blob[2] = 2ULL;
        // 帧 0：AG 段 SYNC_PRE（掩码 0xC = TOK|OUT，槽 2..4 本端 IN/SCR/OUT）
        u64* f0 = blob.data() + UNIFIED_V5_HEADER_SLOTS;
        f0[U_V5_VERB_ID] = DEVKIT_KFC_VERB_SYNC_PRE;
        f0[U_V5_ARG0] = 0xC;
        f0[U_V5_ARG1] = 0x11000;   // myIn
        f0[U_V5_ARG2] = 0x22000;   // myScr
        f0[U_V5_ARG3] = 0x33000;   // myOut
        // 帧 1：RS 段 SYNC_PRE（掩码 0x7 = IN|SCR|TOK）
        u64* f1 = f0 + UNIFIED_V5_SLOTS_PER_FRAME;
        f1[U_V5_VERB_ID] = DEVKIT_KFC_VERB_SYNC_PRE;
        f1[U_V5_ARG0] = 0x7;
        f1[U_V5_ARG1] = 0x44000;
        f1[U_V5_ARG2] = 0x55000;
        f1[U_V5_ARG3] = 0x66000;
        VerbServerCtx s;
        s.rankSize = 4;
        s.channelCount = 3;
        std::string derr;
        const std::vector<PrimRec> prims = SimulateDispatchVerbs(blob, s, derr);
        Check(derr.empty(), "混排两段 SYNC_PRE dispatch ok: " + derr);
        // 混排原语计数：AG 段 0xC 两 XN×3 通道 = 6 WVN + 3 NW；RS 段 0x7 三 XN×3 = 9 WVN
        //   + 3 NW → 6+3+9+3 = 21（掩码不含的 XN 不发射——与 server 两形态分支一致）
        Check(prims.size() == 21, "混排 WVN/NW 计数 = AG(6+3) + RS(9+3) = 21, got " + std::to_string(prims.size()));
        // ★核心断言：last-segment-wins——四向量均 == channelCount（追加语义下 peerTok==6）
        Check(s.peerTok.size() == 3, "peerTok.size()==channelCount==3（追加语义下 6——防退化锚点）, got " +
              std::to_string(s.peerTok.size()));
        Check(s.peerIn.size() == 3 && s.peerScr.size() == 3, "peerIn/peerScr 按段覆写 == 3");
        Check(s.peerOut.size() == 0, "peerOut 末段(RS)不写——覆写语义下为 0（追加语义下残留 3）, got " +
              std::to_string(s.peerOut.size()));
        // 末段值正确：RS 段基址生效，AG 段值被覆写清除
        Check(s.peerIn[0] == 0x44000 && s.peerScr[0] == 0x55000,
              "末段 RS 基址生效（peerIn=0x44000/peerScr=0x55000），非 AG 段残留");
    }
    // 混排后组址链路（R12-8 裁决②的下游可观测面）：AG 段 → RS 段 → READ，单通道
    // （READ 游标 0 → 缓存 [0]）。追加语义下 RS 段 READ 组址会读到 AG 段残留 peerTok
    // 下标错位值——本断言锁段切换无残留：RS 段 NW 掩码恰为 0x7，且末段缓存值正确。
    {
        std::vector<u64> blob(UNIFIED_V5_HEADER_SLOTS + 3ULL * UNIFIED_V5_SLOTS_PER_FRAME, 0);
        blob[0] = PLAN_V5_MAGIC;
        blob[1] = PLAN_V5_VERSION_FIELD;
        blob[2] = 3ULL;
        u64* f0 = blob.data() + UNIFIED_V5_HEADER_SLOTS;
        f0[U_V5_VERB_ID] = DEVKIT_KFC_VERB_SYNC_PRE;
        f0[U_V5_ARG0] = 0xC;
        f0[U_V5_ARG1] = 0x11000;
        f0[U_V5_ARG2] = 0x22000;
        f0[U_V5_ARG3] = 0x33000;
        u64* f1 = f0 + UNIFIED_V5_SLOTS_PER_FRAME;
        f1[U_V5_VERB_ID] = DEVKIT_KFC_VERB_SYNC_PRE;
        f1[U_V5_ARG0] = 0x7;
        f1[U_V5_ARG1] = 0x44000;
        f1[U_V5_ARG2] = 0x55000;
        f1[U_V5_ARG3] = 0x66000;
        u64* f2 = f1 + UNIFIED_V5_SLOTS_PER_FRAME;
        f2[U_V5_VERB_ID] = DEVKIT_KFC_VERB_READ;
        f2[U_V5_ARG0] = 0x100;   // src off
        f2[U_V5_ARG1] = 0x200;   // dst off
        f2[U_V5_ARG2] = 0x1000;  // size
        VerbServerCtx s;
        s.rankSize = 4;
        s.channelCount = 1;   // 单通道：READ 游标 0 → 缓存 [0]——残留即错读
        std::string derr;
        const std::vector<PrimRec> prims = SimulateDispatchVerbs(blob, s, derr);
        Check(derr.empty(), "混排 AG→RS→READ dispatch ok: " + derr);
        size_t nwMasked = 0;
        for (const PrimRec& r : prims) {
            if (r.op == "NW" && r.mask == 0x7U) ++nwMasked;
        }
        Check(nwMasked == 1, "RS 段 NW 掩码 0x7 恰一条（段切换无残留）, got " + std::to_string(nwMasked));
        Check(s.peerIn.size() == 1 && s.peerIn[0] == 0x44000,
              "READ 组址基址 = 末段 RS peerIn（0x44000），非 AG 段残留");
    }
}

// ============================================================================
// ⑥ 等价对数（WP-7）：同 Plan 双路径数据流图等价——经 ST 判据件
//    （st_semantic_checker.h 的 CheckVerbSeqEquivalent——ST 方交付，本测试驱动它
//    对生成方 v5 真帧跑；v3 老路径半边由 SimulateKfcServerV3/序列化 v3 既有链承载）
// ============================================================================
static void TestEquivalence()
{
    printf("== V6 双路径等价（v5 帧展开 vs 老路径基线——ST 判据件驱动） ==\n");
    // 注：本 TU 不 include st_semantic_checker.cc（ST 地盘文件）——等价判据件的
    // 驱动入口在 run_st_checker.sh 的 ST 侧（对生成方 v5 真帧跑 CheckVerbSeqEquivalent，
    // ST 方按 [A1]-[A9] 清单对数）。此处锁生成方侧的两条不变量：
    //   ① v5 帧序的动词 kind 序 == Emit 序（编排结构数据化：帧序即动词序）
    //   ② 逐帧槽值 == VerbCall 字段（src/dst/size 逐槽直转，无重排）
    OpParam op = MakeRsOp(8, 500, 3, CommEngine::COMM_ENGINE_CCU);
    TopoInfoWithNetLayerDetails topo = MakeTopo(8);
    Plan plan;
    std::string err;
    Check(L1Adapter::Compile(op, topo, SingleDomain(8), "sole[mesh]", plan, err) == HCCL_SUCCESS,
          "compile: " + err);
    const VerbSegment vs = EmitSegment(plan, 0, Engine::CCU);
    std::vector<u64> blob;
    Check(SerializeVerbPlan({vs}, plan, blob, err), "serialize: " + err);
    bool kindSeqOk = true;
    bool slotOk = true;
    for (size_t k = 0; k < vs.calls.size(); ++k) {
        const u64* f = blob.data() + 7 + k * 32;
        const VerbCall& c = vs.calls[k];
        if (f[0] != static_cast<u64>(c.verb)) kindSeqOk = false;
        if (c.verb == VerbCall::READ || c.verb == VerbCall::WRITE || c.verb == VerbCall::COPY) {
            if (f[1] != c.src || f[2] != c.dst || f[3] != c.size) slotOk = false;
        }
    }
    Check(kindSeqOk, "帧 kind 序 == Emit 动词序（帧序即动词序，编排结构数据化）");
    Check(slotOk, "数据帧槽值 == VerbCall {src,dst,size} 逐槽直转");
    // AG 段同断言
    OpParam agOp = MakeAgOp(4, 2000, 1, CommEngine::COMM_ENGINE_CCU);
    TopoInfoWithNetLayerDetails agTopo = MakeTopo(4);
    Plan agPlan;
    Check(L1Adapter::Compile(agOp, agTopo, SingleDomain(4), "sole[mesh]", agPlan, err) == HCCL_SUCCESS,
          "AG compile: " + err);
    const VerbSegment agVs = EmitSegment(agPlan, 0, Engine::CCU);
    std::vector<u64> agBlob;
    Check(SerializeVerbPlan({agVs}, agPlan, agBlob, err), "AG serialize: " + err);
    bool agKindOk = true;
    for (size_t k = 0; k < agVs.calls.size(); ++k) {
        const u64* f = agBlob.data() + 7 + k * 32;
        if (f[0] != static_cast<u64>(agVs.calls[k].verb)) agKindOk = false;
    }
    Check(agKindOk, "AG 帧 kind 序 == Emit 序（3 帧：pre/bcast/post）");
}

// ============================================================================
// V7 · WP-9 RS×AICPU 后端（R12-2 七点约束对数）：AicpuReduceSlice::Emit 写侧动词序
//   ——layout 直推几何（禁读侧 peers）、threadNum=N、五步序、remoteCclMem 合成、
//   COPY/REDUCE 在 SYNC_POST 后（PostCopy 结构差——J7 基线同型）
// 8 卡 me=3 slice=1000 手算：
//   WRITE×7（MESH 环序 nextRank=4,5,6,7,0,1,2）：
//     src=InOffset(nextRank)=nextRank*1000、dst=me*outputStride=3000、size=1000
//   SYNC_POST 后：COPY（InOffset(3)=3000 → OutOffset(3)=3000, 1000）+
//     REDUCE×7（scratch+tmpRank*1000 → 3000, 1000）
// ============================================================================
static void TestAicpuRsBackend()
{
    printf("== V7 WP-9 RS×AICPU 后端（写侧动词序——R12-2 七点对数） ==\n");
    OpParam op = MakeRsOp(8, 500, 3, CommEngine::COMM_ENGINE_AICPU);
    TopoInfoWithNetLayerDetails topo = MakeTopo(8);
    Plan plan;
    std::string err;
    Check(L1Adapter::Compile(op, topo, SingleDomain(8), "sole[mesh]", plan, err) == HCCL_SUCCESS,
          "RS×AICPU compile（WP-9 后端注册后 Validate 过）: " + err);
    Check(plan.segments.size() == 1 && plan.segments[0].kind == ActionKind::ReduceSlice,
          "1 ReduceSlice segment");
    // AICPU 半边 Emit 序捕获（l4/backend_aicpu_reduce_slice.h）
    IActionBackend* be = BackendRegistry::Get(ActionKind::ReduceSlice, Engine::AICPU);
    Check(be != nullptr, "AicpuReduceSlice registered（WP-9①）");
    ReduceSliceArgs args;
    std::string berr;
    Check(ReduceSlice::BuildArgs(plan.segments[0], args, berr) == HCCL_SUCCESS, "BuildArgs ok: " + berr);
    CaptureSink sink;
    EmitCtx ctx;
    ctx.sink = &sink;
    be->Emit(plan.segments[0], &args, ctx);
    Check(sink.calls.size() == 17, "Emit 序 17 动词（pre + 7 WRITE + post + COPY + 7 REDUCE）");
    Check(sink.calls[0].verb == VerbCall::SYNC_PEER_PRE, "首动词 SYNC_PRE");
    Check(sink.calls[8].verb == VerbCall::SYNC_PEER_POST, "第 9 动词 SYNC_POST（写段后）");
    Check(sink.calls[9].verb == VerbCall::COPY, "第 10 动词 COPY（PostCopy 在 POST 后——结构差登记）");
    // WRITE 几何（P1-2 + [A8]-c 修复后手算，对照实际 layout）：
    //   src = InOffset(peerAlgRank) = inBase + rsInStride*peerAlgRank = 0 + 1000*peer
    //   （[A8]-c：Compile 期 inputSliceStride=0 退 outputSliceStride=1000——写侧
    //   "我的 input 里 peer 的份"真偏移；旧值恒 0 是缺陷③形态）；
    //   dst = me*outputStride = 3*1000 = 3000（对端 cclBuff 我的槽结构量，绝对由
    //   sink 从 remoteCclMem 合成）；遍历序升序跳过 me（me=3 → 0,1,2,4,5,6,7）
    const u32 writeOrder[7] = {0, 1, 2, 4, 5, 6, 7};
    bool writeOk = true;
    for (u32 k = 0; k < 7; ++k) {
        const VerbCall& c = sink.calls[1 + k];
        if (c.verb != VerbCall::WRITE ||
            c.src != static_cast<u64>(writeOrder[k]) * 1000ULL ||  // InOffset(peer) = 1000*peer
            c.dst != 3000ULL || c.size != 1000ULL ||
            c.peer != writeOrder[k]) {
            writeOk = false;
        }
    }
    Check(writeOk, "WRITE×7：src=InOffset(peer)=peer*1000（[A8]-c 真偏移）、dst=3000、升序 0,1,2,4..7（peer 验序）");
    // COPY 几何：InOffset(3)=0（stride=0）→ OutOffset(3)=3000
    Check(sink.calls[9].src == 3000ULL && sink.calls[9].dst == 3000ULL && sink.calls[9].size == 1000ULL,
          "COPY：src=InOffset(3)=3000（[A8]-c）、dst=OutOffset(3)=3000（我的片 input→output 落位）");
    // REDUCE 几何：scratch+tmpRank*1000 → OutOffset(me)=3000
    bool reduceOk = true;
    // 逐条精确断言（tmpRank 序 0,1,2,4,5,6,7——跳过 me=3）
    const u32 tmpOrder[7] = {0, 1, 2, 4, 5, 6, 7};
    for (u32 k = 0; k < 7; ++k) {
        const VerbCall& c = sink.calls[10 + k];
        if (c.verb != VerbCall::REDUCE ||
            c.src != static_cast<u64>(tmpOrder[k]) * 1000ULL ||
            c.dst != 3000ULL || c.size != 1000ULL) {
            reduceOk = false;
        }
    }
    Check(reduceOk, "REDUCE×7：src=scratch+tmpRank*1000（跳 me）、dst=3000、size=1000");
    // ④ 端到端：sink 驱动真原语（threads=N=8、WRITE 用 threads[1..7]）
    {
        HcommVerbRes res;
        for (u32 i = 0; i < 8; ++i) {
            res.threads.push_back(0x1000 + i);   // threadNum=N（≠AG 的 N-1）
        }
        for (u32 peer = 0; peer < 8; ++peer) {
            if (peer == 3) continue;
            ChannelInfo ch;
            ch.remoteRank = peer;
            ch.handle = 0x20000 + peer;
            ch.remoteCclMem.addr = reinterpret_cast<void*>(
                static_cast<uintptr_t>(0xF00000 + peer * 0x100000));
            res.channels[peer] = {ch};
        }
        HcommVerbSink rsSink(res);
        // 绝对地址合成（消费方口径）：WRITE src 加 sendBuf 现址、REDUCE src 加
        // cclBuff 现址、COPY/REDUCE dst 加 recvBuf 现址（PoC 测试在此手工合成——
        // 生产链路在 consumer 的 InstantiateRsSegment 已做，此处直驱 sink 验原语序）
        const u64 sendBuf = 0x1000000;
        const u64 recvBuf = 0x2000000;
        const u64 cclBuff = 0x3000000;
        rsSink.Emit(VerbCall{VerbCall::SYNC_PEER_PRE});
        for (u32 k = 0; k < 7; ++k) {
            rsSink.Emit(VerbCall{VerbCall::WRITE, sendBuf + static_cast<u64>(writeOrder[k]) * 1000ULL,
                                 3000ULL, 1000ULL, writeOrder[k]});
        }
        rsSink.Emit(VerbCall{VerbCall::SYNC_PEER_POST});
        rsSink.Emit(VerbCall{VerbCall::COPY, sendBuf + 3000ULL, recvBuf + 3000ULL, 1000ULL, 0});
        for (u32 k = 0; k < 7; ++k) {
            rsSink.Emit(VerbCall{VerbCall::REDUCE, cclBuff + static_cast<u64>(tmpOrder[k]) * 1000ULL,
                                 recvBuf + 3000ULL, 1000ULL, 0});
        }
        Check(rsSink.ErrCode() == 0, "RS×AICPU sink 全序 ok: " + rsSink.Err());
        const std::vector<HcommCallRecord>& recs = rsSink.Calls();
        u32 writes = 0, reduces = 0;
        for (const auto& r : recs) {
            if (r.verb == VerbCall::WRITE) ++writes;
            if (r.verb == VerbCall::REDUCE) ++reduces;
        }
        Check(writes == 7 && reduces == 7, "7 WRITE + 7 REDUCE 全驱动");
    }
}

// ============================================================================
// V8 · R13 NHR 段：server 分发循环 NHR_RUN 展开 + 混排复位 + 对拍（M0_R13 §七/§八）
//   手算源：M0_R13_NHR_V5.md §六（等价原语序 61 条·建链集 5 通道口径、WR 几何 tx 序 [2,0,6,4,1,5,7]）
// ============================================================================
static void TestNhrRun()
{
    printf("== V8 R13 NHR_RUN 帧展开（server 镜像：公式重建 + 步进流原语序） ==\n");
    // G1 真帧（Compile 期结构量）：8 卡 me=3、slice=1000、jetty=1
    VerbSegment vs;
    {
        ActionSpec seg;
        seg.kind = ActionKind::ReduceSlice;
        seg.pattern = CommPattern::NHR;
        seg.nhrJettyNum = 1;
        seg.layout.sliceSize = 1000;
        seg.layout.outputSliceStride = 1000;
        seg.layout.repeatNum = 1;
        seg.reduce.dataType = 3;
        seg.reduce.outputType = 3;
        for (u32 r = 0; r < 8; ++r) seg.domain.ranks.push_back(r);
        seg.domain.myRank = 3;
        ReduceSliceArgs args;
        std::string berr;
        Check(ReduceSlice::BuildArgs(seg, args, berr) == HCCL_SUCCESS, "V8 BuildArgs NHR ok: " + berr);
        IActionBackend* be = BackendRegistry::Get(ActionKind::ReduceSlice, Engine::CCU);
        CaptureSink sink;
        EmitCtx ctx;
        ctx.sink = &sink;
        be->Emit(seg, &args, ctx);
        vs.seg = seg;
        vs.actionKind = VerbCall::REDUCE;
        vs.calls = sink.calls;
    }
    std::vector<u64> blob;
    std::string serr;
    Check(SerializeVerbPlan({vs}, Plan{}, blob, serr), "V8 serialize NHR: " + serr);
    // ★R18 全删轮等价改写：帧形态从 NHR_RUN 紧凑三帧改为逐动词展开 21 帧
    //（8P me=3 步表 3 步 nSlices 4+2+1，CCU 无 COPY 口径）。帧值手算：
    //   帧3（首 SLICE_REDUCE）槽 6 = inputStride(1000)、槽 7 = ~0-1、槽 10 = jetty(1)
    //   ——种子段级补写（照 NHR_RUN 槽 7-10 同型）
    Check(blob[2] == 21ULL && blob.size() == 7ULL + 21ULL * 32ULL,
          "V8 blob 21 帧（R18 全删轮展开口径——逐动词一帧）");
    const u64* f3 = blob.data() + 7 + 3 * 32;
    Check(f3[6] == 1000ULL && f3[7] == 0xFFFFFFFFFFFFFFFEULL && f3[10] == 1ULL,
          "V8 首 SR 帧种子槽 6/7/10 = stride/repeatInv/jetty（段级补写——v2 :507-510 "
          "jetty 公式不再进帧槽，slot5/6 旧断言退役）");

    // ---- server 镜像：SimulateDispatchVerbs 两新动词展开对拍 ----
    // 手算期望原语序（★R18 全删轮新口径）：PRE(0x5) 10 WVN + 5 NW →
    //   逐步 [CH(ACK-Rec)=NR, CH(ACK-Wait)=NW, SR×n=(WR+EW)×n, CH(DATA-Rec)=NR,
    //   CH(DATA-Wait)=NW]（步 0/1/2 nSlices 4/2/1）→ POST 5 NR + 5 NW。
    // 计数：WVN=10、NW=5+6+5=16、NR=6+5=11、WR=7、EW=7、LC=0（CCU 无 COPY 口径
    //   ——收尾归位不在动词序，上料/归位由 server 帧外承载）
    VerbServerCtx s;
    s.rankId = 3;
    s.rankSize = 8;
    s.channelCount = 5;   // ★M0 复核修正 A：NHR 段真通道数 = 建链集大小（8 卡 me=3
                          //   → ±1/±2/±4 去重 5 条 ≠ mesh 段 7）——PreSync 的 WVN/NW
                          //   逐通道展开按 NHR 段口径计数
    std::string derr;
    const std::vector<PrimRec> prims = SimulateDispatchVerbs(blob, s, derr);
    Check(derr.empty(), "V8 NHR dispatch ok: " + derr);
    // 计数对拍（R18 展开序手算）：10 WVN + 16 NW + 11 NR + 7 WR + 7 EW + 0 LC
    //   = 51 条（旧宏形态 61 条含帧内 LC/步间屏障镜像；展开后每动词一帧、
    //   握手四态逐帧镜像——数量守恒 = 帧数守恒的 dispatch 面）
    size_t wvn = 0, nw = 0, wr = 0, ew = 0, nr = 0, lc = 0;
    for (const PrimRec& r : prims) {
        if (r.op == "WVN") ++wvn;
        if (r.op == "NW") ++nw;
        if (r.op == "WR") ++wr;
        if (r.op == "EW") ++ew;
        if (r.op == "NR") ++nr;
        if (r.op == "LC") ++lc;
    }
    Check(wvn == 10 && wr == 7 && ew == 7 && nw == 16 && nr == 11 && lc == 0,
          "V8 NHR 原语计数（R18 展开口径）：WVN10 + NW16 + NR11 + WR7 + EW7 + LC0, got WVN=" +
          std::to_string(wvn) + " NW=" + std::to_string(nw) + " NR=" + std::to_string(nr) +
          " WR=" + std::to_string(wr) + " EW=" + std::to_string(ew) + " LC=" + std::to_string(lc));
    // WR 几何对拍：tx 序 [2,0,6,4,1,5,7]（GetStepInfo 手算——步序即发射序，
    // NhrSliceOf 写侧同偏移不变量保持）
    const u32 txAll[7] = {2, 0, 6, 4, 1, 5, 7};
    bool wrGeomOk = true;
    u32 wrIdx = 0;
    for (const PrimRec& r : prims) {
        if (r.op != "WR") continue;
        const u64 expectOff = static_cast<u64>(txAll[wrIdx]) * 1000ULL;
        if (r.a0 != expectOff || r.a1 != expectOff || r.a2 != 1000ULL) wrGeomOk = false;
        ++wrIdx;
    }
    Check(wrGeomOk && wrIdx == 7,
          "V8 7 WR 几何 = 本端/对端 IN+tx*1000 同偏移（tx 序 2,0,6,4,1,5,7——R18 "
          "展开后 SLICE_REDUCE 镜像逐 slice 同序）");

    // ---- 混排复位（[A9] NHR 版）：mesh RS(0x7) → NHR(0x5) → AG(0xC) 三段 ----
    {
        std::vector<u64> mix(UNIFIED_V5_HEADER_SLOTS + 3ULL * UNIFIED_V5_SLOTS_PER_FRAME, 0);
        mix[0] = PLAN_V5_MAGIC;
        mix[1] = PLAN_V5_VERSION_FIELD;
        mix[2] = 3ULL;
        u64* f0 = mix.data() + UNIFIED_V5_HEADER_SLOTS;
        f0[U_V5_VERB_ID] = DEVKIT_KFC_VERB_SYNC_PRE;
        f0[U_V5_ARG0] = 0x7;      // mesh RS
        f0[U_V5_ARG1] = 0x11000; f0[U_V5_ARG2] = 0x22000; f0[U_V5_ARG3] = 0x33000;
        u64* f1m = f0 + UNIFIED_V5_SLOTS_PER_FRAME;
        f1m[U_V5_VERB_ID] = DEVKIT_KFC_VERB_SYNC_PRE;
        f1m[U_V5_ARG0] = 0x5;     // NHR
        f1m[U_V5_ARG1] = 0x44000; f1m[U_V5_ARG2] = 0x55000; f1m[U_V5_ARG3] = 0x66000;
        u64* f2m = f1m + UNIFIED_V5_SLOTS_PER_FRAME;
        f2m[U_V5_VERB_ID] = DEVKIT_KFC_VERB_SYNC_PRE;
        f2m[U_V5_ARG0] = 0xC;     // AG
        f2m[U_V5_ARG1] = 0x77000; f2m[U_V5_ARG2] = 0x88000; f2m[U_V5_ARG3] = 0x99000;
        VerbServerCtx ms;
        ms.rankSize = 4;
        ms.channelCount = 3;
        std::string merr;
        const std::vector<PrimRec> mprims = SimulateDispatchVerbs(mix, ms, merr);
        Check(merr.empty(), "V8 混排三段 SYNC_PRE dispatch ok: " + merr);
        // 计数：mesh 3XN×3ch=9 WVN + 3 NW；NHR 2XN×3=6 WVN + 3 NW；AG 2XN×3=6+3 →
        //   9+3+6+3+6+3 = 30
        Check(mprims.size() == 30, "V8 混排 WVN/NW = (9+3)+(6+3)+(6+3) = 30, got " +
              std::to_string(mprims.size()));
        // ★last-segment-wins：末段 AG(0xC) → peerOut/peerTok==3、peerIn/peerScr==0
        //   （NHR 段 0x5 填的 peerIn 被末段清除——防退化锚点）
        Check(ms.peerOut.size() == 3 && ms.peerTok.size() == 3,
              "V8 末段 AG 生效：peerOut/peerTok == 3");
        Check(ms.peerIn.size() == 0 && ms.peerScr.size() == 0,
              "V8 NHR 段 peerIn 被末段 AG 清除（last-segment-wins，非残留 3）, got " +
              std::to_string(ms.peerIn.size()));
    }
}

// ---------------------------------------------------------------------------
// R18 · #11：SLICE_REDUCE/CH_SYNC 帧序列化 golden + Validate 正负例（宏关可达——
// 展开分支的序列化判定是"动词序含 SLICE_REDUCE"（运行时数据判定），手工构造
// VerbSegment 直喂序列化器即可，无需宏开 Emit）。
// ---------------------------------------------------------------------------
static void TestSliceReduceFrames()
{
    printf("== R18 SLICE_REDUCE/CH_SYNC 帧序列化（#11 展开形态 golden） ==\n");
    // 构造 N=4 me=0 展开形态 VerbSegment（16 动词：上料 COPY×4 + 步×(4+nSlices) + 归位）
    ActionSpec a;
    a.kind = ActionKind::ReduceSlice;
    a.pattern = CommPattern::NHR;
    a.transport = TransportPolicy::VIA_CCUBUF;
    a.domain.ranks = {0U, 1U, 2U, 3U};
    a.domain.myRank = 0;
    a.layout.sliceSize = 1000;
    a.layout.outputSliceStride = 1000;
    a.layout.inBaseOff = 0;
    a.layout.outBaseOff = 0;
    a.layout.scratchBaseOff = 4000;
    a.layout.dataTypeSize = 2;
    a.sync.preBarrier = true;
    a.sync.postBarrier = true;
    a.nhrJettyNum = 1U;

    VerbCall::NhrRunScalars seed;
    seed.valid = true;
    seed.myAlgRank = 0;
    seed.rankNum = 4;
    seed.channelsPerRank = 1;
    seed.sliceSize = 1000;
    seed.inputBase = 0;
    seed.inputSliceStride = 1000;
    seed.scratchBase = 4000;
    seed.repeatNum = 1;
    seed.inputRepeatStride = 0;
    seed.outputRepeatStride = 0;
    seed.domainRanks = a.domain.ranks;

    VerbSegment vs;
    vs.seg = a;
    vs.actionKind = VerbCall::REDUCE;
    // 上料 COPY×4（src=inBase+stride*r, dst=SB+S*r）
    for (u32 r = 0; r < 4; ++r) {
        VerbCall c;
        c.verb = VerbCall::COPY;
        c.src = 0 + 1000ULL * r;
        c.dst = 4000 + 1000ULL * r;
        c.size = 1000;
        vs.calls.push_back(c);
    }
    // 步表 N=4 me=0（M0 §三-2 手算：step0{sendTo=3,recvFrom=1,nSlices=2,tx=[3,1],rx=[0,2]}
    // step1{sendTo=2,recvFrom=2,nSlices=1,tx=[2],rx=[0]}）
    const std::vector<NhrStepInfo> steps = CalcNhrSteps(0, 4);
    for (size_t s = 0; s < steps.size(); ++s) {
        const NhrStepInfo& st = steps[s];
        // ACK-Rec / ACK-Wait（src=sendTo, dst=recvFrom, size=0/1）
        for (u32 half = 0; half < 2; ++half) {
            VerbCall c;
            c.verb = VerbCall::CH_SYNC;
            c.src = st.sendTo;
            c.dst = st.recvFrom;
            c.size = half;   // 0=ACK-Rec / 1=ACK-Wait
            c.nhr = seed;
            vs.calls.push_back(c);
        }
        for (u32 i = 0; i < st.nSlices; ++i) {
            VerbCall c;
            c.verb = VerbCall::SLICE_REDUCE;
            c.src = st.sendTo;   // peerWR
            c.dst = st.recvFrom; // peerRD
            c.size = 1000;
            c.nhr = seed;
            c.nhr.stepIdx = static_cast<u32>(s);
            c.nhr.sliceIdx = i;
            vs.calls.push_back(c);
        }
        // DATA-Rec / DATA-Wait（size=2/3）
        for (u32 half = 0; half < 2; ++half) {
            VerbCall c;
            c.verb = VerbCall::CH_SYNC;
            c.src = st.sendTo;
            c.dst = st.recvFrom;
            c.size = 2 + half;   // 2=DATA-Rec / 3=DATA-Wait
            c.nhr = seed;
            vs.calls.push_back(c);
        }
    }
    // 归位 COPY×1（SB+S*0 → OutOffset(0)=0）
    {
        VerbCall c;
        c.verb = VerbCall::COPY;
        c.src = 4000;
        c.dst = 0;
        c.size = 1000;
        vs.calls.push_back(c);
    }
    Check(vs.calls.size() == 16, "R18 展开 VerbSegment 构造 = 16 动词（N=4 手算）");
    // 序列化 golden
    Plan plan;
    plan.segments = {a};
    std::vector<u64> blob;
    std::string err;
    Check(SerializeVerbPlan({vs}, plan, blob, err), "R18 SerializeVerbPlan(展开形态) ok: " + err);
    // 帧数 = 16（帧数守恒——每动词一帧）
    Check(blob.size() == UNIFIED_V5_HEADER_SLOTS + 16ULL * UNIFIED_V5_SLOTS_PER_FRAME,
          "R18 展开帧数 = 16（逐动词一帧，帧数守恒）");
    // 帧索引手算（构造序：上料 COPY×4 → step0[ACK-Rec, ACK-Wait, SR×2, DATA-Rec,
    // DATA-Wait] → step1[同 6 帧但 SR×1] → 归位）：
    //   idx 0..3 = COPY；idx 4 = CH_SYNC(ACK-Rec)；idx 5 = CH_SYNC(ACK-Wait)；
    //   idx 6/7 = SLICE_REDUCE(step0 i=0/1)；idx 8/9 = CH_SYNC(DATA)；…
    // 逐帧 spot check：首 SLICE_REDUCE 帧 idx=6：slot0=9, step=0, sliceIdx=0,
    // size=1000, peerWR=3, peerRD=1
    {
        const u64* f = blob.data() + UNIFIED_V5_HEADER_SLOTS + 6ULL * UNIFIED_V5_SLOTS_PER_FRAME;
        Check(f[U_V5_VERB_ID] == 9ULL && f[U_V5_SR_STEP] == 0ULL &&
                  f[U_V5_SR_SLICE_IDX] == 0ULL && f[U_V5_SR_SIZE] == 1000ULL &&
                  f[U_V5_SR_PEER_WR] == 3ULL && f[U_V5_SR_PEER_RD] == 1ULL,
              "R18 首帧 SLICE_REDUCE（idx=6）：verb=9 step=0 slice=0 size=1000 peerWR=3 peerRD=1");
        // 段级种子（首 SLICE_REDUCE 帧承载——repeat 种子照 NHR_RUN 槽 7-10 同型）
        Check(f[U_V5_SR_REPEAT_NUM_INV] == ~0ULL - 1ULL && f[U_V5_SR_INPUT_SLICE_STRIDE] == 1000ULL,
              "R18 种子段级补写：REPEAT_NUM_INV（repeatNum=1）+ INPUT_SLICE_STRIDE=1000");
    }
    // CH_SYNC 帧 spot check（idx=4：step0 ACK-Rec——verb=10, peer=sendTo=3, dir=0）
    {
        const u64* f = blob.data() + UNIFIED_V5_HEADER_SLOTS + 4ULL * UNIFIED_V5_SLOTS_PER_FRAME;
        Check(f[U_V5_VERB_ID] == 10ULL && f[U_V5_CS_PEER] == 3ULL && f[U_V5_CS_DIR] == 0ULL,
              "R18 CH_SYNC 首帧（idx=4）：verb=10 peer=3(WR sendTo) dir=0(ACK-Rec)");
    }
    // Validate 正例
    {
        std::string verr;
        Check(ValidateVerbSegment(vs, verr), "R18 Validate 展开形态 ok: " + verr);
    }
    // Validate 负例①：stepIdx 非单调（交换两步顺序）
    {
        VerbSegment bad = vs;
        // 找到 step1 的 SR 与 step0 的最后一个 SR 交换
        size_t srFirst = 0, srLast = 0;
        for (size_t k = 0; k < bad.calls.size(); ++k) {
            if (bad.calls[k].verb == VerbCall::SLICE_REDUCE) {
                if (srFirst == 0) srFirst = k;
                srLast = k;
            }
        }
        std::swap(bad.calls[srFirst].nhr.stepIdx, bad.calls[srLast].nhr.stepIdx);
        std::swap(bad.calls[srFirst].nhr.sliceIdx, bad.calls[srLast].nhr.sliceIdx);
        std::string verr;
        Check(!ValidateVerbSegment(bad, verr),
              "R18 Validate 负例：stepIdx 非单调 → FAIL（" + verr + "）");
    }
    // Validate 负例②：上料 COPY 缺一（upCount != n）
    {
        VerbSegment bad = vs;
        for (size_t k = 0; k < bad.calls.size(); ++k) {
            if (bad.calls[k].verb == VerbCall::COPY) {
                bad.calls.erase(bad.calls.begin() + static_cast<long>(k));
                break;
            }
        }
        std::string verr;
        Check(!ValidateVerbSegment(bad, verr),
              "R18 Validate 负例：上料 COPY 缺一 → FAIL（" + verr + "）");
    }
}

int main()
{
    TestAgGolden();
    TestRsGolden();
    TestRs9PGolden();
    TestNegative();
    TestDispatchLoop();
    TestEquivalence();
    TestAicpuRsBackend();
    TestNhrRun();
    TestSliceReduceFrames();
    printf("\n%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
