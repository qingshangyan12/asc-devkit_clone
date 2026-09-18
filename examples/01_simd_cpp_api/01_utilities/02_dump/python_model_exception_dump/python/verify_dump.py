#!/usr/bin/python3

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

"""验证 Exception Dump 产物数据正确性：与同 seed 重建输入（含注入值回填）逐 tensor bit-exact 比对。
用法: python verify_dump.py [dump目录，默认 ../test/task_fail_dump 最新 dev*]"""

import glob
import os
import sys

import numpy as np
import torch

torch.manual_seed(42)

# ---- 与 ifa_exception_dump.py 完全一致的输入重建 ----
B, QH, KVH, QS, BS, D, KV_SEQ = 18, 64, 1, 1, 128, 128, 8192
MAXB = KV_SEQ // BS + 1
BLOCK_NUM = B * MAXB

q_ref = torch.randn(B, QH, QS, D, dtype=torch.bfloat16)
key_ref = torch.randint(0, 100, (BLOCK_NUM, KVH, D // 32, BS, 32), dtype=torch.int8)
value_ref = torch.randint(0, 100, (BLOCK_NUM, KVH, D // 32, BS, 32), dtype=torch.int8)
block_table_ref = torch.arange(B * MAXB, dtype=torch.int32).view(B, MAXB)
actual_ref = torch.tensor([KV_SEQ] * B, dtype=torch.int64)
dk_ref = torch.randn(KVH, 1, D, dtype=torch.bfloat16)
dv_ref = torch.randn(KVH, 1, D, dtype=torch.bfloat16)

INJECT = {"block_table": (0, 0, 0x7FFFFFFF)}  # scene1 注入值

# ---- 定位 dump 目录（默认脚本目录旁的 test/，保证任意 CWD 可执行；显式传参优先）----
HERE = os.path.dirname(os.path.abspath(__file__))
base = os.path.join(HERE, "..", "test", "task_fail_dump")
dirs = sorted(glob.glob(os.path.join(base, "dev*")))
if len(sys.argv) >= 2:
    dump_dir = sys.argv[1]
elif dirs:
    dump_dir = dirs[-1]
else:
    print("无 dump 目录（在 %s 下未找到 dev*）" % base)
    sys.exit(1)
print("验证目录:", dump_dir)

# ---- 解析 inputs_info.txt ----
info = {}
cur = None
with open(os.path.join(dump_dir, "inputs_info.txt")) as f:
    for line in f:
        line = line.strip()
        if line.startswith("[") and line.endswith("]"):
            cur = line[1:-1]
        elif cur and "=" in line:
            k, v = [x.strip() for x in line.split("=", 1)]
            info.setdefault(cur, {})[k] = v

# ---- 注入回填（dump 的是注入后的设备数据）----
if "block_table" in INJECT:
    i, j, val = INJECT["block_table"]
    block_table_ref[i, j] = val

refs = {
    "query": q_ref,
    "key_cache": key_ref,
    "value_cache": value_ref,
    "actual_seq_kvlen": actual_ref,
    "block_table": block_table_ref,
    "dequant_scale_key": dk_ref,
    "dequant_scale_value": dv_ref,
}


def tensor_bytes(t):
    # bf16 不支持 .numpy()，按位 view 成 uint16 保证 bit-exact 比对
    return (t.view(torch.uint16) if t.dtype == torch.bfloat16 else t).numpy().tobytes()


print("\n===== dump 数据 vs 原始数据（公开接口链路）=====")
ok = 0
total = 0
for name, entry in info.items():
    if name not in refs:
        print("  %-20s (无参考值，跳过)" % name)
        continue
    total += 1
    ref = refs[name]
    data = np.fromfile(os.path.join(dump_dir, "%s.bin" % name), dtype=np.uint8)
    ref_np = tensor_bytes(ref)
    if data.tobytes() == ref_np:
        print("  %-20s BIT-EXACT" % name)
        ok += 1
    else:
        n = min(len(data), len(ref_np))
        diff_idx = next((k for k in range(n) if data[k] != ref_np[k]), -1)
        print(
            "  %-20s DIFF (首个差异 offset=%s, size=%d vs ref=%d)"
            % (name, diff_idx, len(data), len(ref_np))
        )

if "metadata" in info:
    m = np.fromfile(os.path.join(dump_dir, "metadata.bin"), dtype=np.int32)
    print("  %-20s usedCoreNum=%d, len=%d" % ("metadata", m[0], len(m)))

print("\n结论: %d/%d tensor bit-exact 一致（含注入的损坏值）" % (ok, total))
sys.exit(0 if ok == total else 2)
