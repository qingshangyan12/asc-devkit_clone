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

import os
import numpy as np


np.random.seed(19)

# 规格（小 shape 版，与 kernel 一致）
BATCH = 1
C1 = 2
C0 = 16
CIN = C1 * C0  # 32
H = 9
W = 18
COUT = 64
KH = 3
KW = 3
STRIDE = (1, 1)
DILATION = (1, 1)
PADDING = (1, 1, 1, 1)  # top, bottom, left, right
PAD_VALUE = 0


def conv2d_nhwc(x, weight, stride, dilation, padding, pad_value=0):
    """x: [N, H, W, Cin]，weight: [Cout, Cin, KH, KW] -> [N, HOUT, WOUT, Cout]。
    im2col + matmul，分块处理以控制内存峰值。
    """
    stride_h, stride_w = stride
    dilation_h, dilation_w = dilation
    pad_top, pad_bottom, pad_left, pad_right = padding
    n, h, w, cin = x.shape
    cout, _, kh, kw = weight.shape
    hout = (h + pad_top + pad_bottom - dilation_h * (kh - 1) - 1) // stride_h + 1
    wout = (w + pad_left + pad_right - dilation_w * (kw - 1) - 1) // stride_w + 1
    x_pad = np.pad(
        x,
        ((0, 0), (pad_top, pad_bottom), (pad_left, pad_right), (0, 0)),
        mode="constant",
        constant_values=pad_value,
    ).astype(np.float32)

    # weight -> [Cin*KH*KW, Cout]，K 序 (Cin, KH, KW) 与 im2col 一致
    wmat = weight.astype(np.float32).transpose(1, 2, 3, 0).reshape(cin * kh * kw, cout)

    y = np.zeros((n, hout, wout, cout), dtype=np.float32)
    # 逐 batch、逐输出行分块，避免一次构造 [N*HOUT*WOUT, Cin*KH*KW] 巨阵
    for ni in range(n):
        for ho in range(hout):
            cols = np.zeros((wout, cin, kh, kw), dtype=np.float32)
            for hk in range(kh):
                hi = ho * stride_h + hk * dilation_h
                for wk in range(kw):
                    ws = wk * dilation_w
                    cols[:, :, hk, wk] = x_pad[ni, hi, ws : ws + wout * stride_w : stride_w, :]
            y[ni, ho] = cols.reshape(wout, cin * kh * kw) @ wmat
    return y


def weight_to_nd(weight):
    """weight: [Cout, Cin, KH, KW]，Cin = C1*C0 -> [K, Cout] 行主序。

    kernel 侧用 MakeFrameLayout<NDExtLayoutPtn>(K, COUT) 描述 Global Memory 上的
    weight，因此这里必须输出行主序的 [K, Cout]：同一个 k 对应的 Cout 个值连续。

    K 序 = (C1, KH, KW, C0)，与 img2col 展开 K 的顺序一致：
        k = ((c1 * KH + hk) * KW + wk) * C0 + c0
    这样按 C1 切分 K 时，每个 C1 块对应连续的 KH*KW*C0 行，正好是 STEP_KA 个 BASE_K。
    """
    cout, cin, kh, kw = weight.shape
    k = cin * kh * kw
    # [Cout, C1, C0, KH, KW] -> [C1, KH, KW, C0, Cout]
    w5 = weight.reshape(cout, C1, C0, kh, kw)
    out = np.transpose(w5, (1, 3, 4, 2, 0)).reshape(k, cout)
    return np.ascontiguousarray(out)


def to_nc1hwc0(x_nhwc, c1):
    n, h, w, c = x_nhwc.shape
    assert c == c1 * C0
    x = x_nhwc.reshape(n, h, w, c1, C0)
    x = np.transpose(x, (0, 3, 1, 2, 4))  # N, C1, H, W, C0
    return np.ascontiguousarray(x)


def gen_golden_data():
    fmap = np.random.uniform(-1, 1, (BATCH, H, W, CIN)).astype(np.float16)
    weight = np.random.uniform(-1, 1, (COUT, CIN, KH, KW)).astype(np.float16)

    golden = conv2d_nhwc(fmap, weight, STRIDE, DILATION, PADDING, PAD_VALUE).astype(
        np.float16
    )

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    to_nc1hwc0(fmap, C1).reshape(-1).tofile("./input/x1_gm.bin")
    weight_to_nd(weight).reshape(-1).tofile("./input/x2_gm.bin")
    to_nc1hwc0(golden, COUT // C0).reshape(-1).tofile("./output/golden.bin")


if __name__ == "__main__":
    gen_golden_data()
