#!/usr/bin/python3
# coding=utf-8

import argparse
import os
import numpy as np
import ml_dtypes
import en_dtypes

fp4_e1m2 = en_dtypes.float4_e1m2


def pack_two_fp4(matrix_fp4: np.ndarray) -> np.ndarray:
    rows, cols = matrix_fp4.shape
    flat = matrix_fp4.flatten()
    high = flat[::2].view(np.uint8) & 0x0F
    low = (flat[1::2].view(np.uint8) & 0x0F) << 4
    packed = (low | high).reshape(rows, cols // 2)
    return packed
# cd build
# python3 ../scripts/gen_data_row_toggle.py \
#   --m 256 --n 256 --k 1024 \
#   --block-k 256 \
#   --a-pattern intra_block_cycle \
#   --block-values 0,0.5,1,1.5

# python3 ../scripts/gen_data_row_toggle.py --m 256 --n 256 --k 1024 --block-k 256 --a-pattern intra_block_cycle --block-values 0,0.5,1,1.5
# python3 -c "import numpy as np, ml_dtypes; x=np.fromfile('output/golden.bin',dtype=ml_dtypes.bfloat16).astype(np.float32); print(x[0], x[:8])"
# python3 -c "import numpy as np,en_dtypes;fp4=en_dtypes.float4_e1m2;v=np.array([0,0.5,1,1.5],dtype=np.float32).astype(fp4).astype(np.float32);print(v)"
# 
def gen_data(m: int, n: int, k: int, block_k: int, block_values, a_pattern: str) -> None:
    assert k % 2 == 0, "k must be even for fp4x2 packing"
    assert k % block_k == 0, "k must be divisible by block_k"

    # scaleK = align_even(ceil(k / 32))
    scale_k = int(np.ceil(k / 32.0))
    if scale_k % 2 != 0:
        scale_k += 1

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    # A patterns:
    # 1) block_const: each block_k uses one constant from block_values cyclically.
    # 2) intra_block_cycle: inside each block_k, values follow block_values cyclically.
    #    Example block_k=256, block_values=[0,0.5,1,1.5] => 64 cycles per block.
    a = np.zeros((m, k), dtype=np.float32)
    k_blocks = k // block_k
    block_values_np = np.array([float(v) for v in block_values], dtype=np.float32)
    if a_pattern == "block_const":
        for bi in range(k_blocks):
            v = float(block_values_np[bi % len(block_values_np)])
            a[:, bi * block_k:(bi + 1) * block_k] = v
    elif a_pattern == "intra_block_cycle":
        pattern_1d = np.resize(block_values_np, block_k)  # cycle values to fill one block
        for bi in range(k_blocks):
            a[:, bi * block_k:(bi + 1) * block_k] = pattern_1d
    else:
        raise ValueError(f"unsupported a_pattern: {a_pattern}")
    a = a.astype(fp4_e1m2)

    # B: all ones (before transpose), shape [k, n]
    b = np.ones((k, n), dtype=np.float32).astype(fp4_e1m2)

    # scaleA / scaleB all ones in mx-domain (exp=127 => 2^(127-127)=1)
    scale_a = np.full((m, scale_k), 127, dtype=np.uint8)
    scale_b = np.full((scale_k, n), 127, dtype=np.uint8)

    # golden compute in float64, then bf16
    a_full = np.zeros((m, k), dtype=np.float64)
    b_full = np.zeros((k, n), dtype=np.float64)

    a_mx = np.power(2.0, scale_a.astype(np.float64) - 127.0)
    b_mx = np.power(2.0, scale_b.astype(np.float64) - 127.0)

    for i in range(k):
        a_full[:, i] = a[:, i].astype(np.float64) * a_mx[:, i // 32]
        b_full[i, :] = b[i, :].astype(np.float64) * b_mx[i // 32, :]

    golden = np.matmul(a_full, b_full).astype(ml_dtypes.bfloat16)

    # Kernel input convention:
    # A packed from [M, K]
    # B uses pre-transposed storage [N, K], then packed
    b_t = b.transpose()           # [N, K]
    scale_b_t = scale_b.transpose()  # [N, scaleK]

    a_packed = pack_two_fp4(a)
    b_packed = pack_two_fp4(b_t)

    a_packed.tofile("./input/x1_gm.bin")
    b_packed.tofile("./input/x2_gm.bin")
    scale_a.tofile("./input/x1_scale_gm.bin")
    scale_b_t.tofile("./input/x2_scale_gm.bin")
    golden.tofile("./output/golden.bin")

    # IMPORTANT:
    # expected should be computed from quantized fp4 values (a_full), not from source float literals.
    # Different en_dtypes versions may quantize fp4_e1m2 differently.
    expected_row_value = float(a_full[0].sum()) if m > 0 else 0.0
    quantized_probe = np.array(block_values_np, dtype=np.float32).astype(fp4_e1m2).astype(np.float32).tolist()
    print(f"Generated shape: M={m}, N={n}, K={k}, scaleK={scale_k}, block_k={block_k}")
    print(f"Pattern: A mode={a_pattern}, values={block_values}, B all 1, scaleA/scaleB all 1")
    print(f"FP4 quantized probe values: {quantized_probe}")
    print(f"Expected: each output row should be constant value {expected_row_value} (before bf16 cast)")


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--m", type=int, default=256)
    parser.add_argument("--n", type=int, default=256)
    parser.add_argument("--k", type=int, default=1024)
    parser.add_argument("--block-k", type=int, default=256)
    parser.add_argument("--block-values", type=str, default="1,2,3,4")
    parser.add_argument("--a-pattern", type=str, default="block_const",
                        choices=["block_const", "intra_block_cycle"])
    args = parser.parse_args()
    block_values = [float(x) for x in args.block_values.split(",") if x.strip() != ""]
    if len(block_values) == 0:
        raise ValueError("block-values must contain at least one number")
    gen_data(args.m, args.n, args.k, args.block_k, block_values, args.a_pattern)


if __name__ == "__main__":
    main()
