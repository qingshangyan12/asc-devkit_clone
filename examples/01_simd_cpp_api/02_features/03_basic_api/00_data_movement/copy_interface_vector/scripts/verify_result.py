#!/usr/bin/env python3
import argparse
import numpy as np

ROWS, COLS = 32, 64
p = argparse.ArgumentParser()
p.add_argument("--route", type=int, required=True)
p.add_argument("--scenario", type=int, required=True)
p.add_argument("--output", default="output/output.bin")
a = p.parse_args()
src = np.fromfile("input/input.bin", np.int8).reshape(ROWS, COLS)
initial = np.fromfile("input/initial.bin", np.int8).reshape(ROWS, COLS)
golden = initial.copy()
if a.scenario == 1:
    golden[:] = src
elif a.route == 2:
    if a.scenario == 0:
        golden[0:32, 32:64] = src[0:32, 0:32]
    else:
        golden[16:32, 0:64] = src[0:16, 0:64]
else:
    golden[8:16, 0:32] = src[4:12, 0:32]
if a.route == 2:
    golden = golden.astype(np.int32)
    actual = np.fromfile(a.output, np.int32).reshape(ROWS, COLS)
else:
    actual = np.fromfile(a.output, np.int8).reshape(ROWS, COLS)
if not np.array_equal(actual, golden):
    idx = np.argwhere(actual != golden)[0]
    raise SystemExit(f"mismatch at {tuple(idx)}: expected {golden[tuple(idx)]}, got {actual[tuple(idx)]}")
print("test pass")
