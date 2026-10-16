#!/usr/bin/env bash
# R6/R6.5 · AICPU 消费方 + exec 分叉 + CCU 覆写 golden 测试入口
# 用法：bash unified_exec/test/run.sh
#   目标 1：AICPU 消费方本体（blob 解读 + exec 现值实例化 + 逐段驱动）63 断言
#   目标 2：exec 分叉链路 gate（判序守门 + B0 双引擎解锁翻转——R6.5 拆闸后
#           ×CCU 放行）28 断言
#   目标 3：R6.5 B-CCU② CCU 写入方覆写 golden（三坑 + 2GiB 边界 + RS 帧拒收）36 断言
set -euo pipefail
cd "$(dirname "$0")"

UNIFIED=$(cd ../../unified && pwd)
EXEC=$(cd .. && pwd)

g++ -std=c++14 -Werror -Wall \
    -I "$UNIFIED/test/shim" -I "$UNIFIED" -I "$UNIFIED/inc" -I "$UNIFIED/l2" \
    -I "$UNIFIED/l3" -I "$UNIFIED/l4" -I "$UNIFIED/l5" -I "$EXEC" \
    poc_unified_exec_test.cc -o poc_unified_exec_test

./poc_unified_exec_test

# exec 分叉 gate（判序：unified 分叉在 GetAlgExec 之前；B0 双引擎解锁翻转）
g++ -std=c++14 -Werror -Wall \
    -I "$UNIFIED/test/shim" -I "$UNIFIED" -I "$UNIFIED/inc" -I "$UNIFIED/l2" \
    -I "$UNIFIED/l3" -I "$UNIFIED/l4" -I "$UNIFIED/l5" -I "$UNIFIED/exec" -I "$EXEC" \
    poc_unified_fork_gate_test.cc -o poc_unified_fork_gate_test

./poc_unified_fork_gate_test

# R6.5 B-CCU②：CCU 写入方数据槽覆写 golden（host 镜像公式 vs v2 手算期望）
g++ -std=c++14 -Werror -Wall \
    -I "$UNIFIED/test/shim" -I "$UNIFIED" -I "$UNIFIED/inc" -I "$UNIFIED/l2" \
    -I "$UNIFIED/l3" -I "$UNIFIED/l4" -I "$UNIFIED/l5" -I "$EXEC" \
    poc_unified_ccu_overwrite_test.cc -o poc_unified_ccu_overwrite_test

./poc_unified_ccu_overwrite_test

# R14 · WP-3：A 形态并发编排器单测（T1 批划分/T2 域映射/T3 降级/T4 组1 时序
# golden/T5 等价对拍/T6 负例）——宏关形态编译（UseUnifiedConcurrencyPath 编译期
# false，T5 对拍走"消费方本体 vs 手工重放"两路；env 探针下方单独跑宏开形态）
g++ -std=c++14 -Werror -Wall \
    -I "$UNIFIED/test/shim" -I "$UNIFIED" -I "$UNIFIED/inc" -I "$UNIFIED/l2" \
    -I "$UNIFIED/l3" -I "$UNIFIED/l4" -I "$UNIFIED/l5" -I "$EXEC" \
    poc_unified_orch_test.cc -o poc_unified_orch_test

./poc_unified_orch_test

# R14 · env 三态探针（受控 env 各起独立进程——static bool envOn 首调定格，
# 与 unified/test/run.sh 路由探针同机制）：宏开+env 开 → 编排器路径（blob 单域
# 段批批单段 = 串行等价）；宏开+env 关 → 串行 for；宏关 → 恒串行（env 开也无效）
cat > /tmp/orch_env_probe.cc <<'PROBE'
#include "aicpu_consumer.h"
#include "action_collect_all.h"
#include "action_spec.h"
#include "backend_registry.h"
#include "l1_adapter.h"
#include "plan_serializer.h"
#include "backend_aicpu_collect_all.h"
#include "backend_aicpu_reduce_slice.h"
#include "unified_route.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
using namespace mc2_ops_hccl::refactor;
static int g_pass = 0, g_fail = 0;
static void Check(bool cond, const char* what)
{
    if (cond) { ++g_pass; printf("  [PASS] %s\n", what); }
    else      { ++g_fail; printf("  [FAIL] %s\n", what); }
}
extern "C" {
int32_t HcommLocalCopyOnThread(ThreadHandle, void*, const void*, uint64_t) { return 0; }
int32_t HcommReadOnThread(ThreadHandle, ChannelHandle, void*, const void*, uint64_t) { return 0; }
int32_t HcommWriteOnThread(ThreadHandle, ChannelHandle, void*, const void*, uint64_t) { return 0; }
int32_t HcommLocalReduceOnThread(ThreadHandle, void*, const void*, uint64_t, HcommDataType, HcommReduceOp) { return 0; }
int32_t HcommThreadNotifyRecordOnThread(ThreadHandle, ThreadHandle, uint32_t) { return 0; }
int32_t HcommThreadNotifyWaitOnThread(ThreadHandle, uint32_t, uint32_t) { return 0; }
int32_t HcommChannelNotifyRecordOnThread(ThreadHandle, ChannelHandle, uint32_t) { return 0; }
int32_t HcommChannelNotifyWaitOnThread(ThreadHandle, ChannelHandle, uint32_t, uint32_t) { return 0; }
int32_t HcommWriteReduceOnThread(ThreadHandle, ChannelHandle, void*, const void*, uint64_t, HcommDataType, HcommReduceOp) { return 0; }
int32_t HcommReadReduceOnThread(ThreadHandle, ChannelHandle, void*, const void*, uint64_t, HcommDataType, HcommReduceOp) { return 0; }
}
int main()
{
    const bool envOn = (std::getenv("MC2_USE_UNIFIED_CONCURRENCY") != nullptr);
    printf("route concurrency probe env=%s: path=%d\n", envOn ? "on" : "off",
        UseUnifiedConcurrencyPath() ? 1 : 0);
#if defined(MC2_UNIFIED_CONCURRENCY_RESOURCE)
    Check(UseUnifiedConcurrencyPath() == envOn, "macro-on: path follows env (static first-call latch)");
#else
    Check(UseUnifiedConcurrencyPath() == false, "macro-off: path false (compile gate, env ignored)");
#endif
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
    Check(L1Adapter::Compile(op, topo, {{0U, 1U, 2U, 3U}}, "sole[mesh]", plan, perr) == HCCL_SUCCESS,
          "compile RS sole[mesh]");
    std::vector<u64> blob;
    Check(SerializePlanV3(plan, blob, perr), "serialize");
    UnifiedExecMsg msg;
    msg.count = 512; msg.inputAddr = 0x100000; msg.outputAddr = 0x200000;
    msg.dataType = HCCL_DATA_TYPE_FP16;
    msg.cclBuff.addr = reinterpret_cast<void*>(0xC00000ULL); msg.cclBuff.size = 4ULL*1024*1024;
    HcommVerbRes res;
    res.threads = {0x1000, 0x1001, 0x1002, 0x1003};
    for (u32 peer = 1; peer <= 3; ++peer) {
        ChannelInfo ch; ch.remoteRank = peer; ch.handle = 0x20000 + peer;
        res.channels[peer] = {ch};
    }
    const UnifiedExecResult r = ConsumeUnifiedPlanAicpu(
        blob, msg, 0, {0U, 1U, 2U, 3U},
        static_cast<u32>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER), res);
    Check(r.ret == HCCL_SUCCESS, "consume ok (both paths)");
    Check(r.segmentsRun == 1, "segmentsRun = 1");
    Check(r.perSegTrace.size() == 1 && r.perSegTrace[0].find("write(") != std::string::npos,
          "perSegTrace[0] contains write (RS write-side verbs)");
    printf("%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
PROBE

g++ -std=c++14 -Werror -Wall -DMC2_UNIFIED_CONCURRENCY_RESOURCE \
    -I "$UNIFIED/test/shim" -I "$UNIFIED" -I "$UNIFIED/inc" -I "$UNIFIED/l2" \
    -I "$UNIFIED/l3" -I "$UNIFIED/l4" -I "$UNIFIED/l5" -I "$EXEC" \
    /tmp/orch_env_probe.cc -o /tmp/orch_env_probe_on
echo "== R14 并发编排 env 探针（宏开 + env 开 → 编排器路径）=="
MC2_USE_UNIFIED_CONCURRENCY=1 /tmp/orch_env_probe_on
echo "== R14 并发编排 env 探针（宏开 + env 关 → 串行 for 现状）=="
unset MC2_USE_UNIFIED_CONCURRENCY && /tmp/orch_env_probe_on

g++ -std=c++14 -Werror -Wall \
    -I "$UNIFIED/test/shim" -I "$UNIFIED" -I "$UNIFIED/inc" -I "$UNIFIED/l2" \
    -I "$UNIFIED/l3" -I "$UNIFIED/l4" -I "$UNIFIED/l5" -I "$EXEC" \
    /tmp/orch_env_probe.cc -o /tmp/orch_env_probe_off
echo "== R14 并发编排 env 探针（宏关 → 恒串行，env 开也无效）=="
MC2_USE_UNIFIED_CONCURRENCY=1 /tmp/orch_env_probe_off

# R15 · WT-1：三件交付单测（① tier 槽位承载 + ② NHR AICPU 执行面 + ③ Σ 申报）
#   宏开形态（TIER + NHR_AICPU）编译，env 由各探针段独立进程控制：
#   - env 全开（TIER=1 NHR_AICPU=1）：tier 分叉 + NHR Emit/sink 执行面 + Σ 覆写
#   - env 全关：骨架直驱 + NHR 段落 mesh 分支（现状行为）+ max 申报回归
#   宏关形态（不加宏）在下方 env 探针补齐三态。
g++ -std=c++14 -Werror -Wall \
    -DMC2_UNIFIED_TIER_RESOURCE -DMC2_UNIFIED_NHR_AICPU_RESOURCE \
    -I "$UNIFIED/test/shim" -I "$UNIFIED" -I "$UNIFIED/inc" -I "$UNIFIED/l2" \
    -I "$UNIFIED/l3" -I "$UNIFIED/l4" -I "$UNIFIED/l5" -I "$EXEC" \
    poc_unified_tier_payload_test.cc -o poc_unified_tier_payload_test

# ★R18 全删轮（2026-10-14 用户裁定）：NHR_RUN 宏动词路径整体删除，展开形态
#（SLICE_REDUCE/CH_SYNC 逐动词）转正唯一——expand 开关退役（宏/env 两级），
# 三遍跑法退两遍（宏形态遍与展开遍合一；MC2_UNIFIED_NHR_EXPAND_RESOURCE 宏
# 与 MC2_USE_UNIFIED_NHR_EXPAND env 不再存在，UseUnifiedNhrAicpuPath 仍承载
# NHR AICPU 执行面灰度）。
echo "== R15/R18 单测（宏开 + env 开：tier 承载 + NHR 展开执行面 + Σ 申报）=="
MC2_USE_UNIFIED_TIER=1 MC2_USE_UNIFIED_NHR_AICPU=1 ./poc_unified_tier_payload_test

echo "== R15 三件交付单测（宏开 + env 关：骨架直驱/现状回归/max 申报）=="
unset MC2_USE_UNIFIED_TIER MC2_USE_UNIFIED_NHR_AICPU
./poc_unified_tier_payload_test

# R15 · env 三态探针（宏关形态）：宏关时 UseUnifiedTierPath/UseUnifiedNhrAicpuPath
# 恒 false（编译期闸门）——单进程双判（与 R14 探针同机制，宏关补齐三态）
cat > /tmp/r15_env_probe.cc <<'PROBE'
#include "unified_route.h"
#include <cstdio>
#include <cstdlib>
using namespace mc2_ops_hccl::refactor;
int main()
{
    printf("tier gate (macro-off) = %d, nhr gate (macro-off) = %d\n",
        UseUnifiedTierPath(6) ? 1 : 0, UseUnifiedNhrAicpuPath() ? 1 : 0);
    return (UseUnifiedTierPath(6) || UseUnifiedNhrAicpuPath()) ? 1 : 0;
}
PROBE
g++ -std=c++14 -Werror -Wall \
    -I "$UNIFIED/test/shim" -I "$UNIFIED" -I "$UNIFIED/inc" -I "$UNIFIED/l2" \
    -I "$UNIFIED/l3" -I "$UNIFIED/l4" -I "$UNIFIED/l5" -I "$EXEC" \
    /tmp/r15_env_probe.cc -o /tmp/r15_env_probe_off
echo "== R15 env 探针（宏关 → 恒关，env 开也无效）=="
MC2_USE_UNIFIED_TIER=1 MC2_USE_UNIFIED_NHR_AICPU=1 /tmp/r15_env_probe_off && echo "  [PASS] 宏关双 gate 恒 false（编译期闸门）"
