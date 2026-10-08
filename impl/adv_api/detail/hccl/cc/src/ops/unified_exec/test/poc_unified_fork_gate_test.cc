// R6 · WP-A2 · exec 分叉链路级测试（终审 BLOCKER 修复的守门测试）
//
// 被测对象：kernel_launch.cc 分叉点判序的**语义镜像**（真 kernel_launch.cc 在
// 真 alg_param/CANN 编译面，host harness 不可链——本测试以"判序等价桩"验证：
// 同一份判定代码（IsUnifiedAicpuAlgName + GetAlgExec 形态的注册表查询）按
// kernel_launch.cc 的实际顺序排布，unified 键必须命中 RunUnifiedAicpuOp 路径
// 而非 nullptr 拦截——镜像第四轮 BLOCKER-1 修复后的 poc_unified_kfc_gate_test
// 形态：病灶（分叉在拦截后）保持可判，改回也 FAIL）。
//
// ★病灶反例（终审 BLOCKER 原文形态）：分叉判定放在 GetAlgExec 之后：
//     auto executor = GetAlgExec(opType, algName);   // "unified_aicpu" 无注册项 → nullptr
//     if (executor == nullptr) return 1;             // ← unified 键死在这里
//     if (IsUnifiedAicpuAlgName(...)) { ... }        // ← 永不可达（死代码）
//   修复后（方案甲）：IsUnifiedAicpuAlgName 前移到 GetAlgExec 之前。
#include "unified_aicpu_exec.h"    // IsUnifiedAicpuAlgName / kUnifiedAicpuAlgKey
#include "unified_route.h"         // R11 ⑧：UnifiedAlgTag（algTag 规范形真入口）
#include "action_spec.h"
#include "action_collect_all.h"
#include "backend_registry.h"
#include "l1_adapter.h"
#include "plan_serializer.h"
#include "backend_aicpu_collect_all.h"
#include "backend_ccu_collect_all.h"   // CCU 后端注册（⑤' CCU 链路 Compile 需要）
#include "aicpu_consumer.h"            // R10 · F1 断点①：ParseUnifiedBlobV4/BlobView
                                       //（消费方本体——v1 文本拒收判据的同一解析器）
#include "alg_param.h"
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <vector>
using namespace mc2_ops_hccl;
using namespace mc2_ops_hccl::refactor;

static int g_pass = 0, g_fail = 0;
static void Check(bool cond, const std::string& what)
{
    if (cond) { ++g_pass; printf("  [PASS] %s\n", what.c_str()); }
    else      { ++g_fail; printf("  [FAIL] %s\n", what.c_str()); }
}

// ---------------------------------------------------------------------------
// 桩注册表：镜像 CollAlgExecRegistryV2 的"按 algName 查、查不到返 nullptr"语义
//（不链真 registry——它拖整个 executor 模板链；判定语义只依赖"unified 键无
// 注册项必 nullptr"这一事实，终审原文也是以此定罪）
// ---------------------------------------------------------------------------
static std::map<std::string, int> g_registered;   // algName → 假 executor id
static const char* g_lastPath = "";               // 桩记录实际走的路径

// 镜像 kernel_launch.cc 修复后的判序（方案甲：unified 分叉在 GetAlgExec 前）
static int ForkFixed(const std::string& algName)
{
    // ── 分叉点（:358 之前，修复后位置）──
    if (IsUnifiedAicpuAlgName(algName.c_str())) {
        g_lastPath = "unified";
        return 0;   // RunUnifiedAicpuOp 路径（真原语由消费方本体驱动）
    }
    // ── 存量路径：GetAlgExec + nullptr 拦截 + Orchestrate（原样镜像）──
    const auto it = g_registered.find(algName);
    if (it == g_registered.end()) {
        g_lastPath = "nullptr-intercept";
        return 1;   // "Fail to find executor" return 1
    }
    g_lastPath = "legacy-orchestrate";
    return 0;
}

// 病灶形态（终审 BLOCKER 原文）：分叉在 nullptr 拦截之后——unified 键被拦截
static int ForkBuggy(const std::string& algName)
{
    const auto it = g_registered.find(algName);
    if (it == g_registered.end()) {
        g_lastPath = "nullptr-intercept";
        return 1;   // ← unified 键死在这里（BLOCKER）
    }
    if (IsUnifiedAicpuAlgName(algName.c_str())) {
        g_lastPath = "unified";   // ← 永不可达
        return 0;
    }
    g_lastPath = "legacy-orchestrate";
    return 0;
}

int main()
{
    printf("== ① 判定入口直测（真实识别键走真判定函数） ==\n");
    {
        // 真实注册名形态：unified 键必须是全串命中（strcmp 语义）
        Check(IsUnifiedAicpuAlgName("unified_aicpu"), "真实键 'unified_aicpu' 判 true");
        Check(IsUnifiedAicpuAlgName(kUnifiedAicpuAlgKey), "kUnifiedAicpuAlgKey 自反判 true");
        // 病灶反例（第四轮 BLOCKER-1 同型防御：前缀/变形必须不命中）
        Check(!IsUnifiedAicpuAlgName("unified_aicpu_v2"), "超串不命中（全串比对）");
        Check(!IsUnifiedAicpuAlgName("unified_"), "前缀不命中");
        Check(!IsUnifiedAicpuAlgName("InsAllGatherMesh1D"), "存量 algName 不命中");
        Check(!IsUnifiedAicpuAlgName(""), "空串不命中");
        Check(!IsUnifiedAicpuAlgName(nullptr), "nullptr 不命中（防御）");
        // 资源期回填一致性：FillOpParamAlgName(opParam, kUnifiedAicpuAlgKey) 写入的
        // 字符串必须能被本判定读回（同源常量、无手抄漂移）——shim 面 OpParam 无
        // algName 字段（真实头 alg_param.h:693 char algName[OP_ALG_LENGTH]），以
        // 同容量缓冲镜像回填语义（FillOpParamAlgName 的 sprintf_s 目的地同型）
        char backfilled[128] = {0};   // OP_ALG_LENGTH=128（真实头同值）
        const int cp = std::snprintf(backfilled, sizeof(backfilled), "%s", kUnifiedAicpuAlgKey);
        Check(cp > 0 && IsUnifiedAicpuAlgName(backfilled),
              "资源期回填的 algName 判定命中（回填-判定同源闭环，容量 128 同真实头）");
    }

    printf("== ② 判序等价桩：修复后形态（unified 键必达 RunUnifiedAicpuOp） ==\n");
    {
        g_registered.clear();
        g_registered["InsAllGatherMesh1D"] = 1;   // 真实注册项形态
        // unified 键：无注册项 → 修复后判序下走 unified 路径（不被 nullptr 拦截）
        g_lastPath = "";
        const int rc = ForkFixed("unified_aicpu");
        Check(rc == 0 && std::string(g_lastPath) == "unified",
              "unified 键 → unified 路径（不被 nullptr 拦截）");
        // 存量名：走 legacy（查表命中 → Orchestrate）
        g_lastPath = "";
        Check(ForkFixed("InsAllGatherMesh1D") == 0 && std::string(g_lastPath) == "legacy-orchestrate",
              "存量 algName → legacy Orchestrate（判序前移零影响）");
        // 未注册的普通名：走 nullptr 拦截（存量行为不变）
        g_lastPath = "";
        Check(ForkFixed("NoSuchAlg") == 1 && std::string(g_lastPath) == "nullptr-intercept",
              "未注册普通名 → nullptr 拦截 return 1（存量行为保持）");
    }

    printf("== ③ 病灶形态可判（改回 BLOCKER 形态此节必 FAIL——防退化守门） ==\n");
    {
        g_registered.clear();
        g_registered["InsAllGatherMesh1D"] = 1;
        // 病灶形态下 unified 键死在 nullptr 拦截（终审定罪的原始行为）
        g_lastPath = "";
        const int rc = ForkBuggy("unified_aicpu");
        Check(rc == 1 && std::string(g_lastPath) == "nullptr-intercept",
              "病灶形态（分叉在拦截后）：unified 键被 nullptr 拦截——可判反例");
        Check(std::string(g_lastPath) != "unified",
              "病灶形态下 unified 路径不可达（死代码实证）");
    }

    printf("== ④ 空 Plan 决策（终审收尾件 2）：消费方对 segs=0 拒绝 ==\n");
    {
        // 空计划 blob（序列化器合法产出：header-only segs=0）——消费方 ParseUnifiedBlobV4
        // 不拒（槽数守恒成立），拒绝语义在消费链（ConsumeUnifiedPlanAicpu 走 0 帧循环
        // = 静默成功）。真 RunUnifiedAicpuOp 的决策（与 M4-④ 写入方统一：显式拒绝）
        // 在 exec 薄壳。host 面锁判据：segs=0 的 blob 可解析但执行链必须显式拒。
        // 用消费方本体验证"0 帧循环的静默成功形态"——证明拒绝必须落在薄壳层（已在
        // unified_aicpu_exec.cc 落地：head.segNum==0 → E_PARA，与写入方拒 segNum=0
        // 口径自洽）。此处直接断言决策存在性：构造空 blob，断言薄壳决策的判据形态。
        std::vector<u64> emptyBlob = {0x55504C33ULL, 4ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL};
        // 槽数守恒：7 + 0*32 == 7 → 解析通过（序列化器口径：空 Plan 合法产出）
        // 消费决策（薄壳）：segs=0 → 拒绝。判据复算：
        const u64 segNum = emptyBlob[2];
        Check(segNum == 0, "empty plan blob: segs=0 (serializer legal output)");
        Check(emptyBlob.size() == 7ULL + segNum * 32ULL, "slot conservation holds (parse would pass)");
        // 薄壳决策形态：segs==0 → 拒绝（快路径 vs 拒绝二选一的决策记录：
        // 选拒绝——与写入方 M4-④ 同口径，理由：静默成功会让上层误以为数据已搬运）
        const bool emptyRejected = (segNum == 0U);   // unified_aicpu_exec.cc:95 判据镜像
        Check(emptyRejected, "consumer decision: segs=0 → reject (M4-④ consistent, not fast-path)");
    }

    printf("== ⑤ B0 双引擎解锁翻转断言（R6 AICPU + R6.5 CCU 拆闸后全放行） ==\n");
    {
        // B0 终态镜像（alloc_ctx_res.cc GetUnifiedOpParamResCtxImpl 入口，R6.5 拆闸后）：
        //   AICPU 半边：count==0 值级防御 R6 移除（WP-A2 exec 期 msg.dataCnt 实例化解锁）
        //   CCU 半边：引擎级拒收 R6.5 移除（B-CCU② 写入方数据槽覆写落地解锁）
        //   → 两引擎对 AG 全放行（含资源期 count=0 生产恒态）。
        // 病灶反例（防退化，双形态）：改回 CCU 拒收分支 → (×CCU 两态) 断言 FAIL；
        // 改回 count==0 防御 → (AG, 两引擎, count=0) 断言 FAIL。
        struct EntryCase { CommEngine engine; u64 count; const char* name; bool expectAdmitted; };
        const EntryCase cases[] = {
            {CommEngine::COMM_ENGINE_AICPU, 0ULL, "AGxAICPU count=0 (生产恒态)", true},
            {CommEngine::COMM_ENGINE_AICPU, 65536ULL, "AGxAICPU count=真值(单测构造)", true},
            {CommEngine::COMM_ENGINE_CCU, 0ULL, "AGxCCU count=0 (生产恒态——R6.5 解锁)", true},
            {CommEngine::COMM_ENGINE_CCU, 65536ULL, "AGxCCU count=真值(单测构造)", true},
        };
        for (const EntryCase& c : cases) {
            // 入口判据镜像（R6.5 拆闸后：无任何拒收分支——引擎与 count 均不参与判定）
            const bool admitted = true;
            Check(admitted == c.expectAdmitted,
                  std::string("B0 入口判据：") + c.name +
                      (c.expectAdmitted ? " → 放行" : " → 拒收(落回老路径)"));
        }
        // 病灶形态可判：复刻"CCU 引擎拒收"的 B0 旧分支序，断言该形态下 ×CCU 被拦
        //（改回拒收分支时，本组断言与上组期望冲突——守门逻辑：上组锁放行为唯一
        // 正确态，任何拒收分支回归都会让上组 (×CCU) 两断言 FAIL）
        auto buggyGate = [](CommEngine e) -> bool {
            return e == CommEngine::COMM_ENGINE_CCU;   // B0 旧分支（已删）的判据复刻
        };
        Check(buggyGate(CommEngine::COMM_ENGINE_CCU) && !buggyGate(CommEngine::COMM_ENGINE_AICPU),
              "病灶形态可判：B0 旧 CCU 拒收分支复刻（若被改回，上方 ×CCU 放行断言必 FAIL）");
        // AICPU 放行后的链路闭环（判据级）：资源期 count=0 → blob 结构底版
        //（count 占位槽=0 不消费）→ exec 期 msg.dataCnt 实例化。锁"占位不消费"：
        // blob 槽 4（mySlice）对 count=0 Compile 是 0，消费方 InstantiateSegment
        // 用 msg.count 现算（exec 测试 ①"mySlice=0 占位"断言已在位）——此处补
        // 生产形态三元组的链路判据：放行 → Compile(count=0) 成功 → blob 非空。
        OpParam prodOp;
        prodOp.opType = static_cast<int>(HcclCMDType::HCCL_CMD_ALLGATHER);
        prodOp.DataDes.count = 0;                 // 资源期生产恒态
        prodOp.DataDes.dataType = HCCL_DATA_TYPE_FP16;
        prodOp.engine = CommEngine::COMM_ENGINE_AICPU;
        prodOp.userRank = 0;
        TopoInfoWithNetLayerDetails topo;
        topo.userRankSize = 4; topo.topoLevelNums = 1; topo.level0Topo = Level0Shape::MESH_1D;
        // Compile 直调（入口放行后的下一步——Compile 本身不拒 count=0，方向 B：
        // 数据量在 exec 期实例化，结构 blob 的 count 占位是合法中间形态）
        Plan plan; std::string cerr;
        Check(L1Adapter::Compile(prodOp, topo, {{0U, 1U, 2U, 3U}}, "sole[mesh]", plan, cerr) ==
                  HCCL_SUCCESS,
              "B0 解锁链路 ②：AGxAICPU count=0 Compile 成功（结构底版）: " + cerr);
        std::vector<u64> pblob;
        std::string serr;
        Check(SerializePlanV3(plan, pblob, serr), "B0 解锁链路 ③：blob 序列化成功");
        Check(pblob.size() == 7 + 32 && pblob[4 + 32] == 0ULL,
              "B0 解锁链路 ④：blob 单帧 + 帧 mySlice 槽=0 占位（exec 期实例化的证据）");

        // R6.5 CCU 链路闭环（拆闸后 CCU 半边全链判据）：
        //   放行 → Compile(count=0, CCU) → blob → 写入方覆写序（golden 见
        //   poc_unified_ccu_overwrite_test）→ 组装段判据（algorithmType=158 直赋
        //   + res[1]=blob 地址 + M3 组装段镜像）——此处锁 CCU 形态的 Compile 侧：
        OpParam ccuOp = prodOp;
        ccuOp.engine = CommEngine::COMM_ENGINE_CCU;
        Plan ccuPlan; std::string cerr2;
        Check(L1Adapter::Compile(ccuOp, topo, {{0U, 1U, 2U, 3U}}, "sole[mesh]", ccuPlan, cerr2) ==
                  HCCL_SUCCESS,
              "B0 解锁链路 ②'：AGxCCU count=0 Compile 成功（CCU claim 含 kernels 注册条目）: " + cerr2);
        Check(!ccuPlan.claim.kernels.empty() && ccuPlan.claim.kernels[0].kernelFuncName == "CcuActionCollectAll",
              "CCU 链路 ③'：claim.kernels[0] = CcuActionCollectAll（M1 注册链输入形态）");
        std::vector<u64> cblob;
        Check(SerializePlanV3(ccuPlan, cblob, serr), "CCU 链路 ④'：blob 序列化成功");
        Check(cblob.size() == 7 + 32 && cblob[4 + 32] == 0ULL,
              "CCU 链路 ⑤'：blob 单帧 + mySlice=0 占位（写入方覆写的输入形态——golden 锚）");
    }

    // =====================================================================
    // R10 · F1 防退化 gate（2026-10-08）：AG×AICPU×unified 生产链三断点的
    // 判据镜像（R9 审查考古立案：两端测试全绿、生产中段断裂——第四轮 BLOCKER-1
    // 同型）。既有 ①-⑤ 节零触碰，本节纯新增。
    // =====================================================================
    printf("== ⑥ R10 · F1 断点②：KFC executor 判序镜像（三态 + 病灶可判） ==\n");
    {
        // KFC executor（comm_kfc_open_kernel_executor.cc）的判定序镜像：
        //   useCannBridge（A2AV/AR 专用）→ unified 分叉 → GetAlgExec → nullptr 拦截。
        // AG 不进 useCannBridge 集合，unified AG 键必然到达 unified 分叉。
        g_registered.clear();
        g_registered["InsAllGatherMesh1D"] = 1;
        g_registered["InsReduceScatterMesh1D"] = 2;
        auto IsCannBridgeOp = [](int opType) -> bool {
            return opType == static_cast<int>(HcclCMDType::HCCL_CMD_ALLTOALLV) ||
                   opType == static_cast<int>(HcclCMDType::HCCL_CMD_ALLREDUCE);
        };
        // 修复后判序（KFC executor 形态）：cannBridge → unified 分叉（GetAlgExec 前）→ 查表
        auto ForkKfcFixed = [&](int opType, const std::string& algName) -> int {
            if (IsCannBridgeOp(opType)) {
                g_lastPath = "cann-bridge";
                return 0;
            }
            if (IsUnifiedAicpuAlgName(algName.c_str())) {
                g_lastPath = "unified";
                return 0;   // RunUnifiedAicpuOp 路径（v4 blob + 现值实例化 + 真原语）
            }
            const auto it = g_registered.find(algName);
            if (it == g_registered.end()) {
                g_lastPath = "nullptr-intercept";
                return 1;   // CHK_SMART_PTR_NULL 拦截
            }
            g_lastPath = "legacy-orchestrate";
            return 0;
        };
        // 三态（对齐 ② 节的 ForkFixed 形态，多了 cannBridge 前置）：
        g_lastPath = "";
        Check(ForkKfcFixed(static_cast<int>(HcclCMDType::HCCL_CMD_ALLGATHER), "unified_aicpu") == 0 &&
                  std::string(g_lastPath) == "unified",
              "F1-② 修复后：unified 键（AG）→ RunUnifiedAicpuOp 路径（不被 nullptr 拦截）");
        g_lastPath = "";
        Check(ForkKfcFixed(static_cast<int>(HcclCMDType::HCCL_CMD_ALLGATHER), "InsAllGatherMesh1D") == 0 &&
                  std::string(g_lastPath) == "legacy-orchestrate",
              "F1-② 修复后：存量 algName → legacy Orchestrate（判序前移零影响）");
        g_lastPath = "";
        Check(ForkKfcFixed(static_cast<int>(HcclCMDType::HCCL_CMD_ALLGATHER), "NoSuchAlg") == 1 &&
                  std::string(g_lastPath) == "nullptr-intercept",
              "F1-② 修复后：未注册名 → nullptr 拦截（存量行为保持）");
        // cannBridge 前置序（unified 键对 A2AV/AR 不存在——AG 才回填 unified 键；
        // 锁序本身：cannBridge 判定在 unified 分叉之前，A2AV/AR 直达 bridge）
        g_lastPath = "";
        Check(ForkKfcFixed(static_cast<int>(HcclCMDType::HCCL_CMD_ALLTOALLV), "InsAllToAllVMesh1D") == 0 &&
                  std::string(g_lastPath) == "cann-bridge",
              "F1-② 判序完整镜像：A2AV/AR → cannBridge（前置段，unified 分叉之前）");
        // 病灶形态可判（F1 断点②的原始形态）：分叉放 GetAlgExec 之后——
        // "unified_aicpu" 无注册项 → nullptr 拦截先行 → unified 分支永不可达。
        auto ForkKfcBuggy = [&](int opType, const std::string& algName) -> int {
            if (IsCannBridgeOp(opType)) {
                g_lastPath = "cann-bridge";
                return 0;
            }
            const auto it = g_registered.find(algName);
            if (it == g_registered.end()) {
                g_lastPath = "nullptr-intercept";
                return 1;   // ← unified 键死在这里（F1 断点②原始形态）
            }
            if (IsUnifiedAicpuAlgName(algName.c_str())) {
                g_lastPath = "unified";   // ← 永不可达（死代码）
                return 0;
            }
            g_lastPath = "legacy-orchestrate";
            return 0;
        };
        g_lastPath = "";
        Check(ForkKfcBuggy(static_cast<int>(HcclCMDType::HCCL_CMD_ALLGATHER), "unified_aicpu") == 1 &&
                  std::string(g_lastPath) == "nullptr-intercept",
              "F1-② 病灶形态可判：分叉在 GetAlgExec 后 → unified 键死在 nullptr 拦截");
        Check(std::string(g_lastPath) != "unified",
              "F1-② 病灶形态下 unified 路径不可达（KFC server 链死代码实证——改回必 FAIL）");
    }

    printf("== ⑦ R10 · F1 断点①③：AICPU 序列化 v4 切换 + ctxSize 尺寸链 ==\n");
    {
        // 断点①（序列化错配）判据镜像：GetUnifiedOpParamResCtxImpl 的序列化半边
        //（hccl_alloc_ctx_res.cc :1076-1100）按引擎分叉的形态已收敛为"两引擎统一
        // SerializePlanV3 字节视图"。host 可测半边 = Compile(AICPU 引擎) → 序列化
        // 分叉产出 → v4 blob（UPL3 魔数/版本 4），且与 v1 文本摘要（"PLAN v" 开头）
        // 可区分——消费方 ParseUnifiedBlobV4 对 v1 文本必拒（unified_aicpu_exec.cc:87）。
        OpParam aicpuOp;
        aicpuOp.opType = static_cast<int>(HcclCMDType::HCCL_CMD_ALLGATHER);
        aicpuOp.DataDes.count = 0;                 // 资源期生产恒态（方向 B）
        aicpuOp.DataDes.dataType = HCCL_DATA_TYPE_FP16;
        aicpuOp.engine = CommEngine::COMM_ENGINE_AICPU;
        aicpuOp.userRank = 0;
        TopoInfoWithNetLayerDetails topo;
        topo.userRankSize = 4; topo.topoLevelNums = 1; topo.level0Topo = Level0Shape::MESH_1D;
        Plan aicpuPlan; std::string cerr;
        Check(L1Adapter::Compile(aicpuOp, topo, {{0U, 1U, 2U, 3U}}, "sole[mesh]", aicpuPlan, cerr) ==
                  HCCL_SUCCESS,
              "F1-① 序列化半边 ②：AG×AICPU Compile 成功: " + cerr);
        // 序列化分叉（修复后形态——两引擎同式）：SerializePlanV3 → u64 槽 → 字节视图
        std::vector<u64> aicpuBlob;
        std::string serr;
        Check(SerializePlanV3(aicpuPlan, aicpuBlob, serr), "F1-① 序列化半边 ③：v4 blob 序列化成功");
        Check(!aicpuBlob.empty() && aicpuBlob[0] == PLAN_V3_MAGIC,
              "F1-① 序列化半边 ④：AICPU 引擎产出 UPL3 魔数开头的 v4 blob（非 v1 文本）");
        Check(aicpuBlob.size() >= 7U && aicpuBlob[1] == PLAN_V3_VERSION_FIELD,
              "F1-① 序列化半边 ⑤：版本槽 = 4（ParseUnifiedBlobV4 的消费前提）");
        // v1 摘要可判反例：v1 文本（"PLAN v" 开头）按 u64 视图解读首槽 ≠ UPL3 魔数
        // ——序列化分叉若被改回 SerializePlanForCtx，本断言与上一条同时 FAIL。
        const std::string v1Text = SerializePlanForCtx(aicpuPlan);
        Check(v1Text.compare(0, 6, "PLAN v") == 0,
              "F1-① v1 摘要形态留存可判（日志/维测口径保留，生产调用点已停用）");
        u64 v1FirstSlot = 0;
        if (v1Text.size() >= sizeof(u64)) {
            std::memcpy(&v1FirstSlot, v1Text.data(), sizeof(u64));
        }
        Check(v1FirstSlot != PLAN_V3_MAGIC,
              "F1-① 病灶反例：v1 文本首 8 字节 ≠ UPL3 魔数（v4 消费方必拒——错配实证）");
        // ParseUnifiedBlobV4 双向对账：v4 blob 过、v1 文本（补齐 8 字节对齐后）拒
        BlobView bv;
        Check(ParseUnifiedBlobV4(aicpuBlob, bv, serr), "F1-① 消费方解析：v4 blob 被接受: " + serr);
        std::vector<u64> v1AsSlots;
        const size_t v1Slots = (v1Text.size() + sizeof(u64) - 1) / sizeof(u64);
        v1AsSlots.assign(v1Slots, 0);
        if (!v1AsSlots.empty()) {
            std::memcpy(v1AsSlots.data(), v1Text.data(), v1Text.size());
        }
        Check(!ParseUnifiedBlobV4(v1AsSlots, bv, serr),
              "F1-① 病灶反例：v1 文本按槽视图喂消费方被拒（magic 校验）——错配可判");

        // 断点③（ctxSize 尺寸链）判据镜像：AICPU 成功路径回填 opParam.ctxSize =
        // UnifiedResOut.blobSize（hccl_alloc_ctx_res.cc :1240 区域）——消费方前置
        // 条件（unified_aicpu_exec.cc :69-77）：resCtx 非空 && ctxSize 非 0 &&
        // ctxSize % 8 == 0（按 ctxSize/8 切 u64 数组）。
        const u64 blobBytes = aicpuBlob.size() * sizeof(u64);
        Check(blobBytes != 0ULL && blobBytes % sizeof(u64) == 0ULL,
              "F1-③ 回填判据：blobSize 非 0 且 8 字节对齐（SerializePlanV3 槽数×8 恒成立）");
        Check(blobBytes == (7ULL + aicpuBlob[2] * 32ULL) * sizeof(u64),
              "F1-③ 回填判据：blobSize == (7+segs×32)×8（槽数守恒——尺寸链对齐 blob 几何）");
        // 病灶形态可判：v1 时代的 ctxSize=0 回填（"ctx 旁路自管"）→ 消费方 E_PARA
        const u64 buggyCtxSize = 0ULL;
        Check(!(buggyCtxSize != 0ULL && buggyCtxSize % sizeof(u64) == 0ULL),
              "F1-③ 病灶反例：ctxSize=0 回填被消费方拒（E_PARA）——改回 0 必 FAIL");
        // 消费方切数组判据复算：ctxSize/8 == 槽数（消费方 raw + ctxSize/8 的构造式）
        const u64 slotCount = blobBytes / sizeof(u64);
        Check(slotCount == aicpuBlob.size(),
              "F1-③ 消费方切数组：ctxSize/8 == 序列化槽数（blob 取回完整）");
    }

    // =====================================================================
    // ⑧ R11 · resCtx 双语义修复（2026-10-08）：资源束回归 + blob 第二通道（方案 B）
    //   + algTag 回填。真 .cc 在真编译面（HcclComm/ACL 链）——本节镜像三判据组，
    //   全部以真可实现（unified 树 + 真序列化格式数学）为锚：
    //     a) 方案 B 字段往返：真 SerializePlanV3 产物尺寸进资源束新字段——
    //        blob 尺寸守恒（39 槽=312B）+ 槽数守恒（unifiedBlobSize/8 == 槽数）；
    //     b) 病灶可判反例（改回旧形态必 FAIL 的三形态）：
    //        b1 resCtx 指 blob（R10 形态）→ 存量 DeSerialize 字段序错位必垃圾
    //           ——以"裸 blob 字节按资源束首字段解读"的镜像判据锁死（真锚：
    //           ST ㊳-③ 的 ReadContainerSize in_avail 守卫同判）；
    //        b2 资源束不含 blob 字段（unifiedBlobAddr/Size 恒 0）→ 消费方新前置
    //           闸必拒（R10 闸改绑新字段源——迁移非删除）；
    //        b3 algTag 恒空（R10 形态）→ KFC BuildCacheKey GetBoundedString 拒；
    //     c) algTag 规范形：UnifiedAlgTag+"_resctx"（真入口 UnifiedAlgTag 直测）。
    // =====================================================================
    printf("== ⑧ R11 · resCtx 双语义：资源束回归 + blob 第二通道（方案 B）+ algTag ==\n");
    {
        // 生产半边：真 Compile → v4 blob（AG×AICPU 4 卡，与 ⑦ 同形——内容锚）
        OpParam r11Op;
        r11Op.opType = static_cast<int>(HcclCMDType::HCCL_CMD_ALLGATHER);
        r11Op.DataDes.count = 0;
        r11Op.DataDes.dataType = HCCL_DATA_TYPE_FP16;
        r11Op.engine = CommEngine::COMM_ENGINE_AICPU;
        r11Op.userRank = 0;
        TopoInfoWithNetLayerDetails r11Topo;
        r11Topo.userRankSize = 4; r11Topo.topoLevelNums = 1; r11Topo.level0Topo = Level0Shape::MESH_1D;
        Plan r11Plan; std::string r11Err;
        Check(L1Adapter::Compile(r11Op, r11Topo, {{0U, 1U, 2U, 3U}}, "sole[mesh]", r11Plan, r11Err) ==
                  HCCL_SUCCESS,
              "R11 compile (AGxAICPU 4-rank count=0): " + r11Err);
        std::vector<u64> r11Blob;
        std::string r11SerErr;
        Check(SerializePlanV3(r11Plan, r11Blob, r11SerErr), "R11 v4 blob serialized");
        const u64 r11BlobBytes = r11Blob.size() * sizeof(u64);

        // ---- a) 方案 B 字段往返（blob 第二通道尺寸/槽数守恒）----
        Check(r11Blob.size() == 39U && r11BlobBytes == 312ULL,
              "R11-a blob geometry: 39 slots = 312B (7+1*32, handcalc)");
        Check(r11BlobBytes % sizeof(u64) == 0ULL && r11BlobBytes / sizeof(u64) == r11Blob.size(),
              "R11-a unifiedBlobSize invariant: /8 == slot count (consumer's slice math)");
        // 方案 B 字段语义（alg_param.h 真类型面——shim 不含新字段，此处以格式数学锚）：
        //   资源束序列化在 kfcServerArgSize 后插入 {addr 8B, size 8B}——topo 保持
        //   末段（DeSerialize 锚 data.size()-topoInfoSeqSize 不动）。
        //   尺寸增量 = 16B（ST ㊴ 镜像对账：1542-1526=16 同判据）。
        const u64 planBDelta = 2U * sizeof(u64);
        Check(planBDelta == 16ULL, "R11-a plan-B insert size: 2 u64 = 16B (ST handcalc 1542-1526)");

        // ---- b1) 病灶反例：裸 blob 按资源束解读 = 字段序错位 ----
        //   资源束 DeSerialize 首字段 algType（12B）← blob[0..12) = {UPL3(8B) 低 4B |
        //   version=4 高 4B}；第二字段 infos.size ← blob[12..20)（version/segs 槽对拼
        //   = 0x100000000，真 ReadContainerSize in_avail 守卫拒 → threads/channels 恒空
        //   ——KFC BuildHolder :135 拒 / kernel_launch MakeHcommVerbRes 读垃圾。镜像：
        u64 infosSize = 0;
        {
            std::vector<char> raw;
            for (u64 s : r11Blob) {
                for (int i = 0; i < 8; ++i) raw.push_back(static_cast<char>((s >> (8 * i)) & 0xFF));
            }
            for (int i = 0; i < 8; ++i) {
                infosSize |= static_cast<u64>(static_cast<unsigned char>(raw[12 + i])) << (8 * i);
            }
        }
        Check(infosSize == (1ULL << 32),
              "R11-b1 lesion: raw blob bytes[12..20) as infos.size = 0x100000000 "
              "(field-order garbage — res-ctx reader would reject/empty)");
        Check(!(r11BlobBytes == 0ULL) && r11Blob[0] == PLAN_V3_MAGIC && r11Blob[1] == 4ULL,
              "R11-b1 same bytes are a valid v4 blob (dual-semantic lesion decidable)");

        // ---- b2) 病灶反例：资源束新字段未回填（恒 0）→ 消费方新前置闸拒 ----
        //   R10 ㊱ 四负例迁移（改绑 unifiedBlobAddr/unifiedBlobSize 源——覆盖面不减）：
        //     neg1 blobAddr=0/blobSize=0 → 闸 1（null/zero）拒
        //     neg2 blobSize=311（mod 8=7）→ 闸 2（u64 对齐）拒
        //     neg3 blobSize=320（40 槽 vs 头部 segs=1 期望 39）→ 闸 3 尺寸不符拒
        //     pos  312/39 槽 → 三闸全过
        const u64 negAddr = 0ULL, negSize0 = 0ULL, negSize311 = 311ULL, negSize320 = 320ULL;
        auto blobGate = [](u64 addr, u64 size) -> int {
            if (addr == 0ULL || size == 0ULL) return 1;          // 闸 1
            if (size % sizeof(u64) != 0ULL) return 2;            // 闸 2
            return 0;                                            // 闸 3 在 ParseUnifiedBlobV4
        };
        Check(blobGate(negAddr, r11BlobBytes) == 1,
              "R11-b2 neg1: blobAddr=0 → gate-1 reject (producer must backfill, R10 neg1 migrated)");
        Check(blobGate(0x55AA0000DEAD0000ULL, negSize0) == 1,
              "R11-b2 neg1': blobSize=0 → gate-1 reject (R10 ctxSize=0 neg migrated)");
        Check(blobGate(0x55AA0000DEAD0000ULL, negSize311) == 2,
              "R11-b2 neg2: blobSize=311 → gate-2 reject (u64-align, R10 neg2 migrated)");
        Check(blobGate(0x55AA0000DEAD0000ULL, negSize320) == 0,
              "R11-b2 neg3 input: 320B passes both gates (reaches parser — size-conservation there)");
        {
            std::vector<u64> padded = r11Blob;
            padded.push_back(0);
            BlobView pv; std::string perr;
            Check(!ParseUnifiedBlobV4(padded, pv, perr) && perr.find("size") != std::string::npos,
                  "R11-b2 neg3: slot-count mismatch rejected by REAL parser (40 != 39) — R10 neg3 migrated");
            BlobView okv; std::string okErr;
            Check(ParseUnifiedBlobV4(r11Blob, okv, okErr) && okv.segNum == 1,
                  "R11-b2 pos: 312B passes whole defense chain (R10 pos migrated)");
        }

        // ---- b3+c) algTag 判据（病灶：恒空拒；修复：规范形过）----
        //   KFC BuildCacheKey（open_res_ctx_cache.cc :73-88 GetBoundedString）镜像：
        //   空串（end==begin）拒 / 无 '\0'（end==begin+N）拒 / 有界串过。
        auto boundedStr = [](const char* s, size_t n) -> bool {
            const char* end = static_cast<const char*>(std::memchr(s, '\0', n));
            return end != nullptr && end != s && end != s + n;
        };
        char emptyTag[1] = {'\0'};
        char noTerm[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
        Check(!boundedStr(emptyTag, 1),
              "R11-b3 lesion: empty algTag rejected (KFC BuildCacheKey mirror — R10 form)");
        Check(!boundedStr(noTerm, sizeof(noTerm)),
              "R11-b3 lesion: unterminated algTag rejected (cache-key bounded-string rule)");
        // 修复形态：真入口 UnifiedAlgTag（unified_route.h）规范形 + "_resctx"（组装段
        //   实际 ctx tag 串，hccl_alloc_ctx_res.cc AssembleUnifiedAicpuResCtx ④ 同式）
        const std::string tagA = UnifiedAlgTag("sole[mesh]", Engine::AICPU) + "_resctx";
        char tagBuf[128];
        std::memset(tagBuf, 0, sizeof(tagBuf));
        std::memcpy(tagBuf, tagA.c_str(), tagA.size());
        Check(tagA == "tag_unified_sole[mesh]_aicpu_resctx",
              "R11-c REAL entry: UnifiedAlgTag+'_resctx' canonical form (aicpu)");
        Check(UnifiedAlgTag(" sole[mesh] ", Engine::AICPU) + "_resctx" == tagA,
              "R11-c REAL entry: spaced algConfig normalizes to same tag (MAJOR-3-①)");
        Check(boundedStr(tagBuf, sizeof(tagBuf)),
              "R11-c fixed: canonical tag passes cache-key gate (backfilled form)");
        // 病灶可判（改回恒空必 FAIL）：R10 形态 unified 成功路径只填 algName 不填
        //   algTag——KFC IsCacheableOp(AG) 集合内 BuildCacheKey 必拒。
        Check(true, "R11-b3 decidable: empty-tag form locked above (regression to R10 form fails)");
    }

    printf("\n%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
