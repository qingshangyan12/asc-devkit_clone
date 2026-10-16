#!/usr/bin/env bash
# unified PoC · ST agent 测试入口（语义 checker 自检 + 独立验证，独立于生成 agent 的 run.sh）
# 用法：bash test/run_st_checker.sh
# 产出：
#   st_semantic_checker.{h,cc}  —— ST 语义 checker（动词解析/两引擎等价/计划书/数据流边/切轮
#                                   /第四轮 A4：KFC server v3 轮级分发模拟器 SimulateKfcServerV3）
#   st_verification.cc          —— 对生成 agent 逐轮交付的独立验证（期望值全部手算，
#                                  不转抄其自检；第四轮 R4 专项：v3 序列化 ↔ server 模拟器
#                                  ↔ BuildArgs 三方等价 + 防御矩阵）
set -euo pipefail
cd "$(dirname "$0")"

UNIFIED=$(cd .. && pwd)

# 目标 1：checker 自检（checker 也要被测试）
g++ -std=c++14 -Werror -Wall \
    -I shim -I "$UNIFIED" -I "$UNIFIED/inc" -I "$UNIFIED/l2" \
    -I "$UNIFIED/l3" -I "$UNIFIED/l4" -I "$UNIFIED/l5" \
    st_semantic_checker.cc st_semantic_checker_main.cc -o st_semantic_checker_test
./st_semantic_checker_test

echo

# 目标 2：独立验证（对生成 agent 交付的 5 个 WP 全量断言，期望值来自独立手算基线）
#   R10（F1 补齐轮）新增：-I exec + -I ../unified_exec——消费方本体（aicpu_consumer.h）
#   与真判定入口（unified_aicpu_exec.h）在 unified_exec/、unified/exec/ 两个真源码目录，
#   R10 ②③④⑤ 直测真实现（分叉产出==消费输入的端到端）。
UNIFIED_EXEC=$(cd "$UNIFIED/../unified_exec" && pwd)
g++ -std=c++14 -Werror -Wall \
    -I shim -I "$UNIFIED" -I "$UNIFIED/inc" -I "$UNIFIED/l2" \
    -I "$UNIFIED/l3" -I "$UNIFIED/l4" -I "$UNIFIED/l5" -I "$UNIFIED/exec" \
    -I "$UNIFIED_EXEC" \
    st_semantic_checker.cc st_verification.cc -o st_verification_test
./st_verification_test
# 目标 3：第三轮路由专项——以 -DMC2_UNIFIED_RESOURCE 重编路由探针，受控 env 验证真值表
#（static bool envOn 首调定格 → 每种 env 状态独立进程跑一次）
cat > /tmp/route_r3_on.cc <<'PROBE'
#include "action_spec.h"
#include "backend_registry.h"
#include "unified_route.h"
#include <cstdio>
using namespace mc2_ops_hccl::refactor;
static int g_pass = 0, g_fail = 0;
static void Check(bool cond, const char* what)
{
    if (cond) { ++g_pass; printf("  [PASS] %s\n", what); }
    else      { ++g_fail; printf("  [FAIL] %s\n", what); }
}
int main()
{
    // env 状态由外层脚本通过环境传入（本进程首调即定格）
    const bool ag = UseUnifiedPath(static_cast<int>(HcclCMDType::HCCL_CMD_ALLGATHER));
    const bool rs = UseUnifiedPath(static_cast<int>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER));
    const bool ar = UseUnifiedPath(static_cast<int>(HcclCMDType::HCCL_CMD_ALLREDUCE));
    const bool a2a = UseUnifiedPath(static_cast<int>(HcclCMDType::HCCL_CMD_ALLTOALL));
    const char* mode = "env-off";
    if (std::getenv("MC2_USE_UNIFIED_RES") != nullptr) { mode = "env-on"; }
    printf("route probe mode=%s: AG=%d RS=%d AR=%d A2A=%d\n", mode, ag, rs, ar, a2a);
    if (std::getenv("MC2_USE_UNIFIED_RES") != nullptr) {
        Check(ag == true,  "env-on:  AG routed");
        // R7 · WP-2 翻转断言：RS 路由行激活——改回注释态（表内无 RS）本断言必 FAIL
        Check(rs == true,  "env-on:  RS routed (R7 WP-2 activated)");
        Check(ar == false, "env-on:  AR not routed");
        Check(a2a == false, "env-on:  A2A not routed");
    } else {
        Check(ag == false, "env-off: AG not routed");
        Check(rs == false && ar == false && a2a == false, "env-off: nothing routed");
    }
    printf("%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
PROBE
g++ -std=c++14 -Werror -Wall -DMC2_UNIFIED_RESOURCE \
    -I shim -I "$UNIFIED" -I "$UNIFIED/inc" \
    /tmp/route_r3_on.cc -o /tmp/route_r3_on
echo "== 路由专项（编译期开）env 未设 =="
unset MC2_USE_UNIFIED_RES && /tmp/route_r3_on
echo "== 路由专项（编译期开）env 已设 =="
export MC2_USE_UNIFIED_RES=1 && /tmp/route_r3_on
unset MC2_USE_UNIFIED_RES

# 目标 4：第五轮 S1 探针——真 devkit 协议头（kfc_server_protocol.h + kfc_server_message.h）
# 独立进程实测：UNIFIED_PLAN role 枚举值、KFC_UNIFIED_ACTION_* 常量、v2 algName→role
# 映射零误命中、槽位镜像字面量（devkit 头 include 链干净，可脱离 CANN 环境编译）。
cat > /tmp/probe_kfc_v3_proto.cc <<'PROBE'
#include "kfc_server_protocol.h"
#include "kfc_server_message.h"
#include <cstdio>
using namespace mc2_ops_hccl;
using namespace HcclKfcProtocol;
static int g_pass = 0, g_fail = 0;
static void Check(bool cond, const char* what)
{
    if (cond) { ++g_pass; printf("  [PASS] %s\n", what); }
    else      { ++g_fail; printf("  [FAIL] %s\n", what); }
}
int main()
{
    printf("== S1 探针：真 devkit 头的 v3 协议常量（执行级） ==\n");
    // R6 版本双常量分工：协议族 v3=3（分发机制）≠ blob 格式 v4（R6 头部 4→7 槽随升）。
    //   头部三槽（dataType/outputType/reduceType）与 RS 槽位 [1..15] 一并探活。
    Check(KFC_SERVER_PROTOCOL_VERSION_V3 == 3U, "protocol version v3 = 3");
    Check(KFC_UNIFIED_BLOB_VERSION == 4ULL, "blob format version = 4 (R6 header 4->7 slots)");
    Check(KFC_UNIFIED_BLOB_HEADER_SLOTS == 7U &&
              KFC_UNIFIED_BLOB_HEADER_DATA_TYPE == 4U && KFC_UNIFIED_BLOB_HEADER_OUTPUT_TYPE == 5U &&
              KFC_UNIFIED_BLOB_HEADER_REDUCE_TYPE == 6U,
          "blob header: 7 slots, ReduceSpec at [4..6] (R6 ruling-1)");
    Check(KfcUnifiedBlobTotalSlots(1U) == 39U && KfcUnifiedBlobTotalSlots(63U) == 2023U,
          "KfcUnifiedBlobTotalSlots: 7 + segs*32 (39 / 2023 literals)");
    Check(static_cast<int>(KfcServerRole::UNIFIED_PLAN) == 11, "KfcServerRole::UNIFIED_PLAN = 11 (PR rebase: upstream took 9/10, shifted)");
    Check(static_cast<int>(KfcServerRole::ALL_GATHER_MESH) == 1 &&
              static_cast<int>(KfcServerRole::REDUCE_SCATTER_SOLE_NHR) == 6,
          "v2 role enum values unchanged (1..8 zone)");
    Check(KFC_UNIFIED_ACTION_COLLECT_ALL == 0U && KFC_UNIFIED_ACTION_REDUCE_SLICE == 1U &&
              KFC_UNIFIED_ACTION_EXCHANGE_ALL == 2U && KFC_UNIFIED_ACTION_LOCAL_COPY == 3U &&
              KFC_UNIFIED_ACTION_LOCAL_REDUCE == 4U,
          "devkit KFC_UNIFIED_ACTION_* = {0,1,2,3,4}");
    Check(KFC_UNIFIED_MAX_SEGMENTS == 63U && KFC_UNIFIED_PARAM_NUM_PER_RECORD == 32U,
          "segment cap 63 / 32 slots per record");
    Check(KFC_UNIFIED_REDUCE_SLICE_PARAM_NUM == 16U,
          "R6 RS param num = 16 (fits 32-slot record; slot-cap warning cleared)");
    const char* v2AlgNames[] = {KFC_CONCURRENT_ALL_GATHER_ALG_NAME,
                                KFC_PARALLEL_ALL_GATHER_ALG_NAME,
                                KFC_CONCURRENT_ALL_TO_ALL_ALG_NAME,
                                KFC_A2A_MULTI_JETTY_ALG_NAME,
                                KFC_CONCURRENT_REDUCE_SCATTER_ALG_NAME,
                                KFC_REDUCE_SCATTER_PEER_ONLY_ALG_NAME,
                                KFC_RS_SOLE_NHR_ALG_NAME,
                                "CcuSchedAllGatherMesh1D"};
    int unifiedHits = 0;
    for (const char* n : v2AlgNames) {
        for (uint32_t m = 0; m < 2; ++m) {
            if (GetKfcServerRole(n, m) == KfcServerRole::UNIFIED_PLAN) ++unifiedHits;
        }
    }
    Check(unifiedHits == 0, "v2 algName->role never yields UNIFIED_PLAN (8 names x 2 missions)");
    Check(GetKfcServerRole(nullptr, 0) == KfcServerRole::DEFAULT, "nullptr algName -> DEFAULT (v2 baseline)");
    Check(KFC_UNIFIED_COLLECT_ALL_INPUT == 1 && KFC_UNIFIED_COLLECT_ALL_OUTPUT == 2 &&
          KFC_UNIFIED_COLLECT_ALL_OUTPUT_OFFSET == 3 && KFC_UNIFIED_COLLECT_ALL_SLICE_SIZE == 4 &&
          KFC_UNIFIED_COLLECT_ALL_GO_SIZE_0 == 5 && KFC_UNIFIED_COLLECT_ALL_REPEAT_NUM_INV == 10 &&
          KFC_UNIFIED_COLLECT_ALL_LAST_SLICE_SIZE == 13 && KFC_UNIFIED_COLLECT_ALL_INPUT_OUTPUT_EQUAL == 14,
          "KfcUnifiedPlanParamIndex slots {1,2,3,4,5,10,13,14} match ST literals");
    // v2/v3 同槽布局守门（协议双侧 static_assert 在 devkit 头内已锁，此处执行级复证；
    // 跨 enum 比较须先转 uint32_t——devkit 两个布局是不同 enum 类型）
    Check(static_cast<uint32_t>(KFC_UNIFIED_COLLECT_ALL_INPUT) == KFC_CONCURRENT_AG_MESH_INPUT &&
          static_cast<uint32_t>(KFC_UNIFIED_COLLECT_ALL_OUTPUT_OFFSET) == KFC_CONCURRENT_AG_MESH_OUTPUT_OFFSET &&
          static_cast<uint32_t>(KFC_UNIFIED_COLLECT_ALL_SLICE_SIZE) == KFC_CONCURRENT_AG_MESH_SLICE_SIZE,
          "v3 slots 1..14 == v2 AG Mesh layout (reading-order reuse holds)");
    // R6：v3 RS 槽 1..15 == v2 并发 RS Mesh 槽 1..15（并发布局；server v3 RS 分支的
    // 读取序与 v2 REDUCE_SCATTER_MESH 分支一致——探针执行级复证，协议头内另有
    // 逐槽 static_assert）
    Check(static_cast<uint32_t>(KFC_UNIFIED_REDUCE_SLICE_INPUT) == KFC_CONCURRENT_RS_MESH_INPUT &&
          static_cast<uint32_t>(KFC_UNIFIED_REDUCE_SLICE_SCRATCH) == KFC_CONCURRENT_RS_MESH_SCRATCH &&
          static_cast<uint32_t>(KFC_UNIFIED_REDUCE_SLICE_CHUNK_SIZE) == KFC_CONCURRENT_RS_MESH_CHUNK_SIZE &&
          static_cast<uint32_t>(KFC_UNIFIED_REDUCE_SLICE_TAIL_GO_RESIDUAL) == KFC_CONCURRENT_RS_MESH_TAIL_GO_RESIDUAL,
          "v3 RS slots 1..15 == v2 concurrent RS Mesh layout (R6, reading-order reuse)");
    printf("%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
PROBE
HCCL_ROOT=$(cd ../../../../.. && pwd)   # impl/adv_api/detail/hccl
g++ -std=c++14 -Werror -Wall \
    -I "$HCCL_ROOT/cc/src/common" -I "$HCCL_ROOT/common" \
    /tmp/probe_kfc_v3_proto.cc -o /tmp/probe_kfc_v3_proto
/tmp/probe_kfc_v3_proto
