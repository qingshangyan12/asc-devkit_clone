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
C1 = 1
H = 4
W = 4
C0 = 16
CIN = C1 * C0
COUT = 16
KH = 1
KW = 1
STRIDE = (1, 1)
DILATION = (1, 1)
PADDING = (0, 0, 0, 0)  # top, bottom, left, right


def conv2d_nhwc(x, weight, stride, dilation, padding):
    stride_h, stride_w = stride
    dilation_h, dilation_w = dilation
    pad_top, pad_bottom, pad_left, pad_right = padding
    n, h, w, cin = x.shape
    cout, _, kh, kw = weight.shape
    hout = (h + pad_top + pad_bottom - dilation_h * (kh - 1) - 1) // stride_h + 1
    wout = (w + pad_left + pad_right - dilation_w * (kw - 1) - 1) // stride_w + 1
    x_pad = np.pad(x, ((0, 0), (pad_top, pad_bottom), (pad_left, pad_right), (0, 0)), mode="constant")
    y = np.zeros((n, hout, wout, cout), dtype=np.float32)

    for n_idx in range(n):
        for ho in range(hout):
            for wo in range(wout):
                for co in range(cout):
                    acc = 0.0
                    for ci in range(cin):
                        for hk in range(kh):
                            for wk in range(kw):
                                hi = ho * stride_h + hk * dilation_h
                                wi = wo * stride_w + wk * dilation_w
                                acc += float(x_pad[n_idx, hi, wi, ci]) * float(weight[co, ci, hk, wk])
                    y[n_idx, ho, wo, co] = acc
    return y


def gen_golden_data():
    fmap = np.random.uniform(-1, 1, (BATCH, H, W, CIN)).astype(np.float16)
    weight = np.random.uniform(-1, 1, (COUT, CIN, KH, KW)).astype(np.float16)
    golden = conv2d_nhwc(fmap, weight, STRIDE, DILATION, PADDING).astype(np.float16)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    fmap.tofile("./input/x1_gm.bin")
    weight.tofile("./input/x2_gm.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    gen_golden_data()
