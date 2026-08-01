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

BATCH = 4
C0 = 16
C1 = 2
CIN = C1 * C0  # 32
D = 2          # 输入深度，同时也是 Kd
H = 9
W = 9
COUT = 16
KD = D         # 深度方向卷积核尺寸 == D，一次覆盖，输出深度 DOUT = 1
KH = 3
KW = 3
DOUT = 1
STRIDE = (2, 2)
DILATION = (2, 2)
PADDING = (2, 2, 2, 2)  # top, bottom, left, right
PAD_VALUE = 1


def conv3d_ncdhw(x, weight, stride, dilation, padding, pad_value=0):
    """x: [N, C, D, H, W]，weight: [Cout, Cin, KD, KH, KW]。
    深度方向不 padding、不滑窗（KD == D），因此输出深度为 1。
    H/W 方向按 stride/dilation/padding 常规滑窗。
    """
    stride_h, stride_w = stride
    dilation_h, dilation_w = dilation
    pad_top, pad_bottom, pad_left, pad_right = padding
    n, cin, d, h, w = x.shape
    cout, _, kd, kh, kw = weight.shape
    assert kd == d, "本样例要求 KD == D（深度方向一次覆盖）"
    hout = (h + pad_top + pad_bottom - dilation_h * (kh - 1) - 1) // stride_h + 1
    wout = (w + pad_left + pad_right - dilation_w * (kw - 1) - 1) // stride_w + 1
    # 只在 H/W 方向 pad
    x_pad = np.pad(
        x,
        ((0, 0), (0, 0), (0, 0), (pad_top, pad_bottom), (pad_left, pad_right)),
        mode="constant",
        constant_values=pad_value,
    )
    y = np.zeros((n, cout, DOUT, hout, wout), dtype=np.float32)

    for n_idx in range(n):
        for ho in range(hout):
            for wo in range(wout):
                for co in range(cout):
                    acc = 0.0
                    for dk in range(kd):
                        for ci in range(cin):
                            for hk in range(kh):
                                for wk in range(kw):
                                    hi = ho * stride_h + hk * dilation_h
                                    wi = wo * stride_w + wk * dilation_w
                                    acc += float(x_pad[n_idx, ci, dk, hi, wi]) * float(
                                        weight[co, ci, dk, hk, wk]
                                    )
                    y[n_idx, co, 0, ho, wo] = acc
    return y


def weight_to_k_cout(weight):
    """weight: [Cout, Cin, KD, KH, KW]，Cin = C1 * C0。
    img2col 把 D 与 C1 合入 channel 轴（channelSize = D*C1*C0），合轴后的 channel 索引为
    d * (C1*C0) + c1 * C0 + c0，因此 K 序 = (D, C1, KH, KW, C0)：
        k_idx = (((d * C1 + c1) * KH + hk) * KW + wk) * C0 + c0
    """
    cout, cin, kd, kh, kw = weight.shape
    k = kd * cin * kh * kw
    out = np.zeros((k, cout), dtype=np.float16)
    for co in range(cout):
        for dk in range(kd):
            for c1 in range(C1):
                for c0 in range(C0):
                    ci = c1 * C0 + c0
                    for hk in range(kh):
                        for wk in range(kw):
                            k_idx = (((dk * C1 + c1) * kh + hk) * kw + wk) * C0 + c0
                            out[k_idx, co] = weight[co, ci, dk, hk, wk]
    return out.reshape(-1)


def gen_golden_data():
    # fmap NCDHW: [N, C, D, H, W]
    fmap = np.random.uniform(-1, 1, (BATCH, CIN, D, H, W)).astype(np.float16)
    # weight: [Cout, Cin, KD, KH, KW]
    weight = np.random.uniform(-1, 1, (COUT, CIN, KD, KH, KW)).astype(np.float16)

    golden = conv3d_ncdhw(fmap, weight, STRIDE, DILATION, PADDING, PAD_VALUE).astype(
        np.float16
    )

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    # fmap GM 为 NCDHW 行优先连续。
    np.ascontiguousarray(fmap).reshape(-1).tofile("./input/x1_gm.bin")
    # weight GM 按 [K, Cout] 行优先（K 序 = (D, C1, KH, KW, C0)）。
    weight_to_k_cout(weight).tofile("./input/x2_gm.bin")
    # golden GM 为 NCDHW 行优先连续，输出 (N, COUT, DOUT, HOUT, WOUT)。
    np.ascontiguousarray(golden).reshape(-1).tofile("./output/golden.bin")


if __name__ == "__main__":
    gen_golden_data()
