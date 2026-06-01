#!/bin/bash
# ----------------------------------------------------------------------------------------------------------
# This program is free software, you can redistribute it and/or modify it.
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This file is a part of the CANN Open Software.
# Licensed under CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
#
# autotune.sh —— msprof 驱动的"最优核数"二分查找
#
# 原理：中shape下 Task Duration 关于核数是单峰凸函数（核数太小每核工作过重、核数太大调度头
#       开销过重，中间存在唯一谷底）。用斜率符号二分：比较 f(mid) 与 f(mid+1)，
#         f(mid) <= f(mid+1)  → 谷底在左侧（含 mid），令 hi = mid
#         f(mid) >  f(mid+1)  → 谷底在右侧，         令 lo = mid + 1
#       约 log2(范围) 次测量即可收敛。
#
# 计时：用 msprof op 的 Task Duration（device 侧，含调度头开销），多次取平均。
#       不用程序内 aclrtEvent 计时——实测其在高核数区偏平、噪声大，无法稳定定位谷底。
#
# 用法： ./autotune.sh [可执行文件] [核数下界] [核数上界] [每候选采样次数]
#        默认： ./build/grid_config_tune  1  64  3
#
set -u

BIN="${1:-./build/grid_config_tune}"
LO="${2:-1}"
HI="${3:-64}"
TRIALS="${4:-3}"

if [ ! -x "$BIN" ]; then
    echo "[error] 找不到可执行文件: $BIN"
    echo "        请先在本目录: mkdir build && cd build && cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 .. && make"
    exit 1
fi
if ! command -v msprof >/dev/null 2>&1; then
    echo "[error] 未找到 msprof，请先 source CANN 的 set_env.sh"
    exit 1
fi

WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

declare -A CACHE   # 核数 -> 平均 Task Duration，避免重复测量

# measure <num_blocks> ：跑 TRIALS 次 msprof，回显平均 Task Duration(us)
measure() {
    local nb="$1"
    if [ -n "${CACHE[$nb]:-}" ]; then
        echo "${CACHE[$nb]}"
        return
    fi
    local sum=0 cnt=0 t
    for ((t = 1; t <= TRIALS; t++)); do
        local out="$WORK/nb${nb}_t${t}"
        rm -rf "$out"
        # 失败重试一次
        msprof op --application="$BIN $nb" --output="$out" >/dev/null 2>&1 \
            || msprof op --application="$BIN $nb" --output="$out" >/dev/null 2>&1
        local csv
        csv="$(find "$out" -name OpBasicInfo.csv 2>/dev/null | head -1)"
        [ -z "$csv" ] && continue
        local dur
        dur="$(python3 - "$csv" <<'PY'
import csv, sys
rows = list(csv.DictReader(open(sys.argv[1])))
if not rows:
    sys.exit(0)
key = [c for c in rows[0] if 'Task Duration' in c][0]
print(rows[0][key])
PY
)"
        [ -z "$dur" ] && continue
        sum="$(python3 -c "print($sum + $dur)")"
        cnt=$((cnt + 1))
    done
    if [ "$cnt" -eq 0 ]; then
        echo "ERR"
        return
    fi
    local avg
    avg="$(python3 -c "print(round($sum / $cnt, 3))")"
    CACHE[$nb]="$avg"
    echo "$avg"
}

echo "=================================================================="
echo " 中shape gather 最优核数二分查找 (msprof Task Duration 驱动)"
echo " 可执行文件: $BIN"
echo " 搜索区间: [$LO, $HI]   每候选采样: $TRIALS 次取平均"
echo "=================================================================="
printf "%-4s %-8s %-8s %-12s %-12s %-s\n" "轮" "lo" "hi" "f(mid)" "f(mid+1)" "决策"
echo "------------------------------------------------------------------"

lo="$LO"
hi="$HI"
round=0
while [ "$lo" -lt "$hi" ]; do
    round=$((round + 1))
    mid=$(((lo + hi) / 2))
    f_mid="$(measure "$mid")"
    f_next="$(measure "$((mid + 1))")"
    if [ "$f_mid" = "ERR" ] || [ "$f_next" = "ERR" ]; then
        echo "[error] msprof 测量失败，终止"
        exit 1
    fi
    # 打印本轮（区间为收敛前的 [lo,hi]）
    if python3 -c "exit(0 if $f_mid <= $f_next else 1)"; then
        decision="f(mid)<=f(mid+1) → 谷底在左, hi=$mid"
        printf "%-4s [%-3s,%-3s] mid=%-4s %-11s %-11s %s\n" \
            "$round" "$lo" "$hi" "$mid" "$f_mid" "$f_next" "$decision"
        hi="$mid"
    else
        decision="f(mid)>f(mid+1) → 谷底在右, lo=$((mid + 1))"
        printf "%-4s [%-3s,%-3s] mid=%-4s %-11s %-11s %s\n" \
            "$round" "$lo" "$hi" "$mid" "$f_mid" "$f_next" "$decision"
        lo="$((mid + 1))"
    fi
done

best_nb="$lo"
best_dur="$(measure "$best_nb")"
echo "------------------------------------------------------------------"
echo " 收敛：最优核数 = $best_nb 核，Task Duration ≈ ${best_dur} us"
echo " 对照：可用 full_sweep 全量扫描验证该谷底（见 perf_data/full_sweep.csv）"
echo "=================================================================="
