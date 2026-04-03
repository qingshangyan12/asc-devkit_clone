import os
import json
import numpy as np

from common import cpp_to_py_dtype

def nd_2_nz(input, is_tans):
    row, col = input.shape
    assert (row % 16) == 0, "row should be 16 aligned when matrix is NZ format"
    assert (col % 16) == 0, "col should be 16 aligned when matrix is NZ format"
    c0Size = 16
    if input.dtype in (np.float32, np.int32):
        c0Size = 8
    elif input.dtype == np.int8:
        c0Size = 32
    
    if is_tans:
        input = input.reshape((int(col / 16), 16, int(row / c0Size), c0Size)).transpose(2, 0, 1, 3)
    else:
        input = input.reshape((int(row / 16), 16, int(col / c0Size), c0Size)).transpose(2, 0, 1, 3)

def saturation(value, min_val, max_val):
    x_clamped = np.clip(value, min_val, max_val)
    return np.round(x_clamped)

def gen_golden(testcase, output_dir):
    M, N, K = testcase["shape"]
    a_format, b_format, c_format, _ = testcase["format"]
    a_type, b_type, c_type, bias_dtype = [
        cpp_to_py_dtype[dtype] for dtype in testcase["dtype"]
    ]

    a_cpp_dtype, b_cpp_dtype, c_cpp_dtype, _ = [
        dtype for dtype in testcase["dtype"]
    ]

    a_is_tans, b_is_tans = testcase["is_transpose"]
    is_bias = testcase["is_bias"]

    input_shape_x = [M, K]
    input_shape_y = [K, N]

    input_x = np.random.uniform(-2, 2, input_shape_x).astype(a_type)
    input_y = np.random.uniform(-2, 2, input_shape_y).astype(b_type)
    # input_x = np.ones(input_shape_x).astype(a_type)
    # input_y = np.ones(input_shape_y).astype(b_type)
    input_bias = np.random.uniform(-2, 2, [1, N]).astype(bias_dtype)
    # input_bias = np.ones([1, N]).astype(bias_dtype)

    golden = np.matmul(input_x.astype(np.float32), input_y.astype(np.float32))
    if is_bias:
        golden += input_bias.astype(np.float32)
    
    temp_quant_tensor = np.random.uniform(low=-2, high = 2, size=N).astype(np.float32)
    uint32_deq_scale = np.frombuffer(temp_quant_tensor, np.uint32)

    uint32_deq_scale &= 0xFFFFE000
    quant_tensor = uint32_deq_scale.astype(np.uint64)
    quant_tensor |= 1 << 46

    if c_type in (np.half, np.int8):
        golden = golden * temp_quant_tensor

    if c_type == np.int8:
        golden = saturation(golden, -128, 127)
    
    golden = golden.astype(c_type)

    if a_format == "DN":
        input_x = input_x.transpose()
    if b_format == "DN":
        input_y = input_y.transpose()
    if c_format == "DN":
        golden = golden.transpose()

    if a_is_tans:
        input_x = input_x.transpose()
    if b_is_tans:
        input_y = input_y.transpose()

    if a_format == "NZ":
        nd_2_nz(input_x, a_is_tans)
    if b_format == "NZ":
        nd_2_nz(input_y, b_is_tans)
    if c_format == "NZ":
        assert (M % 16) == 0, "M should be 16 aligned when matrix C is NZ format"
        assert (N % 16) == 0, "N should be 16 aligned when matrix C is NZ format"
        golden = golden.reshape((int(M / 16), 16, int(N / 16), 16)).transpose(2, 0, 1, 3)

    os.makedirs(output_dir, exist_ok=True)
    input_x.tofile(os.path.join(output_dir, "input_x.bin"))
    input_y.tofile(os.path.join(output_dir, "input_y.bin"))
    input_bias.tofile(os.path.join(output_dir, "input_bias.bin"))
    quant_tensor.tofile(os.path.join(output_dir, "input_quant.bin"))
    golden.tofile(os.path.join(output_dir, "golden.bin"))