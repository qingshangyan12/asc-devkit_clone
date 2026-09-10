# ELU gen_data.py
import numpy as np
import os

def elu(x, alpha=1.0, scale=1.0, input_scale=1.0):
    """ELU 激活函数"""
    return np.where(x > 0, scale * x, alpha * scale * (np.exp(x * input_scale) - 1))

# 测试 shape 列表（任务书要求）
shapes = [(1,), (32,), (1024,), (2048,)]

alpha = 1.0
scale = 1.0
input_scale = 1.0

# 创建输入输出目录
os.makedirs('./input', exist_ok=True)
os.makedirs('./output', exist_ok=True)

# ========== fp32 数据 ==========
for shape in shapes:
    x = np.random.uniform(-100, 100, shape).astype(np.float32)
    golden = elu(x, alpha, scale, input_scale)

    x.tofile(f'./input/input_x_{"x".join(map(str, shape))}.bin')
    golden.tofile(f'./output/golden_{"x".join(map(str, shape))}.bin')

print("fp32 data generated for shapes:", shapes)

# ========== fp16 数据 ==========
for shape in shapes:
    x = np.random.uniform(-100, 100, shape).astype(np.float16)
    golden = elu(x.astype(np.float32), alpha, scale, input_scale).astype(np.float16)

    x.tofile(f'./input/input_x_fp16_{"x".join(map(str, shape))}.bin')
    golden.tofile(f'./output/golden_fp16_{"x".join(map(str, shape))}.bin')

print("fp16 data generated for shapes:", shapes)