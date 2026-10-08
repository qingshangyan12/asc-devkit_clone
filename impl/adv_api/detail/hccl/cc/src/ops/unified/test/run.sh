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

# R6 · WP-A2 消费方本体（unified_exec/aicpu_consumer.h）：v4 帧解读 + 现值实例化 +
#   逐段驱动 + 切轮多段 + 失败路径（53 项——含 blob 三重校验负例/切轮步长恢复）
UE_DIR="$UNIFIED/../unified_exec"
g++ -std=c++14 -Werror -Wall \
    -I shim -I "$UNIFIED" -I "$UNIFIED/inc" -I "$UNIFIED/l2" \
    -I "$UNIFIED/l3" -I "$UNIFIED/l4" -I "$UNIFIED/l5" -I "$UE_DIR" \
    "$UE_DIR/test/poc_unified_exec_test.cc" -o poc_unified_exec_test

./poc_unified_exec_test
