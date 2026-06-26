#!/usr/bin/env python3
"""
ReduceSum 样例数据生成: RA pattern, float32, R=60
生成 [60,31] 和 [60,64] 两种输入的测试数据
"""
import numpy as np
import os, sys

def gen_data(r_length, a_length, out_dir):
    os.makedirs(out_dir, exist_ok=True)

    # 输入: shape = [r_length, a_length], RA 归约 axis=0
    x_extent = r_length * a_length
    y_extent = a_length

    np.random.seed(42)
    x_gm = np.random.uniform(-10, 10, x_extent).reshape(r_length, a_length).astype(np.float32)
    x_gm.tofile(os.path.join(out_dir, "input_x.bin"))

    # Golden: np.sum along axis=0
    y_gm = np.sum(x_gm, axis=0).astype(np.float32)
    y_gm.tofile(os.path.join(out_dir, "golden.bin"))

    print(f"[{r_length},{a_length}] x_extent={x_extent}, y_extent={y_extent}")
    print(f"  input_x.bin  : {x_extent * 4} bytes")
    print(f"  golden.bin   : {y_extent * 4} bytes")
    print(f"  input[0,:5]  : {x_gm[0, :5]}")
    print(f"  golden[:5]   : {y_gm[:5]}")

if __name__ == "__main__":
    gen_data(60, 31, "./input")
