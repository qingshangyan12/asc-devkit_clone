#!/usr/bin/python3
# coding=utf-8
import os
import numpy as np

np.random.seed(42)

# Convolution parameters (matching the .asc kernel)
BATCH = 4
C0 = 16
C1 = 32
CIN = C1 * C0            # 512
H = 16
W = 96
COUT = 512
COUT_C1 = COUT // C0    # 32
KH = 3
KW = 3
STRIDE_H = 1
STRIDE_W = 1
DILATION_H = 1
DILATION_W = 1
PAD_TOP = 1
PAD_BOTTOM = 1
PAD_LEFT = 1
PAD_RIGHT = 1
PAD_VALUE = 0

HOUT = (H + PAD_TOP + PAD_BOTTOM - DILATION_H * (KH - 1) - 1) // STRIDE_H + 1  # 16
WOUT = (W + PAD_LEFT + PAD_RIGHT - DILATION_W * (KW - 1) - 1) // STRIDE_W + 1  # 96


def conv2d_nhwc(x, weight, stride_h, stride_w, dilation_h, dilation_w,
                pad_top, pad_bottom, pad_left, pad_right, pad_value=0):
    """Reference conv2d in NHWC layout."""
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
                        for hk_idx in range(kh):
                            for wk_idx in range(kw):
                                hi = ho * stride_h + hk_idx * dilation_h
                                wi = wo * stride_w + wk_idx * dilation_w
                                acc += float(x_pad[n_idx, hi, wi, ci]) * float(weight[co, ci, hk_idx, wk_idx])
                    y[n_idx, ho, wo, co] = acc
    return y


def weight_to_k_cout(weight, c1, c0, kh, kw):
    """Rearrange weight [Cout, Cin, KH, KW] to [K, Cout] row-major.
    K order = (C1, KH, KW, C0), matching img2col hardware unfolding order.
    """
    cout, cin, kh_, kw_ = weight.shape
    k = cin * kh_ * kw_
    out = np.zeros((k, cout), dtype=np.float16)
    for co in range(cout):
        for c1_idx in range(c1):
            for c0_idx in range(c0):
                ci = c1_idx * c0 + c0_idx
                for hk_idx in range(kh_):
                    for wk_idx in range(kw_):
                        k_idx = ((c1_idx * kh_ + hk_idx) * kw_ + wk_idx) * c0 + c0_idx
                        out[k_idx, co] = weight[co, ci, hk_idx, wk_idx]
    return out.reshape(-1)


def nhwc_to_nc1hwc0(x_nhwc, c1):
    """Convert NHWC [N, H, W, C] to NC1HWC0 flat array."""
    n, h, w, c = x_nhwc.shape
    c0 = c // c1
    x = x_nhwc.reshape(n, h, w, c1, c0)
    x = np.transpose(x, (0, 3, 1, 2, 4))  # [N, C1, H, W, C0]
    return np.ascontiguousarray(x).reshape(-1)


def gen_data():
    print(f"Generating conv2d data: N={BATCH}, CIN={CIN}, COUT={COUT}, H={H}, W={W}, KH={KH}, KW={KW}")
    print(f"HOUT={HOUT}, WOUT={WOUT}, stride=({STRIDE_H},{STRIDE_W}), pad=({PAD_TOP},{PAD_BOTTOM},{PAD_LEFT},{PAD_RIGHT})")

    # Generate random input data in NHWC layout
    fmap = np.random.uniform(-0.5, 0.5, (BATCH, H, W, CIN)).astype(np.float16)
    weight = np.random.uniform(-0.5, 0.5, (COUT, CIN, KH, KW)).astype(np.float16)

    # Compute golden output using reference convolution
    print("Computing golden output (reference conv2d)...")
    golden = conv2d_nhwc(fmap, weight, STRIDE_H, STRIDE_W, DILATION_H, DILATION_W,
                         PAD_TOP, PAD_BOTTOM, PAD_LEFT, PAD_RIGHT, PAD_VALUE)
    golden = golden.astype(np.float16)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    # Save fmap in NC1HWC0 format
    fmap_nc1hwc0 = nhwc_to_nc1hwc0(fmap, C1)
    fmap_nc1hwc0.tofile("./input/x1_gm.bin")
    print(f"  x1_gm.bin (fmap NC1HWC0): {fmap_nc1hwc0.size} elements, {fmap_nc1hwc0.nbytes} bytes")

    # Save weight in [K, Cout] row-major (K order = C1, KH, KW, C0)
    weight_k_cout = weight_to_k_cout(weight, C1, C0, KH, KW)
    weight_k_cout.tofile("./input/x2_gm.bin")
    print(f"  x2_gm.bin (weight [K,Cout]): {weight_k_cout.size} elements, {weight_k_cout.nbytes} bytes")

    # Save golden in NC1HWC0 format
    golden_nc1hwc0 = nhwc_to_nc1hwc0(golden, COUT_C1)
    golden_nc1hwc0.tofile("./output/golden.bin")
    print(f"  golden.bin (NC1HWC0): {golden_nc1hwc0.size} elements, {golden_nc1hwc0.nbytes} bytes")


if __name__ == "__main__":
    gen_data()
