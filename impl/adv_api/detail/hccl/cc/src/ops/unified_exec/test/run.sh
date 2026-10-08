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
