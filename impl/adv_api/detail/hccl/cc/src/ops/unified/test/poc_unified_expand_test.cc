// unified PoC · L2 Expand 接线验证（host 侧自检，不依赖 devkit 编译环境）
// devkit 类型面经 fake_alg_param.h 注入（自检用；正式件用真实 alg_param.h）

#include "l2/l1_adapter.h"
#include "inc/unified_route.h"   // 第三轮 A5：路由表 / algTag / UseUnifiedPath
#include "l2/alg_config_parser.h"
#include "l2/recipe_registry.h"
#include "inc/tier_expand.h"           // R9：TierParams/ExpandTieredSegments（⑮ 节）
#include "l3/action_collect_all.h"
#include "l3/action_reduce_slice.h"      // R6：ReduceSlice::BuildArgs（② 段生成断言）
#include "l4/backend_aicpu_collect_all.h"
#include "l4/backend_ccu_collect_all.h"
#include "l4/backend_ccu_reduce_slice.h" // R6：CCU RS 后端（include 即注册——② 的
                                        // Validate 缺口断言翻转与 claim.kernel 断言）
#include "verb_sink.h"

#include <cassert>
#include <cstdio>
using namespace mc2_ops_hccl::refactor;

static mc2_ops_hccl::refactor::CcuKernelHandle dummyHandle = 0;   // M3-条件A：Materialize 出参占位
static int g_pass = 0, g_fail = 0;
static void Check(bool cond, const std::string& what)
{
    if (cond) { ++g_pass; printf("  [PASS] %s\n", what.c_str()); }
    else      { ++g_fail; printf("  [FAIL] %s\n", what.c_str()); }
}

// 子串计数（postCopy/read 条数断言用）
static size_t CountOccur(const std::string& s, const std::string& sub)
{
    size_t cnt = 0, pos = 0;
    while ((pos = s.find(sub, pos)) != std::string::npos) { ++cnt; pos += sub.size(); }
    return cnt;
}

static OpParam MakeAgOp(u32 rankSize, u64 countPerRank, CommEngine engine = CommEngine::COMM_ENGINE_AICPU)
{
    OpParam op;
    op.opType = static_cast<int>(HcclCMDType::HCCL_CMD_ALLGATHER);
    op.DataDes.count = countPerRank;
    op.DataDes.dataType = HCCL_DATA_TYPE_FP16;
    op.engine = engine;
    return op;
}

// rankSize 现在在 TopoInfo（真实 alg_param.h:210，shim 已对齐）——用例经此 helper 构造 topo
static TopoInfoWithNetLayerDetails MakeTopo(u32 rankSize, Level0Shape shape = Level0Shape::MESH_1D)
{
    TopoInfoWithNetLayerDetails topo;      // 裸构造（MakeTopo 内不得自调用）
    topo.userRankSize = rankSize;
    topo.topoLevelNums = 1;
    topo.level0Topo = shape;
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
    printf("== ① 基础链路：AG sole[mesh] → 1 段 CollectAll，plan 可 BuildArgs/Emit ==\n");
    {
        OpParam op = MakeAgOp(8, 512 * 1024);   // 8 卡 AG，FP16 每卡 512K 元素 = 1MB
        TopoInfoWithNetLayerDetails topo = MakeTopo(8);
        Plan plan; std::string err;
        Check(L1Adapter::Compile(op, topo, SingleDomain(8), "sole[mesh]", plan, err) == HCCL_SUCCESS,
              "compile ok: " + err);
        Check(plan.segments.size() == 1, "1 segment");
        Check(plan.segments[0].kind == ActionKind::CollectAll, "kind=CollectAll");
        Check(plan.segments[0].pattern == CommPattern::MESH, "pattern=MESH");
        Check(plan.segments[0].domain.Size() == 8, "domain=8 ranks");

        // ③段被消费：BuildArgs → Emit（AICPU read×7 / CCU broadcast）
        LoopSplitter splitter;
        CollectAllArgs args = CollectAll::BuildArgs(plan.segments[0], splitter);
        Check(args.peers.size() == 7, "BuildArgs: 7 peers");
        EmitCtx ctxA, ctxC;
        BackendRegistry::Get(ActionKind::CollectAll, Engine::AICPU)->Emit(plan.segments[0], &args, ctxA);
        BackendRegistry::Get(ActionKind::CollectAll, Engine::CCU)->Emit(plan.segments[0], &args, ctxC);
        Check(ctxA.trace.find("read") != std::string::npos, "AICPU emits read verbs");
        Check(ctxC.trace.find("broadcast") != std::string::npos, "CCU emits broadcast verb");
        Check(plan.claim.channels.size() == 7, "claim: 7 channel requests");
    }

    printf("== ② AR two_shot：sole[mesh.two_shot] → RS→AG 两段带数据流边 ==\n");
    {
        OpParam op = MakeAgOp(8, 1024 * 1024, CommEngine::COMM_ENGINE_CCU);
        op.opType = static_cast<int>(HcclCMDType::HCCL_CMD_ALLREDUCE);
        TopoInfoWithNetLayerDetails topo = MakeTopo(8);
        Plan plan; std::string err;
        // R6 起 ReduceSlice 的 CCU 后端已落（backend_ccu_reduce_slice.h）——Compile 全链
        //（含 Validate）通过；AICPU 引擎仍有缺口（RS 的 AICPU 后端未落 → NOT_SUPPORT，
        // P2 交付物），与 ②' 的 Compile 链断言互补
        Check(L1Adapter::Compile(op, topo, SingleDomain(8), "sole[mesh.two_shot]", plan, err) ==
                  HCCL_SUCCESS,
              "R6: two_shot CCU compile passes (RS backend delivered): " + err);
        Check(plan.segments.size() == 2 && plan.claim.kernels.size() == 2,
              "R6 compile: 2 segments (RS+AG), 2 kernel entries");
        // Expand 结构（与既有口径一致；注意 Expand 追加语义——plan 已有段时 operator+=
        // 逐条追加，此处新开 plan 复核纯 Expand 产物）
        const AlgConfig cfgX = AlgConfigParser::Parse("sole[mesh.two_shot]");
        const PrimitiveRequest reqX = L1Adapter::BuildRequest(op, topo, SingleDomain(8));
        ExpandCtx ctxX; ctxX.domainCount = 1; ctxX.hierarchyRanks = SingleDomain(8);
        Plan planX;
        Check(RecipeRegistry::Get(Recipe::SOLE)->Expand(reqX, cfgX, ctxX, planX, err) == HCCL_SUCCESS,
              "expand ok");
        Check(planX.segments.size() == 2, "2 segments");
        Check(planX.segments[0].kind == ActionKind::ReduceSlice, "seg0=ReduceSlice");
        Check(planX.segments[1].kind == ActionKind::CollectAll, "seg1=CollectAll");
        Check(planX.segments[1].inputRanks == planX.segments[0].outputRanks, "dataflow edge rs→ag");
        // R6 段生成新增断言：RS 段 CalcRes 落地——claim.kernels 含两条（RS 的
        // CcuActionReduceSlice + AG 的 CcuActionCollectAll，operator+= 逐条追加不合并）
        Check(planX.claim.kernels.size() == 2, "R6 claim.kernels: RS + AG two entries (no merge)");
        Check(planX.claim.kernels[0].kernelFuncName == "CcuActionReduceSlice",
              "claim.kernels[0] = CcuActionReduceSlice (RS segment first)");
        Check(planX.claim.kernels[0].channels.size() == 7 && planX.claim.kernels[1].channels.size() == 7,
              "each kernel plan: 7 peer channels");
        // RS 段可 BuildArgs + CCU Emit 动词序（trace 口径：pre + read×7 + reduce + post）
        ReduceSliceArgs rargs;
        std::string berr;
        Check(ReduceSlice::BuildArgs(planX.segments[0], rargs, berr) == HCCL_SUCCESS,
              "RS BuildArgs ok: " + berr);
        EmitCtx ctxR;
        BackendRegistry::Get(ActionKind::ReduceSlice, Engine::CCU)->Emit(planX.segments[0], &rargs, ctxR);
        Check(CountOccur(ctxR.trace, "read(") == 7 && CountOccur(ctxR.trace, "reduce(") == 1 &&
                  CountOccur(ctxR.trace, "syncPeer:pre") == 1 && CountOccur(ctxR.trace, "syncPeer:post") == 1,
              "RS CCU Emit: pre + read*7 + reduce + post");
        // AICPU 引擎缺口仍在（RS 的 AICPU 后端 P2）——快速失败不静默
        OpParam opA = MakeAgOp(8, 1024 * 1024, CommEngine::COMM_ENGINE_AICPU);
        opA.opType = static_cast<int>(HcclCMDType::HCCL_CMD_ALLREDUCE);
        Plan planAicpu; std::string errA;
        Check(L1Adapter::Compile(opA, topo, SingleDomain(8), "sole[mesh.two_shot]", planAicpu, errA) ==
                  HCCL_E_NOT_SUPPORT &&
              errA.find("not in P1 delivery scope") != std::string::npos,
              "AICPU engine still reports RS backend gap (P2): " + errA);
    }

    printf("== ③ AR AUTO 档：小数据 one_shot（CollectAll+LocalReduce）、大数据 two_shot ==\n");
    {
        OpParam op = MakeAgOp(8, 128 * 1024);   // 256KB ≤ 8MiB → one_shot
        op.opType = static_cast<int>(HcclCMDType::HCCL_CMD_ALLREDUCE);
        TopoInfoWithNetLayerDetails topo = MakeTopo(8);
        Plan plan; std::string err;
        const AlgConfig cfg = AlgConfigParser::Parse("sole");
        const PrimitiveRequest req = L1Adapter::BuildRequest(op, topo, SingleDomain(8));
        ExpandCtx ctx; ctx.domainCount = 1; ctx.hierarchyRanks = SingleDomain(8);
        Check(RecipeRegistry::Get(Recipe::SOLE)->Expand(req, cfg, ctx, plan, err) == HCCL_SUCCESS,
              "expand ok");
        Check(plan.segments.size() == 2, "small AR: 2 segs (one_shot)");
        Check(plan.segments[0].kind == ActionKind::CollectAll &&
              plan.segments[1].kind == ActionKind::LocalReduce, "one_shot = CollectAll+LocalReduce");

        OpParam opBig = MakeAgOp(8, 1024 * 1024);
        opBig.opType = static_cast<int>(HcclCMDType::HCCL_CMD_ALLREDUCE);
        opBig.DataDes.count = 8 * 1024 * 1024 / 2; // sliceSize = 8MiB → total = 64MiB > 8MiB → two_shot
        Plan plan2; std::string err2;
        const AlgConfig cfg2 = AlgConfigParser::Parse("sole");
        const PrimitiveRequest req2 = L1Adapter::BuildRequest(opBig, topo, SingleDomain(8));
        Check(RecipeRegistry::Get(Recipe::SOLE)->Expand(req2, cfg2, ctx, plan2, err2) == HCCL_SUCCESS,
              "expand big ok");
        Check(plan2.segments.size() == 2 && plan2.segments[0].kind == ActionKind::ReduceSlice,
              "big AR AUTO: two_shot (RS→AG)");
    }

    printf("== ④ chunk 切分：sole[mesh.chunk] → 2 段，子段并集==原区间 ==\n");
    {
        OpParam op = MakeAgOp(8, 512 * 1024);
        TopoInfoWithNetLayerDetails topo = MakeTopo(8);
        Plan plan; std::string err;
        Check(L1Adapter::Compile(op, topo, SingleDomain(8), "sole[mesh.chunk]", plan, err) ==
                  HCCL_SUCCESS,
              "compile ok: " + err);
        Check(plan.segments.size() == 2, "chunk: 2 segments");
        const u64 full = plan.segments[0].layout.sliceSize * 2;
        const u64 sum = plan.segments[0].layout.sliceSize + plan.segments[1].layout.sliceSize;
        Check(sum == full || plan.segments[0].layout.tailSize != 0,
              "sub-segment union == original span");
    }

    printf("== ⑤ 多域：parallel[mesh,nhr] → 2 段（域 0 mesh + 域 1 nhr） ==\n");
    {
        OpParam op = MakeAgOp(16, 64 * 1024);
        TopoInfoWithNetLayerDetails topo = MakeTopo(16);   // rankSize 在 TopoInfo（真实 :210）
        topo.topoLevelNums = 2;
        std::vector<std::vector<u32>> hier{
            {0, 1, 2, 3, 4, 5, 6, 7}, {8, 9, 10, 11, 12, 13, 14, 15}};
        Plan plan; std::string err;
        Check(L1Adapter::Compile(op, topo, hier, "parallel[mesh,nhr]", plan, err) == HCCL_SUCCESS,
              "compile ok: " + err);
        Check(plan.segments.size() == 2, "parallel: 2 segments");
        Check(plan.segments[0].pattern == CommPattern::MESH &&
              plan.segments[1].pattern == CommPattern::NHR, "per-domain pattern");
        Check(plan.segments[0].domain.Size() == 8 && plan.segments[1].domain.Size() == 8,
              "each domain 8 ranks");
    }

    printf("== ⑥ 快速失败矩阵（Validate 不静默降级，错误码分类） ==\n");
    {
        TopoInfoWithNetLayerDetails topoClos = MakeTopo(8);
        topoClos.level0Topo = Level0Shape::CLOS;    // 非全互联
        OpParam op = MakeAgOp(8, 64 * 1024);
        Plan plan; std::string err;
        Check(L1Adapter::Compile(op, topoClos, SingleDomain(8), "sole[mesh]", plan, err) ==
                  HCCL_E_NOT_SUPPORT &&
              err.find("fully-connected") != std::string::npos, "CLOS + mesh rejected: " + err);

        err.clear();
        Check(L1Adapter::Compile(op, topoClos, SingleDomain(8), "CcuAllGatherMesh1DMem2Mem",
              plan, err) == HCCL_E_PARA && err.find("retired") != std::string::npos,
              "retired name rejected (E_PARA)");

        err.clear();
        Check(L1Adapter::Compile(op, topoClos, SingleDomain(8), "parallel[nhr]", plan, err) ==
                  HCCL_E_PARA &&
              err.find("cannot locate") != std::string::npos, "parallel single stage rejected");

        err.clear();
        Plan planX;
        HcclResult retX = L1Adapter::Compile(op, topoClos, SingleDomain(8), "sole[nhr.two_shot]",
                                             planX, err);
        Check(retX == HCCL_E_PARA && err.find("only valid for AllReduce") != std::string::npos,
              "two_shot on AllGather rejected: " + err);

        err.clear();
        Plan planY;
        Check(L1Adapter::Compile(op, topoClos, SingleDomain(8), "pipeline[mesh]", planY, err) ==
                  HCCL_E_NOT_SUPPORT,
              "pipeline (retired) rejected (E_NOT_SUPPORT)");

        // 能力：CCU 64bit 规约不支持（dataType FP64=10 → size 8）
        OpParam op64 = MakeAgOp(8, 64 * 1024, CommEngine::COMM_ENGINE_CCU);
        op64.DataDes.dataType = HCCL_DATA_TYPE_FP64;
        op64.opType = static_cast<int>(HcclCMDType::HCCL_CMD_ALLREDUCE);
        err.clear();
        Plan planZ;
        // topo 全互联 + AR 小数据 one_shot：LocalReduce 段 → P2 缺口先拦（64bit 校验在其后）
        TopoInfoWithNetLayerDetails topoMesh = MakeTopo(8);
        HcclResult retZ = L1Adapter::Compile(op64, topoMesh, SingleDomain(8), "sole", planZ, err);
        Check(retZ == HCCL_E_NOT_SUPPORT, "CCU FP64 AR rejected (P2 gap or 64bit caps): " + err);
    }

    printf("== ⑦ 切轮（§10.2 A2）：容量约束 → loopTimes 数学、逐轮 offset、尾轮余量 ==\n");
    {
        OpParam op = MakeAgOp(8, 512 * 1024);      // sliceSize = 1MB
        TopoInfoWithNetLayerDetails topo = MakeTopo(8);
        Plan plan; std::string err;
        // cclBuff=1MB、scratchMultiple=2、512 对齐 → scratchBound=512KB → maxPerRound=512KB
        RoundCapacity cap;
        cap.cclBuffSize = 1 * 1024 * 1024;
        cap.scratchMultiple = 2;
        HcclResult ret = L1Adapter::Compile(op, topo, SingleDomain(8), "sole[mesh]", plan, err, cap);
        Check(ret == HCCL_SUCCESS, "compile with capacity ok: " + err);
        Check(plan.segments.size() == 2, "1MB slice / 512KB round → 2 rounds");
        Check(plan.segments[0].layout.sliceSize == 512 * 1024, "round0 slice = 512KB (整轮)");
        Check(plan.segments[1].layout.sliceSize == 512 * 1024, "round1 slice = 512KB (整轮)");
        Check(plan.segments[1].layout.inBaseOff == 512 * 1024 &&
                  plan.segments[1].layout.outBaseOff == 512 * 1024,
              "round1 baseOff 推进 512KB");

        // 尾轮余量：1.25MB / 512KB → 3 轮（512K + 512K + 256K）
        OpParam op2 = MakeAgOp(8, 640 * 1024);     // sliceSize = 1.25MB
        Plan plan2; err.clear();
        RoundCapacity cap2;
        cap2.cclBuffSize = 1 * 1024 * 1024;
        cap2.scratchMultiple = 2;
        ret = L1Adapter::Compile(op2, topo, SingleDomain(8), "sole[mesh]", plan2, err, cap2);
        Check(ret == HCCL_SUCCESS && plan2.segments.size() == 3, "1.25MB / 512KB → 3 rounds");
        Check(plan2.segments[2].layout.sliceSize == 256 * 1024, "尾轮余量 256KB");
        Check(plan2.segments[2].layout.inBaseOff == 1024 * 1024, "尾轮 baseOff = 1MB");

        // 512 对齐：maxPerRound 向下对齐（cclBuff=2.5MB/mul2 = 1.25MB → 对齐后 1MB+ 页内对齐值）
        RoundCapacity cap3;
        cap3.cclBuffSize = 5 * 1024 * 1024;        // /2 = 2.5MB → /512*512 = 2.5MB 已对齐（5MB=5120*512）
        Plan plan3; err.clear();
        OpParam op3 = MakeAgOp(8, 1024 * 1024);    // 2MB slice
        ret = L1Adapter::Compile(op3, topo, SingleDomain(8), "sole[mesh]", plan3, err, cap3);
        Check(ret == HCCL_SUCCESS && plan3.segments.size() == 1, "2MB ≤ 2.5MB(已对齐) → 单轮");

        // symmem：不受中转约束（现状 executor 同分支：maxDataSizePerLoop = dataSize）
        OpParam opS = MakeAgOp(8, 4 * 1024 * 1024); // 8MB slice
        opS.supportSymmetricMemory = true;          // AUTO transport → SYMMEM
        Plan planS; err.clear();
        RoundCapacity capS;
        capS.cclBuffSize = 1024 * 1024;             // 中转只有 1MB，本应切 8 轮
        ret = L1Adapter::Compile(opS, topo, SingleDomain(8), "sole[mesh]", planS, err, capS);
        Check(ret == HCCL_SUCCESS && planS.segments.size() == 1,
              "symmem 单轮完成（不受中转约束）");

        // 不带容量（既有调用面）：单轮语义不破坏
        Plan planNoCap; err.clear();
        OpParam opBig = MakeAgOp(8, 8 * 1024 * 1024);  // 16MB，无约束下仍 1 段
        ret = L1Adapter::Compile(opBig, topo, SingleDomain(8), "sole[mesh]", planNoCap, err);
        Check(ret == HCCL_SUCCESS && planNoCap.segments.size() == 1,
              "无容量约束（缺省）→ 单轮语义保持");
    }

    printf("== ⑧ LoopSplitter（§6.5/B12）：CalGoSize 公式逐字段对照 ==\n");
    {
        LoopSplitter::LoopGroupConfig cfg;   // 默认：loopCount=128, memSlice=4096 → loopSize=512KB
        const u64 loopSize = 512ULL * 1024;
        const u64 memSlice = 4096;

        // 整除：size = 2×loopSize → m=2,n=0,p=0；addrOffset=2×loopSize
        LoopSplitter::GoSize g1 = LoopSplitter::Calc(2 * loopSize, cfg);
        Check(g1.addrOffset == 2 * loopSize && g1.loopParam == 2 && g1.parallelParam == 0 &&
                  g1.residual == 0,
              "整除：m=2, 零头=0");

        // 有零头 loop + 尾块：size = 2×loopSize + 3×memSlice + 1000
        LoopSplitter::GoSize g2 = LoopSplitter::Calc(2 * loopSize + 3 * memSlice + 1000, cfg);
        Check(g2.addrOffset == 2 * loopSize && g2.loopParam == 2 &&
                  g2.parallelParam == LoopSplitter::ParallelParam(2, 1, 2) && g2.residual == 1000,
              "零头+尾块：parallelParam=PP(2,1,2), residual=1000");

        // 只有尾块：size = 2×loopSize + 1000
        LoopSplitter::GoSize g3 = LoopSplitter::Calc(2 * loopSize + 1000, cfg);
        Check(g3.parallelParam == LoopSplitter::ParallelParam(0, 0, 1) && g3.residual == 1000,
              "只有尾块：parallelParam=PP(0,0,1)");

        // 零头整 loop（n!=0,p==0）：size = 2×loopSize + 3×memSlice
        LoopSplitter::GoSize g4 = LoopSplitter::Calc(2 * loopSize + 3 * memSlice, cfg);
        Check(g4.parallelParam == LoopSplitter::ParallelParam(2, 0, 1) && g4.residual == memSlice,
              "零头整 loop：parallelParam=PP(2,0,1), residual=memSlice");

        // 小数据（m==0）：size = 3×memSlice + 1000
        LoopSplitter::GoSize g5 = LoopSplitter::Calc(3 * memSlice + 1000, cfg);
        Check(g5.addrOffset == 0 && g5.loopParam == 0 &&
                  g5.parallelParam == LoopSplitter::ParallelParam(2, 1, 2) && g5.residual == 1000,
              "小数据 m=0：只走零头+尾块");

        // 常数本身用字面量守住（审查 MAJOR-1：12bit 全 1 = 8191，SetBits(12)=(1<<13)-1）
        Check(LoopSplitter::MaxLoopIterNum() == 8191, "MaxLoopIterNum() == 8191（字面量守常数）");

        // 取满边界：size == maxSize = 8192 × loopSize（8192 = 8191+1，字面写死——
        // 期望值不得引用被测常数，防"错常数自洽"）
        const u64 maxSize = 8192ULL * loopSize;
        LoopSplitter::GoSize g6 = LoopSplitter::Calc(maxSize, cfg);
        Check(g6.loopParam == 8191 && g6.addrOffset == 8191ULL * loopSize &&
                  g6.parallelParam == LoopSplitter::ParallelParam(cfg.loopCount - 2, 1, 2) &&
                  g6.residual == memSlice,
              "取满边界 size==maxSize：m=8191, n=127, p=memSlice → PP(126,1,2)");

        // 2GiB~4GiB 区间防越界误判（审查补充：size=3GiB 落在取满前区间，走正常路径）：
        //   3GiB / 512KB loopSize → m = floor(3×2^30 / 2^19) = 6144 ≤ 8191，余 3GiB−6144×512KB
        //   = 0（3GiB = 6144×512KB 整除）→ n=0, p=0 → 整除分支，零头=0
        const u64 size3G = 3ULL * 1024 * 1024 * 1024;
        LoopSplitter::GoSize g7 = LoopSplitter::Calc(size3G, cfg);
        Check(g7.loopParam == 6144 && g7.loopParam <= 8191 &&
                  g7.addrOffset == 6144ULL * loopSize && g7.parallelParam == 0 && g7.residual == 0,
              "3GiB 正常路径：m=6144 ≤ 8191，整除零头=0（防越界误判回归）");

        // ParallelParam 位域：与现状 GetParallelParam 三段位域一致（55/48/41 移位）
        Check(LoopSplitter::ParallelParam(2, 1, 2) ==
                  ((2ULL << 55) | (1ULL << 48) | (2ULL << 41)),
              "ParallelParam 位域 = (n<<55)|(idx<<48)|(total<<41)");

        // 兼容面：goSize[4] 薄包装
        LoopSplitter splitter;
        u64 go[4] = {0, 0, 0, 0};
        splitter.Calc(2 * loopSize + 1000, go);
        Check(go[0] == g3.addrOffset && go[1] == g3.loopParam && go[2] == g3.parallelParam &&
                  go[3] == g3.residual,
              "goSize[4] 薄包装与 GoSize 结构一致");
    }

    printf("== ⑨ kernel 注册条目（§10.2 C9）：CcuKernelPlan 进 claim ==\n");
    {
        OpParam op = MakeAgOp(8, 512 * 1024, CommEngine::COMM_ENGINE_CCU);
        TopoInfoWithNetLayerDetails topo = MakeTopo(8);
        Plan plan; std::string err;
        Check(L1Adapter::Compile(op, topo, SingleDomain(8), "sole[mesh]", plan, err) == HCCL_SUCCESS,
              "compile ok: " + err);
        Check(plan.claim.kernels.size() == 1, "claim: 1 kernel plan entry");
        Check(!plan.claim.kernels.empty() &&
                  plan.claim.kernels[0].kernelFuncName == "CcuActionCollectAll",
              "kernelFuncName = CcuActionCollectAll（动作 kernel，非算法名）");
        Check(!plan.claim.kernels.empty() && plan.claim.kernels[0].channels.size() == 7,
              "kernel plan channels: 7 peers × 1 channel");

        // 通道限定词联动：multi-channel:2 → 每 peer 2 条
        Plan planMc; err.clear();
        HcclResult ret = L1Adapter::Compile(op, topo, SingleDomain(8), "sole[mesh.multi-channel:2]",
                                            planMc, err);
        Check(ret == HCCL_SUCCESS && planMc.claim.kernels[0].channels.size() == 7 &&
                  planMc.claim.kernels[0].channels[0].count == 2,
              "multi-channel:2 → 7 peers × 2 channels in kernel plan");

        // chunk 切分联动（审查 MINOR-1）：2 段 → kernels 逐条追加翻倍（operator+= 不合并）
        Plan planChunk; err.clear();
        ret = L1Adapter::Compile(op, topo, SingleDomain(8), "sole[mesh.chunk]", planChunk, err);
        Check(ret == HCCL_SUCCESS && planChunk.claim.kernels.size() == 2,
              "chunk 2 段 → kernels 翻倍为 2 条（逐条追加不合并）");
    }

    printf("== ⑩ 动词映射表（§10.2 A1）：两引擎每动词有 target、无空洞 ==\n");
    {
        // 七动词汇表全枚举（REDUCE 属 P2 动作但表行必须已在——映射表是 P2 接线工单）
        const VerbCall::Verb all[] = {VerbCall::COPY,     VerbCall::POST_COPY,  VerbCall::READ,
                                      VerbCall::WRITE,    VerbCall::REDUCE,     VerbCall::BROADCAST,
                                      VerbCall::SYNC_PEER_PRE,                 VerbCall::SYNC_PEER_POST};
        u32 aicpuHit = 0, ccuHit = 0;
        for (VerbCall::Verb v : all) {
            if (LookupVerbMap(v, /*isCcu*/ false) != nullptr) ++aicpuHit;
            if (LookupVerbMap(v, /*isCcu*/ true) != nullptr) ++ccuHit;
        }
        Check(aicpuHit == 8, "AICPU 表：8 动词全覆盖（含 P2 的 REDUCE 占位行）");
        Check(ccuHit == 8, "CCU 表：8 动词全覆盖");

        // 每行 target/note 非空（dump 可读性：§6.13.3 表 1 的行 = 接线工单）
        bool targetsOk = true;
        for (VerbCall::Verb v : all) {
            const VerbMapEntry* eA = LookupVerbMap(v, false);
            const VerbMapEntry* eC = LookupVerbMap(v, true);
            if (eA == nullptr || eC == nullptr ||
                std::string(eA->target).empty() || std::string(eA->note).empty() ||
                std::string(eC->target).empty() || std::string(eC->note).empty()) {
                targetsOk = false;
            }
        }
        Check(targetsOk, "每行 target+note 非空（P2 接线工单完整）");

        // 关键行内容抽查：READ 的 AICPU 表行必须注明 ACK/DATA_SIGNAL 握手（A1 的要点）
        const VerbMapEntry* eRead = LookupVerbMap(VerbCall::READ, false);
        Check(eRead != nullptr &&
                  std::string(eRead->target).find("SendRecvRead") != std::string::npos &&
                  std::string(eRead->note).find("ACK") != std::string::npos,
              "AICPU READ 行：SendRecvRead + ACK/DATA_SIGNAL 握手注记");
        const VerbMapEntry* eBc = LookupVerbMap(VerbCall::BROADCAST, true);
        Check(eBc != nullptr && std::string(eBc->target).find("GroupBroadcast") != std::string::npos,
              "CCU BROADCAST 行：GroupBroadcast 目标");
    }

    printf("== ⑪ PostLocalCopy（§10.2 B4）：VIA_CCUBUF 段后回拷、SYMMEM 无此步 ==\n");
    {
        // VIA_CCUBUF：默认 transport（非 symmem 的 AUTO 落点）→ trace 含 postCopy
        OpParam op = MakeAgOp(8, 64 * 1024);
        TopoInfoWithNetLayerDetails topo = MakeTopo(8);
        Plan plan; std::string err;
        Check(L1Adapter::Compile(op, topo, SingleDomain(8), "sole[mesh]", plan, err) == HCCL_SUCCESS,
              "compile ok (VIA_CCUBUF): " + err);
        LoopSplitter splitter;
        CollectAllArgs args = CollectAll::BuildArgs(plan.segments[0], splitter);
        EmitCtx ctxA;
        BackendRegistry::Get(ActionKind::CollectAll, Engine::AICPU)->Emit(plan.segments[0], &args, ctxA);
        // §10.2 B4 勘误（审查 BLOCKER-1）：PostLocalCopy 无调用点（死代码），read 直达
        // output——任何 transport 下 trace 均不含 postCopy
        Check(CountOccur(ctxA.trace, "postCopy(") == 0,
              "AICPU VIA_CCUBUF：无 postCopy（read 直达 output，PostLocalCopy 系死代码）");
        const size_t readCnt = CountOccur(ctxA.trace, "read(");
        Check(readCnt == 7, "read×7（单段搬运，无双倍回拷）");

        // SYMMEM：直读 output，无 postCopy
        OpParam opSym = op;
        opSym.supportSymmetricMemory = true;
        Plan planSym; err.clear();
        Check(L1Adapter::Compile(opSym, topo, SingleDomain(8), "sole[mesh,symmem]", planSym, err) ==
                  HCCL_SUCCESS,
              "compile ok (SYMMEM): " + err);
        CollectAllArgs argsSym = CollectAll::BuildArgs(planSym.segments[0], splitter);
        EmitCtx ctxSym;
        BackendRegistry::Get(ActionKind::CollectAll, Engine::AICPU)->Emit(planSym.segments[0], &argsSym,
                                                                          ctxSym);
        Check(CountOccur(ctxSym.trace, "postCopy(") == 0, "SYMMEM：无 postCopy");

        // CCU 恒无 postCopy（broadcast 的 dst 数组含本 rank 落位，一步到位）
        EmitCtx ctxC;
        BackendRegistry::Get(ActionKind::CollectAll, Engine::CCU)->Emit(plan.segments[0], &args, ctxC);
        Check(CountOccur(ctxC.trace, "postCopy(") == 0, "CCU：无 postCopy（并入 broadcast）");

        // 可注入 sink：结构化 VerbCall 流（A1 架构验证——后端不感知 sink 是谁）
        std::vector<VerbCall> fired;
        struct CaptureSink : IVerbSink {
            std::vector<VerbCall>* out;
            explicit CaptureSink(std::vector<VerbCall>* o) : out(o) {}
            void Emit(const VerbCall& c) override { out->push_back(c); }
        };
        CaptureSink cap(&fired);
        EmitCtx ctxCap;
        ctxCap.sink = &cap;
        BackendRegistry::Get(ActionKind::CollectAll, Engine::AICPU)->Emit(plan.segments[0], &args,
                                                                           ctxCap);
        Check(fired.size() > 0 && fired.back().verb == VerbCall::SYNC_PEER_POST,
              "CaptureSink：末动词 = SYNC_PEER_POST（结构化流可注入）");
        bool hasRead = false;
        for (const VerbCall& c : fired) hasRead = hasRead || (c.verb == VerbCall::READ);
        Check(hasRead, "CaptureSink：READ 以枚举形式到达（非字符串）");
    }

    printf("== ⑫ token 获取链（§10.2 B8）：优先级 + 伪 token 确定性 + 可注入 ==\n");
    {
        // 优先级 1：input 非空 → token 基于 inputVa（伪 token = va ^ sliceSize）
        OpParam op = MakeAgOp(8, 64 * 1024);
        op.inputPtr = reinterpret_cast<void*>(0x1000);
        op.outputPtr = reinterpret_cast<void*>(0x2000);
        TopoInfoWithNetLayerDetails topo = MakeTopo(8);
        Plan plan; std::string err;
        Check(L1Adapter::Compile(op, topo, SingleDomain(8), "sole[mesh]", plan, err) == HCCL_SUCCESS,
              "compile ok (input+output): " + err);
        const u64 sliceBytes = 64 * 1024 * 2;   // FP16
        Check(plan.segments[0].token == (0x1000ULL ^ sliceBytes),
              "优先级①：input 非空 → token = inputVa ^ sliceSize");

        // 优先级 2：input 空、output 非空 → 基于 outputVa
        OpParam opOut = op;
        opOut.inputPtr = nullptr;
        Plan planOut; err.clear();
        Check(L1Adapter::Compile(opOut, topo, SingleDomain(8), "sole[mesh]", planOut, err) ==
                  HCCL_SUCCESS,
              "compile ok (output only): " + err);
        Check(planOut.segments[0].token == (0x2000ULL ^ sliceBytes),
              "优先级②：input 空 → token = outputVa ^ sliceSize");

        // 双空 → 0（host 单测场景；P2 真机 Validate 拒 0 token）
        OpParam opNone = op;
        opNone.inputPtr = nullptr;
        opNone.outputPtr = nullptr;
        Plan planNone; err.clear();
        Check(L1Adapter::Compile(opNone, topo, SingleDomain(8), "sole[mesh]", planNone, err) ==
                  HCCL_SUCCESS,
              "compile ok (no va): " + err);
        Check(planNone.segments[0].token == 0, "双空 → token=0（P2 真机拒）");

        // 可注入真实现：tokenOf 覆写伪 token（P2 注 GetTokenInfo 的口子）
        Plan planInj; err.clear();
        Check(L1Adapter::Compile(op, topo, SingleDomain(8), "sole[mesh]", planInj, err,
                                 RoundCapacity{}, /*tokenOf*/ [](u64 va, u64) { return va + 7; }) ==
                  HCCL_SUCCESS,
              "compile ok (injected tokenOf): " + err);
        Check(planInj.segments[0].token == 0x1000ULL + 7,
              "注入 tokenOf：token = f(va)（真实现入口验证）");

        // token 进消息槽（BuildArgs 半边，回归：ACTION_ARGS 的 TOKEN 槽位）
        LoopSplitter splitter;
        CollectAllArgs args = CollectAll::BuildArgs(plan.segments[0], splitter);
        Check(args.slots.TOKEN == plan.segments[0].token, "token 进消息槽（slots.TOKEN）");
    }

    printf("== ⑬ two_shot 中转建模（§12-9 B2）：RS 落 scratch、AG 从 scratch 取 ==\n");
    {
        OpParam op = MakeAgOp(8, 1024 * 1024);
        op.opType = static_cast<int>(HcclCMDType::HCCL_CMD_ALLREDUCE);
        op.DataDes.count = 4 * 1024 * 1024 / 2;   // sliceSize=4MB；总 32MB > 8MiB → two_shot
        TopoInfoWithNetLayerDetails topo = MakeTopo(8);
        Plan plan; std::string err;
        const AlgConfig cfg = AlgConfigParser::Parse("sole[mesh.two_shot]");
        const PrimitiveRequest req = L1Adapter::BuildRequest(op, topo, SingleDomain(8));
        ExpandCtx ctx; ctx.domainCount = 1; ctx.hierarchyRanks = SingleDomain(8);
        Check(RecipeRegistry::Get(Recipe::SOLE)->Expand(req, cfg, ctx, plan, err) == HCCL_SUCCESS,
              "expand ok");
        Check(plan.segments.size() == 2, "two_shot: 2 segments");

        // 手算基线（独立于实现，字面量）：构造时 inBaseOff=0x100、scratchBaseOff=0x5000、
        // sliceSize=4MB、outputSliceStride=4MB
        const DataLayout& rsL = plan.segments[0].layout;
        const DataLayout& agL = plan.segments[1].layout;
        // RS 段 output 仍写真实 output（B2 中转不改 RS 的 output 半边——RS 收敛到 scratch 的
        // 语义由 AG 的 input 半边表达：两段共享同一 scratch 区）
        Check(agL.inBaseOff == rsL.scratchBaseOff, "AG input 基址 = RS 布局的 scratch 基址");
        Check(agL.inputSliceStride == 0,
              "AG input 步长 = 0（对任意 peer 取数源恒 scratch[0]，现状 :297 归并终点 :363/:367 取数源）");
        Check(agL.outBaseOff == rsL.outBaseOff && agL.outputSliceStride == rsL.outputSliceStride,
              "AG output 半边不变（仍写真实 output）");
        // 逐值：AG 对全 peer 的取数源等值 == scratch 基址（0 号槽），与 RS 归并终点一致
        //（现状 :297 reduceDstSlice offset=hcclBuffBaseOffset、:363/:367 恒取 hcclBuffBaseOffset）
        const u32 n = 8;
        bool const0 = true;
        for (u32 r = 0; r < n; ++r) {
            if (agL.InOffset(r) != rsL.scratchBaseOff) { const0 = false; }
        }
        Check(const0, "逐值：AG InOffset(r) 对全 peer 等值 == scratchBase（scratch[0] 恒定槽）");

        // sequence 配方回归：ForSecondStage 仍是域间衔接语义（前域 output 即后域 input）
        DataLayout seqL = rsL;
        seqL.ForSecondStage();   // 非 const 调用确认可编译（返回值语义测试见下）
        const DataLayout next = rsL.ForSecondStage();
        Check(next.inBaseOff == rsL.outBaseOff && next.inputSliceStride == rsL.outputSliceStride,
              "ForSecondStage 保留（sequence 域间衔接语义不变）");
    }

    printf("== ⑭ 第三轮 A5：路由表 / Materialize / algTag / Plan 序列化 ==\n");
    {
        // 路由表：AG/RS 命中、其余不命中（编译期开关经 unified_route.h 的 MC2_UNIFIED_RESOURCE 控制，
        // host 自检无该宏 → UseUnifiedPath 恒 false——用路由表数据本身断言圈定范围）。
        // R7 · WP-2 翻转断言：RS 行激活（AG 1 行 → AG+RS 2 行）——改回注释态本断言必 FAIL
        Check(sizeof(kUnifiedRoutes) / sizeof(kUnifiedRoutes[0]) == 2,
              "route table: AG + RS in scope (R7 WP-2 activated)");
        Check(kUnifiedRoutes[0].op == HcclCMDType::HCCL_CMD_ALLGATHER &&
              kUnifiedRoutes[0].kind == ActionKind::CollectAll,
              "route entry 0 = ALLGATHER -> CollectAll");
        Check(kUnifiedRoutes[1].op == HcclCMDType::HCCL_CMD_REDUCE_SCATTER &&
              kUnifiedRoutes[1].kind == ActionKind::ReduceSlice,
              "route entry 1 = REDUCE_SCATTER -> ReduceSlice (R7 · WP-2 gate)");
        // 防退化反例：R6 旧形态（RS 注释态、表长 1）下上述两条断言 FAIL——病态可判
#ifndef MC2_UNIFIED_RESOURCE
        Check(UseUnifiedPath(static_cast<int>(HcclCMDType::HCCL_CMD_ALLGATHER)) == false,
              "compile-time switch off: UseUnifiedPath false even for AG");
#endif

        // Materialize：NoopBroker 下 claim→申请单翻译逐字段（8 卡 AG：7 通道、threads=8）
        OpParam op = MakeAgOp(8, 512 * 1024);
        TopoInfoWithNetLayerDetails topo = MakeTopo(8);
        Plan plan; std::string err;
        Check(L1Adapter::Compile(op, topo, SingleDomain(8), "sole[mesh]", plan, err) == HCCL_SUCCESS,
              "compile ok: " + err);
        NoopBroker broker;
        Check(L1Adapter::Materialize(plan, Engine::AICPU, broker, err, dummyHandle) == HCCL_SUCCESS, "materialize aicpu ok");
        // 8 卡 AG：slaveThreadNum = N-2 = 6（AG 主线程也认领一个 peer，§6.13.2 实证
        // ins_temp_all_gather_mesh_1D.cc:36-37）→ Materialize 申请 threads = 6+1 = 7
        // notify = max(notifyNumOnMainThread=6, perThread=1)+1 = 7
        //（AG 主线程 notify = threadNum-1 = 6，ins_temp_all_gather_mesh_1D.cc:40 现状公式同源）
        Check(broker.calls.size() == 2 && broker.calls[0] == "threads:7+notify:7" &&
              broker.calls[1] == "chan:7",
              "AICPU broker calls: threads:7 + notify:7 + chan:7");
        // CCU：kernel 注册 + 通道（无线程）
        NoopBroker brokerCcu;
        Check(L1Adapter::Materialize(plan, Engine::CCU, brokerCcu, err, dummyHandle) == HCCL_SUCCESS, "materialize ccu ok");
        Check(brokerCcu.calls.size() == 2 && brokerCcu.calls[0] == "ccuk:1" && brokerCcu.calls[1] == "chan:7",
              "CCU broker calls: ccuk:1 + chan:7");

        // algTag 新键格式（C10）
        const std::string tag = UnifiedAlgTag("sole[mesh]", Engine::CCU);
        Check(tag == "tag_unified_sole[mesh]_ccu", "algTag format: tag_unified_<cfg>_<engine>");
        // MAJOR-3-①（三轮审查）：规范形归一——空串/带空格/紧凑形三种输入必须同键
        const std::string t1 = UnifiedAlgTag("", Engine::AICPU);
        const std::string t2 = UnifiedAlgTag(" sole[mesh] ", Engine::AICPU);
        const std::string t3 = UnifiedAlgTag("sole[mesh]", Engine::AICPU);
        Check(t1 == "tag_unified_sole_aicpu", "empty algConfig -> canonical 'sole'");
        Check(t2 == t3, "space-trimmed algConfig -> same key as compact form");

        // Plan 序列化：段数/首段字段/claim 摘要（round-trip 关键内容）
        const std::string blob = SerializePlanForCtx(plan);
        // v1 摘要 → v3 版本号（第四轮 A4：Plan.version 升 3，摘要函数沿用仅格式头跟随）
        Check(blob.find("PLAN v3 segs:1") == 0, "plan blob header: v3 segs:1");
        Check(blob.find("kind:0") != std::string::npos && blob.find("chan:1") != std::string::npos,
              "plan blob seg fields: kind/pattern/chan present");
        // claim 摘要：threads=6（N-2 slave）；kernels=1（CalcRes 汇总两引擎申报——CCU 半边
        // 填 kernels，引擎选择留在申报之后，§6.13.2；AICPU Materialize 不消费它）
        Check(blob.find("claim threads:6 channels:7 kernels:1") != std::string::npos,
              "plan blob claim summary: aicpu(slaveThreads=6,channels=7,kernels=1)");
        // chunk 2 段的序列化（segments 数进 blob 头）
        Plan planChunk; std::string errChunk;
        Check(L1Adapter::Compile(op, topo, SingleDomain(8), "sole[mesh.chunk]", planChunk, errChunk) ==
              HCCL_SUCCESS, "compile chunk ok");
        Check(SerializePlanForCtx(planChunk).find("segs:2") != std::string::npos,
              "plan blob header: segs:2 for chunk");
    }

    printf("== ⑮ R9 档位参数：TierParams 缺省零变化 + parallel RS 骨架挂载 ==\n");
    {
        // ---- 防退化第一闸：无 tier 的既有 Plan 逐字段零变化 ----
        // sole[mesh] AG（最老形态）：Plan 无 tier 字段消费方——hasTier 恒 false，
        // 段数/类型/claim 全部与 R7 基线一致（改回"AG 也挂 tier"或字段漂移必 FAIL）
        {
            OpParam op = MakeAgOp(8, 512 * 1024);
            TopoInfoWithNetLayerDetails topo = MakeTopo(8);
            Plan plan; std::string err;
            Check(L1Adapter::Compile(op, topo, SingleDomain(8), "sole[mesh]", plan, err) ==
                      HCCL_SUCCESS, "⑮ sole AG compile ok: " + err);
            Check(plan.tier.hasTier == false, "⑮ 防退化：sole AG Plan 不携带 tier（缺省零变化）");
            Check(plan.segments.size() == 1 && plan.segments[0].kind == ActionKind::CollectAll,
                  "⑮ 防退化：sole AG 1 段 CollectAll（R7 基线不变）");
        }
        // parallel[mesh,nhr] AG（多域但非 RS）：不挂 tier（RS 专属——AG 无档位需求，
        // selector 无 AG 的 4MiB 分档）。防退化反例：若误把 tier 挂到 AG parallel，
        // 以下断言 FAIL（ForkBuggy 式病态可判）
        {
            OpParam op = MakeAgOp(16, 64 * 1024);
            TopoInfoWithNetLayerDetails topo = MakeTopo(16);
            topo.topoLevelNums = 2;
            std::vector<std::vector<u32>> hier{
                {0, 1, 2, 3, 4, 5, 6, 7}, {8, 9, 10, 11, 12, 13, 14, 15}};
            Plan plan; std::string err;
            Check(L1Adapter::Compile(op, topo, hier, "parallel[mesh,nhr]", plan, err) ==
                      HCCL_SUCCESS, "⑮ parallel AG compile ok: " + err);
            Check(plan.segments.size() == 2, "⑮ 防退化：parallel AG 仍 2 段骨架（R7 基线）");
            Check(plan.tier.hasTier == false,
                  "⑮ 防退化：parallel AG 不挂 tier（档位是 RS 专属，AG 误挂必 FAIL）");
        }
        // ---- parallel RS 骨架挂载：tier 字段值逐项手算 ----
        // ★终审 MAJOR-2 修复后：慢轴带宽按域 1 成员数分档 {2→29, 4→35}，其余成员数
        //   tier 不置位（骨架直跑语义安全）。本节三态：
        //   ① 域 1 = 4 成员（PCIeMix 16 卡档）→ tier 挂 35/(4-1)；
        //   ② 域 1 = 2 成员（PCIeMix 8 卡档）→ tier 挂 29/(2-1)（终审第二组值锚）；
        //   ③ 域 1 = 8 成员（非分档拓扑）→ tier 不置位（防退化反例，硬编码 35 时代
        //      会误挂——本断言改回必 FAIL）。
        // threshBytes=4MiB（selector :19）；A partRatio=1/2（:229-234）；align=512。
        {
            // ① 4 成员慢域（4 server × 8 卡的 32 rank，取双域切面 8+4）
            OpParam op4 = MakeAgOp(12, 64 * 1024, CommEngine::COMM_ENGINE_CCU);
            op4.opType = static_cast<int>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER);
            TopoInfoWithNetLayerDetails topo4 = MakeTopo(12);
            topo4.topoLevelNums = 2;
            std::vector<std::vector<u32>> hier4{
                {0, 1, 2, 3, 4, 5, 6, 7}, {0, 8, 9, 10}};
            Plan plan4; std::string err4;
            Check(L1Adapter::Compile(op4, topo4, hier4, "parallel[mesh,nhr]", plan4, err4) ==
                      HCCL_SUCCESS, "⑮ parallel RS（4 成员慢域）compile ok: " + err4);
            Check(plan4.segments.size() == 2 &&
                      plan4.segments[0].kind == ActionKind::ReduceSlice,
                  "⑮ parallel RS 骨架：2 段 ReduceSlice（每域 1 段）");
            Check(plan4.tier.hasTier == true, "⑮ parallel RS（4 成员慢域）挂 tier（R9 增量）");
            Check(plan4.tier.threshBytes == 4ULL * 1024 * 1024,
                  "⑮ tier.thresh = 4MiB（selector :19 常数）");
            Check(plan4.tier.interleave.partRatioNum == 1 && plan4.tier.interleave.partRatioDen == 2,
                  "⑮ tier.A partRatio = 1/2（GetParallelDataSplit :229-234 口径）");
            Check(plan4.tier.stepwise.bwFastNum == 50 && plan4.tier.stepwise.bwFastDen == 1,
                  "⑮ tier.B 快轴带宽 = 50/1（BW_OMNI_DEFAULT）");
            Check(plan4.tier.stepwise.bwSlowNum == 35 && plan4.tier.stepwise.bwSlowDen == 3,
                  "⑮ tier.B 慢轴带宽 = 35/(4-1)（16 卡档，eqBw :365-366 口径）");
            Check(plan4.tier.stepwise.alignBytes == 512, "⑮ tier.B 对齐粒度 = 512");
            // ② 2 成员慢域（PCIeMix 8 卡档 → 29 常数，MAJOR-2 分档判据）
            OpParam op2 = MakeAgOp(10, 64 * 1024, CommEngine::COMM_ENGINE_CCU);
            op2.opType = static_cast<int>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER);
            TopoInfoWithNetLayerDetails topo2 = MakeTopo(10);
            topo2.topoLevelNums = 2;
            std::vector<std::vector<u32>> hier2{
                {0, 1, 2, 3, 4, 5, 6, 7}, {0, 8}};
            Plan plan2; std::string err2;
            Check(L1Adapter::Compile(op2, topo2, hier2, "parallel[mesh,nhr]", plan2, err2) ==
                      HCCL_SUCCESS, "⑮ parallel RS（2 成员慢域）compile ok: " + err2);
            Check(plan2.tier.hasTier == true && plan2.tier.stepwise.bwSlowNum == 29 &&
                      plan2.tier.stepwise.bwSlowDen == 1,
                  "⑮ tier.B 慢轴带宽 = 29/1（8 卡档分档——MAJOR-2：硬编码 35 必 FAIL）");
            // ③ 8 成员慢域（非分档拓扑）→ tier 不置位（骨架直跑，无步进序列）
            OpParam op8 = MakeAgOp(16, 64 * 1024, CommEngine::COMM_ENGINE_CCU);
            op8.opType = static_cast<int>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER);
            TopoInfoWithNetLayerDetails topo8 = MakeTopo(16);
            topo8.topoLevelNums = 2;
            std::vector<std::vector<u32>> hier8{
                {0, 1, 2, 3, 4, 5, 6, 7}, {8, 9, 10, 11, 12, 13, 14, 15}};
            Plan plan8; std::string err8;
            Check(L1Adapter::Compile(op8, topo8, hier8, "parallel[mesh,nhr]", plan8, err8) ==
                      HCCL_SUCCESS, "⑮ parallel RS（8 成员慢域）compile ok: " + err8);
            Check(plan8.tier.hasTier == false,
                  "⑮ 防退化：非分档慢域（8 成员）tier 不置位（骨架直跑——改回硬编码必 FAIL）");
        }
    }

    printf("== ⑯ R9 A 交错展开：4 段序列数学手算（sync 位/域/part 切分） ==\n");
    {
        // 场景对齐老 Parallel 版：32 rank = 4 server × 8 卡（域 0 mesh 8、域 1 nhr 4），
        // 单块 3MiB（FP16 count=1.5M → 3MiB < 4MiB → A 交错）。
        // 手算基线（对齐 §1.1 图 + ziliao :435-459）：
        //   part0 = part1 = 1.5MiB；排列 [mesh(P0), nhr(P1), nhr(P0), mesh(P1)]
        //   sync：seg0 pre=T post=F；seg1 pre=T post=T（批1边界）；
        //         seg2 pre=T post=F；seg3 pre=T post=T（批2边界）
        OpParam op = MakeAgOp(32, 1536 * 1024, CommEngine::COMM_ENGINE_CCU);   // RS 走 CCU（AICPU 后端 P2 缺口）
        op.opType = static_cast<int>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER);
        TopoInfoWithNetLayerDetails topo = MakeTopo(32);
        topo.topoLevelNums = 2;
        std::vector<std::vector<u32>> hier;
        {
            std::vector<u32> mesh{0, 1, 2, 3, 4, 5, 6, 7};
            std::vector<u32> nhr{0, 8, 16, 24};   // 4 server 同位卡 0
            hier = {mesh, nhr};
        }
        Plan plan; std::string err;
        Check(L1Adapter::Compile(op, topo, hier, "parallel[mesh,nhr]", plan, err) ==
                  HCCL_SUCCESS, "⑯ compile ok: " + err);
        Check(plan.tier.hasTier == true, "⑯ tier present");
        Check(plan.segments.size() == 2, "⑯ 骨架 2 段（blob 序列化口径=骨架段数）");
        // 序列化尺寸守卫（口径修正）：v4 blob = 7 + 2×32 = 71 槽——骨架进 blob、
        // 展开在消费方。★但 mesh,nhr 双域骨架的 NHR 段被 SerializeSegmentV3 拒收
        //（M0-④ 协议缺口：v3 帧无 pattern 载体——R9 维持拒收，WP-4 方案落文档），
        // 故 blob 尺寸断言用 **MESH+MESH 双域**骨架验证（同段数同几何，NHR 帧不进
        // 断言路径）；mesh,nhr 骨架的 v3 拒收本身是另一条防退化断言（见下）。
        {
            OpParam opM = MakeAgOp(32, 1536 * 1024, CommEngine::COMM_ENGINE_CCU);
            opM.opType = static_cast<int>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER);
            TopoInfoWithNetLayerDetails topoM = MakeTopo(32);
            topoM.topoLevelNums = 2;
            // 双 mesh 域（两 server 组各 8 卡——平行域形态，段几何与 mesh,nhr 相同）
            std::vector<std::vector<u32>> hierM{
                {0, 1, 2, 3, 4, 5, 6, 7}, {8, 9, 10, 11, 12, 13, 14, 15}};
            Plan planM; std::string errM;
            Check(L1Adapter::Compile(opM, topoM, hierM, "parallel[mesh,mesh]", planM, errM) ==
                      HCCL_SUCCESS, "⑯ mesh,mesh RS compile ok: " + errM);
            std::vector<u64> blob;
            std::string serr;
            Check(SerializePlanV3(planM, blob, serr), "⑯' serialize mesh-mesh skeleton: " + serr);
            Check(blob.size() == 7 + 2 * 32,
                  "⑯ 骨架 blob = 7+2×32 槽（展开后段不进 blob——消费方 exec 期展开）");
            // mesh,nhr 骨架 v3 拒收（防退化：RS NHR 帧放开须 v5 协议，M0-④）
            std::vector<u64> blobN;
            std::string serrN;
            Check(!SerializePlanV3(plan, blobN, serrN),
                  "⑯ mesh,nhr RS 骨架 v3 拒收保持（pattern 载体缺口，M0-④ 勘误后仍不放开）");
            Check(serrN.find("pattern carrier") != std::string::npos,
                  "⑯ 拒收 err 指明 pattern 载体缺口（非槽宽误传）");
        }
        // A 展开（Instantiate 单决策点半边：ExpandTieredSegments 纯数据变换）
        std::vector<ActionSpec> expanded;
        std::string xerr;
        const u64 totalBytes = 3ULL * 1024 * 1024;
        Check(ExpandTieredSegments(plan.segments, plan.tier, totalBytes, expanded, xerr),
              "⑯ A expand ok: " + xerr);
        Check(expanded.size() == 4, "⑯ A 展开 4 段（两批 × 两域）");
        if (expanded.size() == 4) {
            // 排列：seg0=mesh P0、seg1=nhr P1、seg2=nhr P0、seg3=mesh P1
            Check(expanded[0].pattern == CommPattern::MESH && expanded[0].layout.inBaseOff == 0 &&
                      expanded[0].layout.sliceSize == 1536 * 1024,
                  "⑯ seg0 = mesh P0（基址 0、片 1.5MiB）");
            Check(expanded[1].pattern == CommPattern::NHR &&
                      expanded[1].layout.inBaseOff == 1536 * 1024 &&
                      expanded[1].layout.sliceSize == 1536 * 1024,
                  "⑯ seg1 = nhr P1（基址 1.5MiB、片 1.5MiB）");
            Check(expanded[2].pattern == CommPattern::NHR && expanded[2].layout.inBaseOff == 0 &&
                      expanded[2].layout.sliceSize == 1536 * 1024,
                  "⑯ seg2 = nhr P0（基址 0、片 1.5MiB）");
            Check(expanded[3].pattern == CommPattern::MESH &&
                      expanded[3].layout.inBaseOff == 1536 * 1024 &&
                      expanded[3].layout.sliceSize == 1536 * 1024,
                  "⑯ seg3 = mesh P1（基址 1.5MiB、片 1.5MiB）");
            // sync 位：批内 F/F、批边界 T/T（§3.4.5 合取屏障——按组语义：
            // {批成员区间 [0,1]/[2,3], 边界屏障 seg1.post+seg2.pre}）
            Check(expanded[0].sync.preBarrier == true && expanded[0].sync.postBarrier == false,
                  "⑯ seg0 sync = T/F（批内段：无 post 等待）");
            Check(expanded[1].sync.preBarrier == true && expanded[1].sync.postBarrier == true,
                  "⑯ seg1 sync = T/T（批 1 边界：合取屏障）");
            Check(expanded[2].sync.preBarrier == true && expanded[2].sync.postBarrier == false,
                  "⑯ seg2 sync = T/F（批 2 内段）");
            Check(expanded[3].sync.preBarrier == true && expanded[3].sync.postBarrier == true,
                  "⑯ seg3 sync = T/T（批 2 边界=序列末）");
            // 域正确性：批 1 = 域 0(P0)∥域 1(P1)——两段域成员不同（并发窗）；
            // 批 2 = 域 1(P0)∥域 0(P1)——对调
            Check(expanded[0].domain.Size() == 8 && expanded[1].domain.Size() == 4,
                  "⑯ 批 1：mesh 域 8 卡 ∥ nhr 域 4 卡（双域并发窗）");
            Check(expanded[2].domain.Size() == 4 && expanded[3].domain.Size() == 8,
                  "⑯ 批 2：nhr 域 4 卡 ∥ mesh 域 8 卡（对调）");
            // P 切分守恒：P0+P1 == 全量（每域各半，批内/批间并集覆盖整块）
            Check(expanded[0].layout.sliceSize + expanded[3].layout.sliceSize == totalBytes,
                  "⑯ mesh 域 P0+P1 == 3MiB（partRatio 切分守恒）");
            Check(expanded[1].layout.sliceSize + expanded[2].layout.sliceSize == totalBytes,
                  "⑯ nhr 域 P0+P1 == 3MiB（partRatio 切分守恒）");
        }
        // THRESH 边界（两侧）：3MiB+512 < 4MiB 仍是 A；恰好 4MiB 切 B（严格小于，
        // selector :378 `dataSize <` 同式——防退化：改成 <= 会使 4MiB 落 A 必 FAIL）
        {
            std::vector<ActionSpec> eA, eB;
            std::string aerr, berr;
            Check(ExpandTieredSegments(plan.segments, plan.tier, 4ULL * 1024 * 1024 - 512, eA, aerr) &&
                      eA.size() == 4,
                  "⑯ THRESH-512（3MiB+512K）→ A 交错 4 段");
            Check(ExpandTieredSegments(plan.segments, plan.tier, 4ULL * 1024 * 1024, eB, berr) &&
                      eB.size() == 12,
                  "⑯ 恰好 4MiB → B 步进（严格小于：== THRESH 走 B，selector 同式）");
        }
        // 展开失败路径（防御）：段数≠2 / totalBytes=0 显式拒收
        {
            std::vector<ActionSpec> bad = plan.segments;
            bad.push_back(plan.segments[0]);
            std::vector<ActionSpec> out;
            std::string ferr;
            Check(!ExpandTieredSegments(bad, plan.tier, totalBytes, out, ferr) &&
                      ferr.find("exactly 2") != std::string::npos,
                  "⑯ 防御：3 段骨架拒收（err 指明段数契约）");
            Check(!ExpandTieredSegments(plan.segments, plan.tier, 0, out, ferr) &&
                      ferr.find("totalBytes is 0") != std::string::npos,
                  "⑯ 防御：totalBytes=0 拒收（count 未注入显式失败）");
        }
    }

    printf("== ⑰ R9 B 步进展开：片宽 golden 复算（PCIeMix 32P 12MiB，步 0 比 10:1） ==\n");
    {
        // ★golden 复算锚（主会话 02:31 裁决硬要求，审查方 /tmp/r9_golden_b.py 对数）：
        //   场景 = PCIeMix 32P（4 server × 8 卡），单块 12MiB（≥4MiB → B 步进）。
        //   老公式（ziliao omnipipe_data_slice_calc.cc:345-463）内层轴序：慢轴 nhr 占
        //   x 槽、快轴 mesh 占 y 槽；带宽 eqBw0(mesh)=50、eqBw1(nhr)=35/(4-1)。
        //   审查方独立手算（golden 脚本逐行转录）：
        //     nhr（慢）= [154624, 1700864, 2429440, 3470336, 2380288, 2601984]
        //     mesh（快）= [1546240, 2429440, 3470336, 4982272, 10202624, 2380288]
        //     步 0 比 mesh/nhr = 1546240/154624 = 恰 10:1（bandwidthRatio=50/(35/3)
        //     ≈4.286 经 (yRankSize-1)/(xRankSize-1)=7/3 折算 → 10:1）
        //   ★断言不含 sum==total（finStepMark 末两步拆步使两轴序列和都偏离总量
        //   ——golden 实测 slow Σ=12,737,536 / fast Σ=25,011,200 / total=12,582,912）
        OpParam op = MakeAgOp(32, 6 * 1024 * 1024, CommEngine::COMM_ENGINE_CCU);   // RS 走 CCU（AICPU 后端 P2 缺口）
        op.opType = static_cast<int>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER);
        TopoInfoWithNetLayerDetails topo = MakeTopo(32);
        topo.topoLevelNums = 2;
        std::vector<std::vector<u32>> hier;
        {
            std::vector<u32> mesh{0, 1, 2, 3, 4, 5, 6, 7};
            std::vector<u32> nhr{0, 8, 16, 24};   // 4 server 同位卡
            hier = {mesh, nhr};
        }
        Plan plan; std::string err;
        Check(L1Adapter::Compile(op, topo, hier, "parallel[mesh,nhr]", plan, err) ==
                  HCCL_SUCCESS, "⑰ compile ok: " + err);
        // 慢轴（域 1）成员数 4 → tier 慢轴带宽分母 = 4-1 = 3（eqBw = 35/3）
        Check(plan.tier.stepwise.bwSlowNum == 35 && plan.tier.stepwise.bwSlowDen == 3,
              "⑰ tier 慢轴带宽 = 35/3（4 成员 nhr 域的 eqBw）");
        const StepwiseSlicePlan sp = CalcStepwiseSlices(
            12ULL * 1024 * 1024, 4, 8,
            plan.tier.stepwise.bwFastNum, plan.tier.stepwise.bwFastDen,
            plan.tier.stepwise.bwSlowNum, plan.tier.stepwise.bwSlowDen,
            plan.tier.stepwise.alignBytes);
        Check(sp.stepNum == 6, "⑰ 步数 = 6（MAX_STEP_NUM=5 + RS 多拆一步）");
        const u64 goldSlow[] = {154624, 1700864, 2429440, 3470336, 2380288, 2601984};
        const u64 goldFast[] = {1546240, 2429440, 3470336, 4982272, 10202624, 2380288};
        bool slowEq = true, fastEq = true;
        for (u32 i = 0; i < sp.stepNum && i < 6; ++i) {
            slowEq = slowEq && sp.slowSlices[i] == goldSlow[i];
            fastEq = fastEq && sp.fastSlices[i] == goldFast[i];
        }
        Check(slowEq, "⑰ 慢轴（nhr）片宽序列 == golden 逐片（6 片等比非均匀）");
        Check(fastEq, "⑰ 快轴（mesh）片宽序列 == golden 逐片（6 片等比非均匀）");
        Check(sp.fastSlices[0] == 10ULL * sp.slowSlices[0],
              "⑰ 步 0 片宽比 mesh/nhr = 恰 10:1（golden 锚）");
        // 片宽非均匀证据：步 0 与步 1 快轴片宽不等（若误用均匀 stepWidth 必 FAIL）
        Check(sp.fastSlices[0] != sp.fastSlices[1],
              "⑰ 片宽非均匀（步 0 ≠ 步 1——均匀化病态可判）");
        // B 段序列展开：12 段（6 步 × 2 域），全段 T/T、片基址独立推进
        std::vector<ActionSpec> expanded;
        std::string xerr;
        Check(ExpandTieredSegments(plan.segments, plan.tier, 12ULL * 1024 * 1024, expanded, xerr),
              "⑰ B expand ok: " + xerr);
        Check(expanded.size() == 12, "⑰ B 展开 12 段（6 步 × 两域交替）");
        if (expanded.size() == 12) {
            bool alt = true;      // 排列 [mesh(s), nhr(s)] × 6
            bool syncTT = true;   // 全段 T/T（步步隔断）
            bool widthOk = true;  // 逐段片宽 == golden 对应轴片
            bool baseOk = true;   // 片基址 = 同轴前缀和
            u64 fastBase = 0, slowBase = 0;
            for (u32 s = 0; s < 6; ++s) {
                const ActionSpec& fs = expanded[2 * s];
                const ActionSpec& ss = expanded[2 * s + 1];
                alt = alt && fs.pattern == CommPattern::MESH && ss.pattern == CommPattern::NHR;
                syncTT = syncTT && fs.sync.preBarrier && fs.sync.postBarrier &&
                         ss.sync.preBarrier && ss.sync.postBarrier;
                widthOk = widthOk && fs.layout.sliceSize == goldFast[s] &&
                          ss.layout.sliceSize == goldSlow[s];
                baseOk = baseOk && fs.layout.inBaseOff == fastBase && ss.layout.inBaseOff == slowBase;
                fastBase += goldFast[s];
                slowBase += goldSlow[s];
            }
            Check(alt, "⑰ 排列 = [mesh(s0), nhr(s0), mesh(s1), …] 交替");
            Check(syncTT, "⑰ 全段 sync T/T（步步隔断——现有串行消费方零改动可跑）");
            Check(widthOk, "⑰ 逐段片宽 == golden（展开器与 CalcStepwiseSlices 同源）");
            Check(baseOk, "⑰ 片基址 = 同轴片宽前缀和（两轴独立推进）");
        }
    }

    printf("== ⑱ R9 B 步进第二组值：16P PCIeMix（8 卡档 29，步数分支真数锚） ==\n");
    {
        // ★终审 MAJOR-1 修复判据（主会话授权第二组值断言，2026-10-07）：
        //   golden 32P 主场景（4 < ratio 4.286）不进步数分支恒 6 步——单场景验证
        //   有盲区。16P 档（慢域 2 成员、带宽 29）ratio=50/29≈1.724 < 2 → **进步数
        //   分支**，且老式真数 (xRankSize − bandwidthRatio)=0.276 与错式 (xRankSize−1)
        //   =1 的 log 取值不同——步数 5（老/修复后）vs 3（修复前错式）可判。
        //   期望值 = 审查方老代码逐行转录独立手算（/tmp/r9_old_vs_r9.py 老侧实测，
        //   非生成方数字）：
        //     step = 5；nhr（慢）= [199168, 2602496, 1361408, 3464192, 2009088]
        //            mesh（快）= [2403328, 4486656, 2348032, 5972992, 3464192]
        //   （带宽 29 = BW_OMNI_PCIE_EIGHT_RS_CLOS，ziliao executor :349-350 的
        //    rankSizeLevel1==RANK_SIZE_LEVEL_2 档——MAJOR-2 分档判据同场景锚定）
        const StepwiseSlicePlan sp2 = CalcStepwiseSlices(
            9ULL * 1024 * 1024, 2, 8,
            50U, 1U,    // fast = BW_OMNI_DEFAULT
            29U, 1U,    // slow = 8 卡档 29（eqBw = 29/(2-1) = 29/1）
            512U);
        Check(sp2.stepNum == 5, "⑱ 16P 档步数 = 5（步数分支真数锚——MAJOR-1 错式必得 3）");
        const u64 gold2Slow[] = {199168, 2602496, 1361408, 3464192, 2009088};
        const u64 gold2Fast[] = {2403328, 4486656, 2348032, 5972992, 3464192};
        bool slow2Eq = true, fast2Eq = true;
        for (u32 i = 0; i < sp2.stepNum && i < 5; ++i) {
            slow2Eq = slow2Eq && sp2.slowSlices[i] == gold2Slow[i];
            fast2Eq = fast2Eq && sp2.fastSlices[i] == gold2Fast[i];
        }
        Check(slow2Eq, "⑱ 16P 慢轴（nhr）片宽序列 == 老代码独立手算（5 片）");
        Check(fast2Eq, "⑱ 16P 快轴（mesh）片宽序列 == 老代码独立手算（5 片）");
        // 16P 展开：5 步 × 2 域 = 10 段（骨架 tier 由配方挂 29/1——⑮ ②已锚）
        OpParam op = MakeAgOp(10, 64 * 1024, CommEngine::COMM_ENGINE_CCU);
        op.opType = static_cast<int>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER);
        TopoInfoWithNetLayerDetails topo = MakeTopo(10);
        topo.topoLevelNums = 2;
        std::vector<std::vector<u32>> hier{
            {0, 1, 2, 3, 4, 5, 6, 7}, {0, 8}};
        Plan plan; std::string err;
        Check(L1Adapter::Compile(op, topo, hier, "parallel[mesh,nhr]", plan, err) ==
                  HCCL_SUCCESS, "⑱ 16P compile ok: " + err);
        Check(plan.tier.hasTier == true && plan.tier.stepwise.bwSlowNum == 29 &&
                  plan.tier.stepwise.bwSlowDen == 1,
              "⑱ 16P tier 慢轴 = 29/1（8 卡档分档——MAJOR-2：硬编码 35 必 FAIL）");
        std::vector<ActionSpec> expanded16p;
        std::string xerr;
        Check(ExpandTieredSegments(plan.segments, plan.tier, 9ULL * 1024 * 1024, expanded16p, xerr),
              "⑱ 16P B expand ok: " + xerr);
        Check(expanded16p.size() == 10, "⑱ 16P B 展开 10 段（5 步 × 两域交替）");
        if (expanded16p.size() == 10) {
            bool w16 = true;
            for (u32 s = 0; s < 5; ++s) {
                w16 = w16 && expanded16p[2 * s].layout.sliceSize == gold2Fast[s] &&
                      expanded16p[2 * s + 1].layout.sliceSize == gold2Slow[s];
            }
            Check(w16, "⑱ 16P 逐段片宽 == 老代码独立手算（修复后展开器全档位忠实）");
        }
    }

    printf("\n%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
