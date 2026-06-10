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
import argparse
import numpy as np


np.random.seed(19)

BATCH = 4
C0 = 16
C1 = 2
CIN = C1 * C0            # 32
H = 9
W = 9
COUT = 16
KH = 3
KW = 3
STRIDE = (2, 2)
DILATION = (2, 2)
PADDING = (2, 2, 2, 2)   # top, bottom, left, right

CUBE_BLOCK = 16


def ceil_align(value, factor):
    return (value + factor - 1) // factor * factor


def conv2d_nhwc(x, weight, stride, dilation, padding, pad_value=0):
    stride_h, stride_w = stride
    dilation_h, dilation_w = dilation
    pad_top, pad_bottom, pad_left, pad_right = padding
    n, h, w, cin = x.shape
    cout, _, kh, kw = weight.shape
    hout = (h + pad_top + pad_bottom - dilation_h * (kh - 1) - 1) // stride_h + 1
    wout = (w + pad_left + pad_right - dilation_w * (kw - 1) - 1) // stride_w + 1
    x_pad = np.pad(x, ((0, 0), (pad_top, pad_bottom), (pad_left, pad_right), (0, 0)),
                   mode="constant", constant_values=pad_value)
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


def weight_to_k_cout(weight):
    # weight: [Cout, Cin, KH, KW]，Cin = C1 * C0
    cout, cin, kh, kw = weight.shape
    k = cin * kh * kw
    out = np.zeros((k, cout), dtype=np.float16)
    for co in range(cout):
        for c1 in range(C1):
            for c0 in range(C0):
                ci = c1 * C0 + c0
                for hk in range(kh):
                    for wk in range(kw):
                        k_idx = ((c1 * kh + hk) * kw + wk) * C0 + c0
                        out[k_idx, co] = weight[co, ci, hk, wk]
    return out.reshape(-1)


def weight_to_cout_k(weight):
    # 场景4：与 weight_to_k_cout 相同的 K 序 (C1,KH,KW,C0)，但输出 [Cout, K] 行优先，供 Dn2Nz 使用。
    cout, cin, kh, kw = .shape
    k = cin * kh * kw
    out = np.zeros((cout, k), dtype=np.float16)
    for co in range(cout):
        for c1 in range(C1):
            for c0 in range(C0):
                ci = c1 * C0 + c0
                for hk in range(kh):
                    for wk in range(kw):
                        k_idx = ((c1 * kh + hk) * kw + wk) * C0 + c0
                        out[co, k_idx] = weight[co, ci, hk, wk]
    return out.reshape(-1)


def gen_golden_data(scenario_num):
    fmap = np.random.uniform(-1, 1, (BATCH, H, W, CIN)).astype(np.float16)
    weight = np.random.uniform(-1, 1, (COUT, CIN, KH, KW)).astype(np.float16)
    # 场景2：pad填充值为1（显式SetLoadDataPaddingValue），golden需用相同的constant_values；其余场景填充0。
    pad_value = 1 if scenario_num == 2 else 0
    golden = conv2d_nhwc(fmap, weight, STRIDE, DILATION, PADDING, pad_value).astype(np.float16)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    if scenario_num in (1, 2, 3):
        # 场景1/2/3：FeatureMap 为 NHWC；Weight 为 [K, Cout]。
        # 场景2仅fmatrix/pad改用显式接口设置、场景3仅img2col沿K方向分块repeat，输入布局均与场景1相同。
        fmap.tofile("./input/x1_gm.bin")
        weight_to_k_cout(weight).tofile("./input/x2_gm.bin")
    else:
        # 场景4：FeatureMap 为 NCHW([CIN, H*W]/batch) ；Weight 为 [Cout, K] 写盘。
        fmap_chw = np.transpose(fmap, (0, 3, 1, 2)).reshape(BATCH, CIN, H * W)
        fmap_chw.astype(np.float16).tofile("./input/x1_gm.bin")
        weight_to_cout_k(weight).tofile("./input/x2_gm.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    #   1：Nd2Nz 单次img2col；2：Nd2Nz K方向repeat；3：显式SetFmatrix/SetLoadDataPaddingValue；4：Dn2Nz(仅dav-3510)。
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 5))
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum)
