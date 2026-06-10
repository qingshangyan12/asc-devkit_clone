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

# SCENARIO_NUM 选择 GM 输入布局，与 kernel/CMake 的同名宏保持一致：
#   1（默认）：FeatureMap=NHWC、Weight=[K,Cout]，kernel 侧走 Nd2Nz。
#   2：FeatureMap=CHW([CIN,H*W])、Weight=[Cout,K]，kernel 侧走 Dn2Nz（仅 dav-3510）。
SCENARIO_NUM = int(os.environ.get("SCENARIO_NUM", "1"))

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
    # 经探针实测：dav-3510 img2col 在 A2 的 K 轴顺序为 (C1, KH, KW, C0)，
    # 即 C1 在最外层、C0 在最内层：k = ((c1*KH + hk)*KW + wk)*C0 + c0。
    # 输出 [K, Cout] 行优先，经 kernel 侧 Nd2Nz -> L1 [K, Cout] NZ -> LoadData2D 转置 -> L0B 与 img2col 对齐。
    # 使用 Nd2Nz（dav-2201/dav-3510 均支持），统一两个架构的权重搬运路径。
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
    # 场景2：与 weight_to_k_cout 相同的 K 序 (C1,KH,KW,C0)，但输出 [Cout, K] 行优先，
    # 供 kernel 侧 Dn2Nz(dValue=COUT, nValue=K) -> L1 [K, Cout] NZ -> LoadData2D 转置 -> L0B。
    cout, cin, kh, kw = weight.shape
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


def gen_golden_data():
    fmap = np.random.uniform(-1, 1, (BATCH, H, W, CIN)).astype(np.float16)
    weight = np.random.uniform(-1, 1, (COUT, CIN, KH, KW)).astype(np.float16)
    # 场景4：pad填充值为1（显式SetLoadDataPaddingValue），golden需用相同的constant_values；其余场景填充0。
    pad_value = 1 if SCENARIO_NUM == 4 else 0
    golden = conv2d_nhwc(fmap, weight, STRIDE, DILATION, PADDING, pad_value).astype(np.float16)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    if SCENARIO_NUM in (1, 3, 4):
        # 场景1/3/4：FeatureMap 以 NHWC 写盘；Weight 以 [K, Cout] 写盘。kernel 侧走 Nd2Nz。
        # 场景3仅img2col的repeat方式不同、场景4仅fmatrix/pad改用显式接口设置，输入布局均与场景1相同。
        fmap.tofile("./input/x1_gm.bin")
        weight_to_k_cout(weight).tofile("./input/x2_gm.bin")
    else:
        # 场景2：FeatureMap 以 CHW([CIN, H*W]/batch) 通道优先写盘；Weight 以 [Cout, K] 写盘。kernel 侧走 Dn2Nz。
        fmap_chw = np.transpose(fmap, (0, 3, 1, 2)).reshape(BATCH, CIN, H * W)
        fmap_chw.astype(np.float16).tofile("./input/x1_gm.bin")
        weight_to_cout_k(weight).tofile("./input/x2_gm.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    gen_golden_data()
