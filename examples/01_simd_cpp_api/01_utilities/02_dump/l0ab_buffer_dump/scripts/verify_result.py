#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import re
import sys
from pathlib import Path

import numpy as np


MATMUL_M = 16
MATMUL_K = 32
MATMUL_N = 16
A_ELEMENT_COUNT = MATMUL_M * MATMUL_K
B_ELEMENT_COUNT = MATMUL_K * MATMUL_N
OUTPUT_ELEMENT_COUNT = MATMUL_M * MATMUL_N
TOLERANCE = 1e-6


def verify_result(output_path, golden_path):
    output = np.fromfile(output_path, dtype=np.float32)
    golden = np.fromfile(golden_path, dtype=np.float32)
    if output.size != OUTPUT_ELEMENT_COUNT or golden.size != OUTPUT_ELEMENT_COUNT:
        print(f"unexpected element count: output={output.size}, golden={golden.size}, expected={OUTPUT_ELEMENT_COUNT}")
        print("Output result check: FAIL")
        return False
    mismatch_indexes = np.flatnonzero(np.abs(output - golden) > TOLERANCE)
    for index in mismatch_indexes[:100]:
        print(f"data index: {index:06d}, expected: {golden[index]:.6f}, actual: {output[index]:.6f}")
    passed = mismatch_indexes.size == 0
    print(f"Output result check: {'PASS' if passed else 'FAIL'}")
    return passed


def parse_dump(text, desc, expected_count, expected_fields=()):
    matches = re.findall(rf"DumpTensor: desc={desc},([^\n]*)\n\[(.*?)\]", text, re.S)
    if len(matches) != 1:
        raise RuntimeError(f"expected one DumpTensor desc={desc}, found {len(matches)}")
    header, body = matches[0]
    for field in expected_fields:
        if field not in header:
            raise RuntimeError(f"DumpTensor desc={desc} missing header field: {field}")
    values = []
    for item in body.replace("\n", " ").split(","):
        item = item.strip()
        if not item:
            continue
        values.append(float(item))
    if len(values) != expected_count:
        raise RuntimeError(
            f"unexpected DumpTensor desc={desc} element count: {len(values)}, expected: {expected_count}"
        )
    return values


def build_a_layout_golden(a_flat):
    # A1/A2 的 NZ 物理布局：[K 分形 j][M 行][16 个连续 K 元素]。
    # A1_phys[j*256 + m*16 + kk] = A[m][j*16 + kk]
    golden = []
    for j in range(MATMUL_K // 16):
        for m in range(MATMUL_M):
            for kk in range(16):
                golden.append(float(a_flat[m * MATMUL_K + j * 16 + kk]))
    return golden


def build_b1_layout_golden(b_flat):
    # B1 的 NZ 物理布局：nValue=K，dValue=N，每个 K 行 32B=16 个 N 元素连续，即 B1 按 [K, N] 行主序。
    # B1_phys[k*16 + n] = B[k][n]
    golden = []
    for k in range(MATMUL_K):
        for n in range(MATMUL_N):
            golden.append(float(b_flat[k * MATMUL_N + n]))
    return golden


def build_b2_layout_golden(b_flat):
    # L0B 的 ZN 物理布局：LoadData 对每个 [16,16] 分形转置后，按 K 分形 f 顺序摆放。
    # B2_phys[f*256 + n*16 + kk] = B[f*16 + kk][n]
    golden = []
    for f in range(MATMUL_K // 16):
        for n in range(MATMUL_N):
            for kk in range(16):
                golden.append(float(b_flat[(f * 16 + kk) * MATMUL_N + n]))
    return golden


def check_equal(name, actual, expected):
    if len(actual) != len(expected):
        print(f"{name}: FAIL (length actual={len(actual)}, expected={len(expected)})")
        return False
    ok = all(abs(a - e) <= TOLERANCE for a, e in zip(actual, expected))
    if ok:
        print(f"{name}: PASS")
        return True
    print(f"{name}: FAIL")
    for index, (a, e) in enumerate(zip(actual, expected)):
        if abs(a - e) > TOLERANCE:
            print(f"  [{index:03d}] actual={a:.6f}, expected={e:.6f}")
            if index > 16:
                print("  ...")
                break
    return False


def main():
    if len(sys.argv) != 6:
        print(f"Usage: {sys.argv[0]} <output.bin> <golden.bin> <run.log> <a.bin> <b.bin>")
        return 1

    text = Path(sys.argv[3]).read_text(encoding="utf-8")
    a = np.fromfile(sys.argv[4], dtype=np.float16).astype(np.float64)
    b = np.fromfile(sys.argv[5], dtype=np.float16).astype(np.float64)
    if a.size != A_ELEMENT_COUNT:
        raise RuntimeError(f"unexpected matrix A element count: {a.size}")
    if b.size != B_ELEMENT_COUNT:
        raise RuntimeError(f"unexpected matrix B element count: {b.size}")
    a_flat = a.tolist()
    b_flat = b.tolist()

    a1_golden = build_a_layout_golden(a_flat)
    a2_golden = a1_golden  # 本样例配置下 L0A 与 L1 物理布局一致
    b1_golden = build_b1_layout_golden(b_flat)
    b2_golden = build_b2_layout_golden(b_flat)

    checks = [
        verify_result(sys.argv[1], sys.argv[2]),
        check_equal(
            "Matrix A in L1 data check",
            parse_dump(text, 100, A_ELEMENT_COUNT, ("data_type=float16", "position=L1")),
            a1_golden,
        ),
        check_equal(
            "Matrix B in L1 data check",
            parse_dump(text, 101, B_ELEMENT_COUNT, ("data_type=float16", "position=L1")),
            b1_golden,
        ),
        check_equal(
            "Matrix A in L0A data check",
            parse_dump(text, 200, A_ELEMENT_COUNT, ("data_type=float16", "position=L0A")),
            a2_golden,
        ),
        check_equal(
            "Matrix B in L0B data check",
            parse_dump(text, 201, B_ELEMENT_COUNT, ("data_type=float16", "position=L0B")),
            b2_golden,
        ),
    ]
    if all(checks):
        print("test pass!")
        return 0
    return 1


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (OSError, RuntimeError, ValueError) as error:
        print(error)
        sys.exit(1)
