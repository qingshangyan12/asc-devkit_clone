#!/usr/bin/python3
# coding=utf-8
"""Golden generator for the bessel_i0 sim check.

golden = scipy.special.i0 (computed in fp64, then cast to fp32 -- exactly the acceptance golden).
Sampling is shaped like the FUNCTION, not the interval: both A&S regions (|x|<=3.75 and |x|>3.75),
the branch seam at 3.75, the large-x overflow edge, negatives (evenness) and explicit specials.

Usage: gen_data.py <size> [mode]
   mode = mixed (default) | uniform   (uniform reproduces the shipped harness distribution: U(-15,15))
"""
import os
import sys
import numpy as np
from scipy.special import i0

F32 = np.float32
BP = 3.75


def make_mixed(size):
    """Function-shaped sample set, deterministic, exactly `size` elements."""
    rng = np.random.default_rng(0)
    parts = []
    # region1 body: |x| <= 3.75, where I0 ~ 1..9. Include both signs (I0 is even).
    parts.append(np.linspace(-BP, BP, max(size // 6, 1)))
    # near 0: I0 ~ 1, most sensitive to the region1 constant term
    parts.append(np.linspace(-0.5, 0.5, max(size // 8, 1)))
    # branch seam at 3.75: where the region1/region2 Select flips. Seam bugs live exactly here.
    d = np.concatenate([-np.logspace(-6, -1, max(size // 16, 1)),
                        np.logspace(-6, -1, max(size // 16, 1))])
    parts.append(BP + d)
    # region2 body, finite: 3.75 .. 20
    parts.append(np.linspace(BP, 20.0, max(size // 6, 1)))
    # large-x up to the fp32 overflow edge (~88.7 where exp(a) itself overflows; I0 finite to ~91.9)
    parts.append(np.linspace(20.0, 88.0, max(size // 8, 1)))
    # negatives at large magnitude (evenness under |x|)
    parts.append(-np.linspace(4.0, 88.0, max(size // 8, 1)))
    # uniform bulk = the range the shipped harness uses
    parts.append(rng.uniform(-15, 15, max(size // 8, 1)))
    # exact edges / specials -- the spec demands large-x + NaN/+-Inf handling explicitly.
    # +-Inf -> NaN and 100.0 -> +Inf (fp32 overflow) verify the boundary behaviour.
    parts.append(np.array([0.0, -0.0, 1.0, BP, 88.0, 92.0, 100.0, np.inf, -np.inf, np.nan]))

    x = np.concatenate(parts).astype(F32)
    if x.size >= size:
        if size >= 16:
            edges = x[-10:]
            body = x[:-10]
            idx = np.linspace(0, body.size - 1, size - 10).astype(int)
            x = np.concatenate([body[idx], edges]).astype(F32)
        else:
            idx = np.linspace(0, x.size - 1, size).astype(int)
            x = x[idx].astype(F32)
    else:
        pad = rng.uniform(-15, 15, size - x.size).astype(F32)
        x = np.concatenate([x, pad]).astype(F32)
    return x[:size]


def main():
    size = int(sys.argv[1]) if len(sys.argv) > 1 else 2048
    mode = sys.argv[2] if len(sys.argv) > 2 else "mixed"

    if mode == "uniform":
        x = np.random.default_rng(0).uniform(-15, 15, [size]).astype(F32)
    else:
        x = make_mixed(size)

    with np.errstate(invalid='ignore', divide='ignore', over='ignore'):
        golden = i0(x.astype(np.float64)).astype(F32)   # fp64 reference, cast to fp32 = the golden

    os.makedirs("./input", exist_ok=True)
    os.makedirs("./output", exist_ok=True)
    x.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")

    a = np.abs(x)
    finite = np.isfinite(golden)
    r2 = (a > BP) & finite
    print(f"[gen_data] mode={mode} size={size} | finite={finite.sum()} nonfinite={(~finite).sum()} | "
          f"region2 (|x|>3.75) pts={r2.sum()} ({100.0*r2.sum()/max(size,1):.1f}%) | "
          f"max|x|={float(a[np.isfinite(a)].max()):.1f}")
    if finite.any():
        print(f"[gen_data] I0 range over finite golden: [{golden[finite].min():.3f}, {golden[finite].max():.3e}]")


if __name__ == "__main__":
    main()
