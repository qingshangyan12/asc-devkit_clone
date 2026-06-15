#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

"""
gen_data.py — bank_conflict 样例数据生成

支持 CASE_NUM=1~9，按地址重叠关系分三类优化：

  1. CASE 1~5 — z 与 x/y 无地址重叠
     1000 次 z = x + y 幂等，golden = x + y，无需循环。

  2. CASE 6, 8, 9 — z == y（完全重叠），单次 Add 内无跨 repeat 干扰
     每次迭代 result += src0_pattern，需逐次累加以保证 float32 精度一致。
     src0_pattern 根据 src0BlkStride / src0RepStride 构建。

  3. CASE 7 — z = y + 8 floats（部分重叠），单次 Add 内存在跨 repeat 读写干扰
     1000 次迭代后（> 512）收敛到不动点，golden 可由 stride-8 累加和解析计算：
       golden[i::8] = cumsum(x[i::8]) + y[i]
"""

import numpy as np
import os
import argparse

# ---- kernel 常量 (与 bank_conflict.asc / config.h 对齐) ----
TOTAL_LENGTH = 4096
REPEAT_TIMES_FOR_INSTR_EXCUTE = 1000
ITEMS_PER_ITER = 64                       # 256 bytes / sizeof(float) = 64 elements per repeat
REPEAT_TIMES = TOTAL_LENGTH // ITEMS_PER_ITER  # 64
DATABLOCK_FLOATS = 8                      # 32 bytes / sizeof(float) = 8 floats per DataBlock
NUM_BLOCKS_PER_REPEAT = ITEMS_PER_ITER // DATABLOCK_FLOATS  # 8

# ---- 地址配置 (来自 config.h，单位: 字节) ----
ADDR_CONFIGS = {
    1: (0x00000, 0x10100, 0x20000),
    2: (0x00000, 0x04000, 0x10000),
    3: (0x00000, 0x10000, 0x14100),
    4: (0x00000, 0x10100, 0x04000),
    5: (0x00000, 0x10100, 0x04020),
    6: (0x00000, 0x10100, 0x10100),
    7: (0x00000, 0x10100, 0x10120),
    8: (0x00000, 0x10100, 0x10100),
    9: (0x00000, 0x10100, 0x10100),
}

# ---- BinaryRepeatParams: (dstBlkS, src0BlkS, src1BlkS, dstRepS, src0RepS, src1RepS) ----
STRIDE_CONFIGS = {
    1: (1, 1, 1, 8, 8, 8),
    2: (1, 1, 1, 8, 8, 8),
    3: (1, 1, 1, 8, 8, 8),
    4: (1, 1, 1, 8, 8, 8),
    5: (1, 1, 1, 8, 8, 8),
    6: (1, 1, 1, 8, 8, 8),
    7: (1, 1, 1, 8, 8, 8),
    8: (1, 0, 1, 8, 1, 8),
    9: (1, 0, 1, 8, 8, 8),
}


def _build_src0_pattern(x_data, case_num):
    """根据 Add 高维切分语义，构建一次 Add 调用中 src0 的完整读取模式。

    对 repeat r (0..63)、DataBlock k (0..7)：
      src0 起始地址 = x_off + r * src0RepStride * 8 + k * src0BlkStride * 8
      连续读取 8 个 float 元素
    """
    _, src0BlkS, _, _, src0RepS, _ = STRIDE_CONFIGS[case_num]
    src0 = np.empty(TOTAL_LENGTH, dtype=np.float32)
    for r in range(REPEAT_TIMES):
        for k in range(NUM_BLOCKS_PER_REPEAT):
            src_idx = r * src0RepS * DATABLOCK_FLOATS + k * src0BlkS * DATABLOCK_FLOATS
            dst_start = r * ITEMS_PER_ITER + k * DATABLOCK_FLOATS
            src0[dst_start:dst_start + DATABLOCK_FLOATS] = x_data[src_idx:src_idx + DATABLOCK_FLOATS]
    return src0


def _gen_golden_no_overlap(x_data, y_data):
    return (x_data + y_data).astype(np.float32)


def _gen_golden_z_eq_y(x_data, y_data, case_num):
    """CASE 6, 8, 9: z == y，无跨 repeat 干扰。
    每次 Add 调用 result += src0_pattern，逐次累加保证 float32 精度一致。
    """
    src0_pattern = _build_src0_pattern(x_data, case_num)
    result = y_data.copy()
    for _ in range(REPEAT_TIMES_FOR_INSTR_EXCUTE):
        result += src0_pattern
    return result


def _gen_golden_case7(x_data, y_data):
    """CASE 7: z = y + 8 floats，跨 repeat 读写干扰。
    1000 次迭代后收敛到不动点（512 < 1000 即可收敛），由 stride-8 累加和解析计算：
      golden[i::8] = cumsum(x[i::8]) + y[i]
    """
    x_mat = x_data.reshape(-1, 8)  # (512, 8)
    golden = (np.cumsum(x_mat, axis=0) + y_data[:8]).reshape(-1)
    return golden.astype(np.float32)


def gen_golden_data(case_num):
    np.random.seed(0)
    input_x = np.random.uniform(1, 100, [TOTAL_LENGTH]).astype(np.float32)
    input_y = np.random.uniform(1, 100, [TOTAL_LENGTH]).astype(np.float32)

    if case_num in (1, 2, 3, 4, 5):
        golden = _gen_golden_no_overlap(input_x, input_y)
    elif case_num == 7:
        golden = _gen_golden_case7(input_x, input_y)
    else:
        golden = _gen_golden_z_eq_y(input_x, input_y, case_num)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_x.tofile("./input/input_x.bin")
    input_y.tofile("./input/input_y.bin")
    golden.tofile("./output/golden.bin")

    print(f"[INFO] CASE_NUM={case_num}, golden generated")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate test data for bank_conflict")
    parser.add_argument("--case", "-c", type=int, default=None,
                        help="case number (1-9), overrides CASE_NUM env")
    args = parser.parse_args()

    case_num = args.case if args.case is not None else int(os.environ.get("CASE_NUM", "1"))
    gen_golden_data(case_num)
