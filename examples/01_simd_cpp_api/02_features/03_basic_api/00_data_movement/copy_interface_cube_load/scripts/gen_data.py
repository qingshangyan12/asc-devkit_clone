#!/usr/bin/env python3
import os
import numpy as np

D = 32
os.makedirs("input", exist_ok=True)
os.makedirs("output", exist_ok=True)
matrix_src = ((np.arange(D * D).reshape(D, D) % 13) - 6).astype(np.float16)
matrix_initial = np.full((D, D), -9, dtype=np.float16)
identity = np.eye(D, dtype=np.float16)
zero = np.zeros((D, D), dtype=np.float16)
bias_src = (np.arange(D, dtype=np.float32) / 4 - 3).astype(np.float32)
bias_initial = np.full(D, -7, dtype=np.float32)
matrix_src.tofile("input/matrix_src.bin")
matrix_initial.tofile("input/matrix_initial.bin")
matrix_a_src = matrix_src.copy()
matrix_a_initial = matrix_initial.copy()
matrix_b_src = matrix_src.T.copy()
matrix_b_initial = matrix_initial.T.copy()
matrix_a_src.tofile("input/matrix_a_src.bin")
matrix_a_initial.tofile("input/matrix_a_initial.bin")
matrix_b_src.tofile("input/matrix_b_src.bin")
matrix_b_initial.tofile("input/matrix_b_initial.bin")
identity.tofile("input/identity.bin")
zero.tofile("input/zero.bin")
bias_src.tofile("input/bias_src.bin")
bias_initial.tofile("input/bias_initial.bin")
