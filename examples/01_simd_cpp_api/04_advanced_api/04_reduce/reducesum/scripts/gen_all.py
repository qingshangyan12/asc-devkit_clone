#!/usr/bin/env python3
"""
ReduceSum 测试数据生成 (all cases)
用法:
  python gen_all.py          # 生成所有case
  python gen_all.py 60_31    # 仅生成 [60,31]
  python gen_all.py 60_64    # 仅生成 [60,64]
"""
import numpy as np, os, sys

def gen_data(r_length, a_length, case_name):
    in_dir = os.path.join("..", "input")
    out_dir = os.path.join("..", "output")
    os.makedirs(in_dir, exist_ok=True)
    os.makedirs(out_dir, exist_ok=True)

    x_extent = r_length * a_length
    y_extent = a_length

    np.random.seed(42)
    x_gm = np.random.uniform(-10, 10, x_extent).reshape(r_length, a_length).astype(np.float32)

    # 命名区分两个 case 的输入
    in_file = f"input_x_{case_name}.bin" if len(sys.argv) <= 1 else "input_x.bin"
    out_file = f"golden_{case_name}.bin" if len(sys.argv) <= 1 else "golden.bin"

    x_gm.tofile(os.path.join(in_dir, in_file))

    y_gm = np.sum(x_gm, axis=0).astype(np.float32)
    y_gm.tofile(os.path.join(out_dir, out_file))

    print(f"[{case_name}] shape=[{r_length},{a_length}], x={x_extent}, y={y_extent}")
    print(f"  golden[:5]: {y_gm[:5]}")

if __name__ == "__main__":
    case = sys.argv[1] if len(sys.argv) > 1 else ""
    if not case or case == "60_31":
        gen_data(60, 31, "60_31")
    if not case or case == "60_64":
        gen_data(60, 64, "60_64")
