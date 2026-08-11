#!/usr/bin/env python3
import argparse
import numpy as np

D = 32
p = argparse.ArgumentParser()
p.add_argument("--route", type=int, required=True)
p.add_argument("--scenario", type=int, required=True)
p.add_argument("--output", default="output/output.bin")
a = p.parse_args()

matrix_src = np.fromfile("input/matrix_src.bin", np.float16).reshape(D, D)
matrix_initial = np.fromfile("input/matrix_initial.bin", np.float16).reshape(D, D)
bias_src = np.fromfile("input/bias_src.bin", np.float32)
bias_initial = np.fromfile("input/bias_initial.bin", np.float32)

if a.route == 2:
    final_bias = bias_initial.copy()
    if a.scenario == 1:
        final_bias[:] = bias_src
    else:
        final_bias[16:32] = bias_src[8:24]
    golden = np.broadcast_to(final_bias, (D, D)).copy()
    actual = np.fromfile(a.output, np.float32).reshape(D, D)
elif a.route in (3, 4):
    final_matrix = matrix_initial.astype(np.float32)
    src_matrix = matrix_src.astype(np.float32)
    if a.scenario == 1:
        final_matrix[:] = src_matrix
    elif a.scenario == 0:
        final_matrix[:, 16:32] = src_matrix[:, 0:16]
    else:
        final_matrix[16:32, :] = src_matrix[0:16, :]
    golden = final_matrix
    actual = np.fromfile(a.output, np.float32).reshape(D, D)
else:
    golden = matrix_initial.copy()
    if a.scenario == 1:
        golden[:] = matrix_src
    else:
        golden[16:32, :] = matrix_src[0:16, :]
    actual = np.fromfile(a.output, np.float16)
    if a.route == 0:
        # NZ physical order is [N1, M, N0].
        golden = golden.reshape(D, D // 16, 16).transpose(1, 0, 2).reshape(-1)
    else:
        golden = golden.reshape(-1)

if not np.allclose(actual, golden, rtol=2e-3, atol=2e-3):
    idx = np.flatnonzero(~np.isclose(actual.reshape(-1), golden.reshape(-1), rtol=2e-3, atol=2e-3))[0]
    raise SystemExit(f"mismatch at flat index {idx}: expected {golden.reshape(-1)[idx]}, got {actual.reshape(-1)[idx]}")
print("test pass")
