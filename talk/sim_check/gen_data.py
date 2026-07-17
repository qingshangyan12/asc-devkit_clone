#!/usr/bin/python3
# coding=utf-8
"""Golden generator for the ndtri sim check.

Differs from the shipped self_test gen_data.py in two ways:
  1. golden = scipy.special.ndtri (shipped one uses digamma as a placeholder)
  2. sampling is shaped like the FUNCTION, not the interval (talk/ndtri.md F9).
     uniform(0,1) puts ~95% of points in the central branch and NEVER samples p<1e-6,
     so it cannot see a broken tail. We interleave deep tail / tails / seams / edges.

Usage: gen_data.py <size> [mode]
   mode = mixed (default) | uniform   (uniform reproduces the shipped harness distribution)
"""
import os
import sys
import numpy as np
from scipy.special import ndtri

F32 = np.float32


def make_mixed(size):
    """Function-shaped sample set, deterministic, exactly `size` elements."""
    rng = np.random.default_rng(0)
    parts = []
    # deep lower tail: p down to ~1e-37 (z ~ -13). uniform sampling can NEVER reach here.
    parts.append(np.logspace(-37, -6, max(size // 8, 1)))
    # lower tail
    parts.append(np.logspace(-6, np.log10(0.02425), max(size // 8, 1)))
    # upper tail. fp32 ulp(1)=1.19e-7 -> smallest representable (1-p) is 2^-24=5.96e-8,
    # so the upper tail saturates at z~+5.4. The domain is ASYMMETRIC in fp32.
    parts.append(1.0 - np.logspace(np.log10(0.02425), -7.2, max(size // 8, 1)))
    # seams: where the Select mask flips (|q| vs 0.47575). Branch-seam bugs live here.
    for bp in (0.02425, 1 - 0.02425):
        d = np.concatenate([-np.logspace(-7, -3, max(size // 16, 1)),
                            np.logspace(-7, -3, max(size // 16, 1))])
        parts.append(np.clip(bp + d, 1e-38, 1 - 1e-7))
    # central + uniform bulk
    parts.append(np.linspace(0.02425, 1 - 0.02425, max(size // 8, 1)))
    parts.append(rng.uniform(0, 1, max(size // 8, 1)))
    # exact edges / specials -- the spec demands these explicitly
    parts.append(np.array([0.0, 1.0, 0.5, -0.1, 1.1, np.nan, np.inf, -np.inf]))

    p = np.concatenate(parts).astype(F32)
    if p.size >= size:
        if size >= 16:
            # keep the 8 edge cases: take them plus a strided sample of the rest
            edges = p[-8:]
            body = p[:-8]
            idx = np.linspace(0, body.size - 1, size - 8).astype(int)
            p = np.concatenate([body[idx], edges]).astype(F32)
        else:
            # too small to reserve the edge block (size-8 would go negative); stride the whole pool
            idx = np.linspace(0, p.size - 1, size).astype(int)
            p = p[idx].astype(F32)
    else:
        pad = rng.uniform(0, 1, size - p.size).astype(F32)
        p = np.concatenate([p, pad]).astype(F32)
    return p[:size]


def main():
    size = int(sys.argv[1]) if len(sys.argv) > 1 else 2048
    mode = sys.argv[2] if len(sys.argv) > 2 else "mixed"

    if mode == "uniform":
        p = np.random.default_rng(0).uniform(0, 1, [size]).astype(F32)
    else:
        p = make_mixed(size)

    with np.errstate(invalid='ignore', divide='ignore'):
        golden = ndtri(p.astype(np.float64)).astype(F32)

    os.makedirs("./input", exist_ok=True)
    os.makedirs("./output", exist_ok=True)
    p.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")

    finite = np.isfinite(golden)
    tail = ((p < 0.02425) | (p > 1 - 0.02425)) & finite
    print(f"[gen_data] mode={mode} size={size} | finite={finite.sum()} "
          f"nonfinite={(~finite).sum()} | tail-branch pts={tail.sum()} "
          f"({100.0*tail.sum()/max(size,1):.1f}%) | deep tail (p<1e-6)={int((p < 1e-6).sum())}")
    print(f"[gen_data] z range over finite golden: [{golden[finite].min():.3f}, {golden[finite].max():.3f}]")


if __name__ == "__main__":
    main()
