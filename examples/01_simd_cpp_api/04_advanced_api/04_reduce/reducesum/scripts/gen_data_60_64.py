#!/usr/bin/env python3
"""生成 [60,64] 的输入和 golden 数据"""
import numpy as np, os

r_length, a_length = 60, 64
x_extent = r_length * a_length
y_extent = a_length

os.makedirs("./input", exist_ok=True)

np.random.seed(42)
x_gm = np.random.uniform(-10, 10, x_extent).reshape(r_length, a_length).astype(np.float32)
x_gm.tofile("./input/input_x.bin")

y_gm = np.sum(x_gm, axis=0).astype(np.float32)
y_gm.tofile("./output/golden.bin")

print(f"[{r_length},{a_length}] x_extent={x_extent}, y_extent={y_extent}")
print(f"  golden[:5]: {y_gm[:5]}")
