#!/usr/bin/env bash
# unified PoC · L2 Expand 接线自检（host 侧，无 devkit 编译环境依赖）
#
# 用法：bash test/run.sh     （在 unified/ 目录下执行，或任意位置 bash <path>/run.sh）
#
# 说明：
#   unified 树直接 include devkit 真实 alg_param.h（ops/op_common/inc），该头又级联依赖
#   ccu_datatype_v1.h / acl_base_rt.h 等 CANN 环境头，本机无完整 devkit 编译环境时不可用。
#   故自检走 shim：shim/alg_param.h 经 -I 优先命中，转发到 fake_alg_param.h（提供 unified
#   链路消费的最小 devkit 类型面，声明形态与真实头一致：typedef enum / constexpr 表）。
#   在 devkit 树内（mc2_client 目标的 include 路径）编译时真实头先命中，shim 失效、无副作用。
#
#   注：WP5 已完成 C++14 合规（action_spec.h 结构化绑定改迭代器循环），本测试按 devkit
#   目标标准 -std=c++14 -Werror -Wall 编译。
set -euo pipefail
cd "$(dirname "$0")"

UNIFIED=$(cd .. && pwd)

g++ -std=c++14 -Werror -Wall \
    -I shim -I "$UNIFIED" -I "$UNIFIED/inc" -I "$UNIFIED/l2" \
    -I "$UNIFIED/l3" -I "$UNIFIED/l4" -I "$UNIFIED/l5" \
    poc_unified_expand_test.cc -o poc_unified_expand_test

./poc_unified_expand_test

# 第四轮 A4（§12-11 甲路线）：v3 真序列化 golden-bytes 自检
#   被测：inc/plan_serializer.h（SerializePlanV3 / SerializeSegmentV3）
#   含三处同名登记 static_assert（ActionKind ↔ 槽 0 常量 ↔ server 分支 ID 镜像）
g++ -std=c++14 -Werror -Wall \
    -I shim -I "$UNIFIED" -I "$UNIFIED/inc" -I "$UNIFIED/l2" \
    -I "$UNIFIED/l3" -I "$UNIFIED/l4" -I "$UNIFIED/l5" \
    poc_unified_serialize_v3_test.cc -o poc_unified_serialize_v3_test

./poc_unified_serialize_v3_test

# A4 补充回归（BLOCKER-1 修复守门）：host 侧 "CcuAction" 前缀判定
#   被测：ccu_temp_kfc_server.cc 两处 compare(0,9,...) 判定式的语义镜像
#   （注册名命中 + v2 名单 11 名零误命中 + 长度字面量 == strlen）
g++ -std=c++14 -Werror -Wall poc_unified_kfc_gate_test.cc -o poc_unified_kfc_gate_test

./poc_unified_kfc_gate_test

# R6 · WP-A2（AICPU unified 执行器）：blob 判据 + exec 现值实例化 + 消费方驱动链
g++ -std=c++14 -Werror -Wall \
    -I shim -I "$UNIFIED" -I "$UNIFIED/inc" -I "$UNIFIED/l2" \
    -I "$UNIFIED/l3" -I "$UNIFIED/l4" -I "$UNIFIED/l5" \
    poc_unified_aicpu_exec_test.cc -o poc_unified_aicpu_exec_test

./poc_unified_aicpu_exec_test

# R6 · WP-A1（动词→真原语）：READ 五步 / SYNC 两段循环 / 同源断言 / 拒绝矩阵
#   真原语以桩记录（源码级判据：桩序列对照 wrapper.cc:521-551/:848-857/:881-890 手写期望）
g++ -std=c++14 -Werror -Wall \
    -I shim -I "$UNIFIED" -I "$UNIFIED/inc" -I "$UNIFIED/l2" \
    -I "$UNIFIED/l3" -I "$UNIFIED/l4" -I "$UNIFIED/l5" \
    poc_unified_verb_sink_test.cc -o poc_unified_verb_sink_test

./poc_unified_verb_sink_test

# R6 · WP-A1 消费方本体（unified_exec/aicpu_consumer.h）：v4 帧解读 + 现值实例化 +
#   逐段驱动 + 切轮多段 + 失败路径（53 项——含 blob 三重校验负例/切轮步长恢复）
UE_DIR="$UNIFIED/../unified_exec"
g++ -std=c++14 -Werror -Wall \
    -I shim -I "$UNIFIED" -I "$UNIFIED/inc" -I "$UNIFIED/l2" \
    -I "$UNIFIED/l3" -I "$UNIFIED/l4" -I "$UNIFIED/l5" -I "$UE_DIR" \
    "$UE_DIR/test/poc_unified_exec_test.cc" -o poc_unified_exec_test

./poc_unified_exec_test

# R12 · WP-2/WP-8：动词序列化器 v5（verb_serializer.h）golden-bytes + server verb
#   分发循环 host 镜像 + 限界/容量/未知 verb 负例 + 双路径等价不变量（64 项）
g++ -std=c++14 -Werror -Wall \
    -I shim -I "$UNIFIED" -I "$UNIFIED/inc" -I "$UNIFIED/l2" \
    -I "$UNIFIED/l3" -I "$UNIFIED/l4" -I "$UNIFIED/l5" \
    poc_unified_verb_exec_test.cc -o poc_unified_verb_exec_test

./poc_unified_verb_exec_test

# R12 · WP-5 灰度开关探针：UseUnifiedVerbPath 三态真值（编译期关恒 false；宏开 +
# RES env 未设 = v3 缺省；宏开 + 双 env = v5 放行 + 路由外 op 拒）。受控 env 各起
# 独立进程（static bool envOn 首调定格——与 run_st_checker.sh 路由专项同机制）。
cat > /tmp/route_r12_verb_off.cc <<'PROBE'
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
    const int ag = static_cast<int>(HcclCMDType::HCCL_CMD_ALLGATHER);
    const int rs = static_cast<int>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER);
    const int ar = static_cast<int>(HcclCMDType::HCCL_CMD_ALLREDUCE);
    const char* mode = "macro-off";
    if (std::getenv("MC2_USE_UNIFIED_VERB") != nullptr) { mode = "macro+env"; }
    else if (std::getenv("MC2_USE_UNIFIED_RES") != nullptr) { mode = "macro-on-res-only"; }
    printf("route verb probe mode=%s: AG=%d RS=%d AR=%d\n", mode,
        UseUnifiedVerbPath(ag), UseUnifiedVerbPath(rs), UseUnifiedVerbPath(ar));
    if (std::getenv("MC2_USE_UNIFIED_VERB") != nullptr) {
        Check(UseUnifiedVerbPath(ag) == true,  "macro+env: AG verb true (v5)");
        Check(UseUnifiedVerbPath(rs) == true,  "macro+env: RS verb true (v5)");
        Check(UseUnifiedVerbPath(ar) == false, "macro+env: AR not routed -> verb false");
    } else if (std::getenv("MC2_USE_UNIFIED_RES") != nullptr) {
        Check(UseUnifiedVerbPath(ag) == false, "macro on + RES only: verb false (v3 default)");
        Check(UseUnifiedVerbPath(rs) == false, "macro on + RES only: RS verb false");
    } else {
        Check(UseUnifiedVerbPath(ag) == false, "macro-off: verb false (compile gate, env ignored)");
        Check(UseUnifiedVerbPath(rs) == false, "macro-off: RS verb false");
    }
    printf("%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
PROBE
g++ -std=c++14 -Werror -Wall \
    -I shim -I "$UNIFIED" -I "$UNIFIED/inc" \
    /tmp/route_r12_verb_off.cc -o /tmp/route_r12_verb_off
echo "== R12 verb 路由探针（编译期默认关）=="
unset MC2_USE_UNIFIED_VERB MC2_USE_UNIFIED_RES && /tmp/route_r12_verb_off
echo "== R12 verb 路由探针（宏开 + RES 开 + VERB 关）=="
g++ -std=c++14 -Werror -Wall -DMC2_UNIFIED_VERB_RESOURCE -DMC2_UNIFIED_RESOURCE \
    -I shim -I "$UNIFIED" -I "$UNIFIED/inc" \
    /tmp/route_r12_verb_off.cc -o /tmp/route_r12_verb_on
unset MC2_USE_UNIFIED_VERB && export MC2_USE_UNIFIED_RES=1 && /tmp/route_r12_verb_on
unset MC2_USE_UNIFIED_RES
echo "== R12 verb 路由探针（宏开 + 双 env 开）=="
export MC2_USE_UNIFIED_RES=1 MC2_USE_UNIFIED_VERB=1 && /tmp/route_r12_verb_on
unset MC2_USE_UNIFIED_RES MC2_USE_UNIFIED_VERB

# R12 · WP-1：devkit 侧 v5 协议头探针（kfc_server_message.h v5 段 + kfc_server_protocol.h
#   UNIFIED_VERB role——独立进程实测：verb ID 表/槽位/XN 掩码/前缀长度/magic·版本/
#   blob 槽数公式/63 帧上限/v2 名单零误命中）
cat > /tmp/probe_kfc_v5_proto.cc <<'PROBE'
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
    printf("== R12 探针：devkit 头的 v5 协议常量（执行级） ==\n");
    Check(static_cast<int>(KfcServerRole::UNIFIED_VERB) == 12, "KfcServerRole::UNIFIED_VERB = 12 (PR rebase: shifted past upstream 9/10)");
    Check(static_cast<int>(KfcServerRole::UNIFIED_PLAN) == 11 &&
              static_cast<int>(KfcServerRole::ALL_GATHER_MESH) == 1,
          "v2 role enum unchanged; UNIFIED_PLAN = 11 (PR rebase shift)");
    Check(KFC_UNIFIED_VERB_COPY == 0U && KFC_UNIFIED_VERB_POST_COPY == 1U &&
              KFC_UNIFIED_VERB_READ == 2U && KFC_UNIFIED_VERB_WRITE == 3U &&
              KFC_UNIFIED_VERB_REDUCE == 4U && KFC_UNIFIED_VERB_BROADCAST == 5U &&
              KFC_UNIFIED_VERB_SYNC_PRE == 6U && KFC_UNIFIED_VERB_SYNC_POST == 7U &&
              KFC_UNIFIED_VERB_NHR_RUN == 8U &&
              KFC_UNIFIED_VERB_SLICE_REDUCE == 9U && KFC_UNIFIED_VERB_CH_SYNC == 10U &&
              KFC_UNIFIED_VERB_ID_MAX == 10U,
          "v5 verb ids = {0..10} (VerbCall::Verb order; NHR_RUN=8 R13, SLICE_REDUCE=9/"
          "CH_SYNC=10 R18 · #11 NHR 动词对称化——枚举尾纯追加, ID_MAX 8→10 是 M0_R18 §四裁决)");
    Check(KFC_VERB_SLOT_ARG0 == 1 && KFC_VERB_SLOT_ARG1 == 2 && KFC_VERB_SLOT_ARG2 == 3 &&
              KFC_VERB_SLOT_ARG3 == 4 && KFC_VERB_GO_SIZE_0 == 5 && KFC_VERB_GO_SIZE_3 == 8 &&
              KFC_VERB_SLOTS_PER_FRAME == 32U,
          "v5 frame slots {1,2,3,4} args + {5..8} goSize, 32 slots/frame");
    Check(KFC_VERB_XN_MASK_AG_PRE == 0xCU && KFC_VERB_XN_MASK_RS_PRE == 0x7U &&
              KFC_VERB_XN_MASK_POST == 0x10U && KFC_VERB_XN_MASK_NHR_PRE == 0x5U,
          "XN masks AG=0xC RS=0x7 POST=0x10 NHR=0x5 ({IN=0,SCR=1,TOK=2,OUT=3,POST=4})");
    // R13：NHR_RUN 帧槽位镜像 sole NHR 布局（KfcVerbNhrRunSlot ↔ KFC_RS_SOLE_NHR_*
    // 互锁 + 槽 31 pattern 载体 + NHR_RUN 槽宽 ≤ 32）——值比较经 uint32_t 显式
    // 转换（两枚举类型不同，直接 == 触发 -Wenum-compare）
    Check(KFC_VERB_NHR_INPUT == 1U && KFC_VERB_NHR_OUTPUT == 2U &&
              static_cast<uint32_t>(KFC_VERB_NHR_SLICE_SIZE) ==
                  static_cast<uint32_t>(KFC_RS_SOLE_NHR_SLICE_SIZE) &&
              static_cast<uint32_t>(KFC_VERB_NHR_INPUT_SLICE_STRIDE) ==
                  static_cast<uint32_t>(KFC_RS_SOLE_NHR_INPUT_SLICE_STRIDE) &&
              static_cast<uint32_t>(KFC_VERB_NHR_SLICE_ONE_JETTY_SIZE) ==
                  static_cast<uint32_t>(KFC_RS_SOLE_NHR_SLICE_ONE_JETTY_SIZE) &&
              static_cast<uint32_t>(KFC_VERB_NHR_SLICE_LAST_JETTY_SIZE) ==
                  static_cast<uint32_t>(KFC_RS_SOLE_NHR_SLICE_LAST_JETTY_SIZE) &&
              static_cast<uint32_t>(KFC_VERB_NHR_REPEAT_NUM_INV) ==
                  static_cast<uint32_t>(KFC_RS_SOLE_NHR_REPEAT_NUM_INV) &&
              static_cast<uint32_t>(KFC_VERB_NHR_INPUT_REPEAT_STRIDE) ==
                  static_cast<uint32_t>(KFC_RS_SOLE_NHR_INPUT_REPEAT_STRIDE) &&
              static_cast<uint32_t>(KFC_VERB_NHR_OUTPUT_REPEAT_STRIDE) ==
                  static_cast<uint32_t>(KFC_RS_SOLE_NHR_OUTPUT_REPEAT_STRIDE) &&
              KFC_VERB_NHR_JETTY_NUM == 10U && KFC_VERB_NHR_PARAM_NUM == 11U,
          "R13 NHR_RUN slots mirror sole-NHR layout (slot 3..9) + slot 10 jettyNum");
    Check(KFC_VERB_SLOT_PATTERN == 31U && KFC_VERB_PATTERN_MESH == 0U &&
              KFC_VERB_PATTERN_NHR == 1U,
          "R13 pattern carrier: SYNC_PRE slot 31 (MESH=0 default / NHR=1)");
    Check(sizeof(KFC_UNIFIED_VERB_PREFIX) - 1 == KFC_UNIFIED_VERB_PREFIX_LEN &&
              KFC_UNIFIED_VERB_PREFIX_LEN == 7U,
          "CcuVerb prefix literal len == 7 == constant (BLOCKER-1 discipline)");
    Check(KFC_UNIFIED_VERB_BLOB_MAGIC == 0x55504C35ULL && KFC_UNIFIED_VERB_BLOB_VERSION == 5ULL,
          "UPL5 magic + version 5 (decoupled from UPL3/4)");
    Check(KfcUnifiedVerbBlobTotalSlots(1U) == 39U && KfcUnifiedVerbBlobTotalSlots(63U) == 2023U,
          "verb blob slots = 7 + frames*32 (39 / 2023 literals)");
    Check(KFC_UNIFIED_VERB_MAX_FRAMES == 63U, "frame cap 63 (ring 64 - sentinel)");
    Check(KFC_UNIFIED_BLOB_MAGIC == 0x55504C33ULL && KFC_UNIFIED_BLOB_VERSION == 4ULL,
          "v4 blob constants unchanged (UPL3/4 coexist by gate)");
    const char* v2Names[] = {KFC_CONCURRENT_ALL_GATHER_ALG_NAME, KFC_RS_SOLE_NHR_ALG_NAME,
                             "CcuKernelKfcServer", "CcuSchedAllGatherMesh1D"};
    int hits = 0;
    for (const char* n : v2Names) {
        for (uint32_t m = 0; m < 2; ++m) {
            if (GetKfcServerRole(n, m) == KfcServerRole::UNIFIED_VERB) ++hits;
            if (GetKfcServerRole(n, m) == KfcServerRole::UNIFIED_PLAN) ++hits;
        }
    }
    Check(hits == 0, "v2 algName->role never yields UNIFIED_VERB/UNIFIED_PLAN (4 names x 2 missions)");
    printf("%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
PROBE
HCCL_ROOT=$(cd ../../../../.. && pwd)   # impl/adv_api/detail/hccl
g++ -std=c++14 -Werror -Wall \
    -I "$HCCL_ROOT/cc/src/common" -I "$HCCL_ROOT/common" \
    /tmp/probe_kfc_v5_proto.cc -o /tmp/probe_kfc_v5_proto
/tmp/probe_kfc_v5_proto
