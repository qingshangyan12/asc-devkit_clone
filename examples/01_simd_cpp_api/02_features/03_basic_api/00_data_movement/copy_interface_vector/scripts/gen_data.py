#!/usr/bin/env python3
import os
import numpy as np

ROWS, COLS = 32, 64
src = ((np.arange(ROWS * COLS, dtype=np.int32) % 101) - 50).astype(np.int8).reshape(ROWS, COLS)
initial = np.full((ROWS, COLS), -111, dtype=np.int8)
os.makedirs("input", exist_ok=True)
os.makedirs("output", exist_ok=True)
src.tofile("input/input.bin")
initial.tofile("input/initial.bin")
src.reshape(ROWS, COLS // 32, 32).transpose(1, 0, 2).tofile("input/input_nz.bin")
initial.reshape(ROWS, COLS // 32, 32).transpose(1, 0, 2).tofile("input/initial_nz.bin")
np.eye(COLS, dtype=np.int8).tofile("input/probe.bin")
