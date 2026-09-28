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

"""离线解析 Exception Dump 保存的 kernel args 区（args_raw.bin）：32 槽入参指针区 + tiling 字段。
字段布局依据 incre_flash_attention_tilingdata.h 的 BaseParams 定义顺序。
用法: python parse_tiling.py [args_raw.bin 路径（默认 ../test/task_fail_dump 最新 dump）]"""

import glob
import os
import struct
import sys

import numpy as np

# ---- 定位 args_raw.bin（默认脚本目录旁的 test/，保证任意 CWD 可执行）----
base = os.path.join(
    os.path.dirname(os.path.abspath(__file__)), "..", "test", "task_fail_dump"
)
cands = sorted(glob.glob(os.path.join(base, "dev*", "args_raw.bin")))
if not cands:
    print("未找到 args_raw.bin，请先运行 test/test_public_dump.py scene1")
    sys.exit(1)
path = sys.argv[1] if len(sys.argv) > 1 else cands[-1]
raw = np.fromfile(path, dtype=np.uint8)
print("解析: %s (%d bytes)\n" % (path, raw.size))

# ---- Part 1: 前 32 个入参指针 ----
NAMES = [
    "query",
    "key",
    "value",
    "pseShift",
    "attenMask",
    "actualSeqLengthsQ",
    "actualSeqLengths",
    "deqScale1",
    "quantScale1",
    "deqScale2",
    "quantScale2",
    "quantOffset2",
    "antiquantScale",
    "antiquantOffset",
    "blocktable",
    "queryPaddingSize",
    "kvPaddingSize",
    "keyAntiquantScale",
    "keyAntiquantOffset",
    "valueAntiquantScale",
    "valueAntiquantOffset",
    "keySharedPrefix",
    "valueSharedPrefix",
    "actualSharedPrefixLen",
    "queryRope",
    "keyRope",
    "keyRopeAntiquantScale",
    "dequantScaleQuery",
    "metaData",
    "attentionOut",
    "softmaxLse",
    "workspace",
]
print("===== kernel 入参指针区（args[0..31]，每槽 8B）=====")
for i in range(32):
    (p,) = struct.unpack_from("<Q", raw, i * 8)
    flag = "" if p == 0 else "  <-- 非空"
    print("  args[%2d] %-24s = 0x%016x%s" % (i, NAMES[i] + ":", p, flag))

# ---- Part 2: tiling 结构（offset 256 起，按 BaseParams 字段序）----
TILING_OFF = 256
FIELDS = [
    ("batchSize", "I"),
    ("seqSize", "I"),
    ("qSeqSize", "I"),
    ("headSize", "I"),
    ("headSizeV", "I"),
    ("blockSize", "I"),
    ("maxBlockNumPerBatch", "I"),
    ("maxBlockNumPerSeq", "I"),
    ("scaleValue", "f"),
    ("kvHeadNum", "I"),
    ("headNumRatio", "I"),
    ("qHeadNum", "I"),
    ("nNumOfQInOneGroup", "I"),
    ("batchContinuousFlag", "I"),
]
print("\n===== IncreFlashAttentionTilingData.BaseParams (offset %d) =====" % TILING_OFF)
off = TILING_OFF
for name, fmt in FIELDS:
    (v,) = struct.unpack_from("<" + fmt, raw, off)
    print("  +%-4d %-26s = %s" % (off, name, v))
    off += 4

# 已知值校验（本测试场景）
expect = {
    "batchSize": 18,
    "seqSize": 8320,
    "qSeqSize": 1,
    "headSize": 128,
    "headSizeV": 128,
    "blockSize": 128,
    "maxBlockNumPerBatch": 65,
    "kvHeadNum": 1,
    "qHeadNum": 64,
    "nNumOfQInOneGroup": 64,
    "batchContinuousFlag": 1,
}
print("\n===== 与 python 侧调用参数的一致性校验 =====")
ok = 0
for k, v in expect.items():
    fmt = dict(FIELDS)[k]
    (got,) = struct.unpack_from(
        "<" + fmt, raw, TILING_OFF + [f for f, _ in FIELDS].index(k) * 4
    )
    mark = "OK" if got == v else "MISMATCH(期望 %s)" % v
    ok += got == v
    print("  %-24s got=%-10s %s" % (k, got, mark))
(scale,) = struct.unpack_from("<f", raw, TILING_OFF + 32)
print(
    "  scaleValue=%.6f (期望 1/sqrt(128)=%.6f) %s"
    % (scale, 128**-0.5, "OK" if abs(scale - 128**-0.5) < 1e-6 else "MISMATCH")
)
print("\n校验结果: %d/%d 字段一致" % (ok + 1, len(expect) + 1))

# ---- Part 3: tiling 尾部上下文（u32 连续段，标注可疑含义）----
print(
    "\n===== tiling 后续 u32 概览（偏移 %d 之后，共 %d B）====="
    % (TILING_OFF, raw.size - TILING_OFF)
)
u32all = (
    raw[TILING_OFF:].copy().view(np.uint32)
    if (raw.size - TILING_OFF) % 4 == 0
    else None
)
if u32all is not None:
    interesting = [
        (i, int(v)) for i, v in enumerate(u32all) if v in (1170, 8320, 18, 8192, 43692)
    ]  # totalBlockNum/seqSize 等
    print(
        "  命中已知特征值: "
        + ", ".join("+%d=%d" % (TILING_OFF + i * 4, v) for i, v in interesting[:10])
    )
    print("  (完整字段对照需结合 incre_flash_attention_tilingdata.h 各模板 Params 段)")
