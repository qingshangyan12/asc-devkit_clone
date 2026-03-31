#!/usr/bin python3
# coding=utf-8

import os
import sys
import numpy as np
import tensorflow as tf

bfloat16 = tf.bfloat16.as_numpy_dtype

DTYPE_MAP = {
    "int8_t": np.int8,
    "int16_t": np.int16,
    "half": np.float16,
    "bfloat16_t": bfloat16,
    "int32_t": np.int32,
    "float": np.float32
}

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from gen_data_utils import (
    get_dtype_limits,
    generate_two_inputs,
    save_bin
)

NUM_CORES = 3
BLOCK_LENGTH = 256


def compute_golden(input_a, input_b, data_type, saturation=True):
    with np.errstate(over='ignore', under='ignore', invalid='ignore'):
        result = input_a + input_b
    limits = get_dtype_limits(data_type)
    
    if np.issubdtype(data_type, np.floating):
        if saturation and not np.isnan(result):
            result = np.clip(result, limits['min'], limits['max'])
        elif saturation and np.isnan(result):
            result = 0.0
    else:
        if saturation:
            result = np.clip(result, limits['min'], limits['max'])
    
    return result


def gen_data():
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    
    print("Generating binary input and golden data...")
    
    for dtype_name in DTYPE_MAP.keys():
        data_type = DTYPE_MAP[dtype_name]
        
        inputs_a, inputs_b = generate_two_inputs(data_type)
        num_combinations = len(inputs_a)
        
        repeat = (BLOCK_LENGTH + num_combinations - 1) // num_combinations
        
        input_a_data = np.array(list(inputs_a) * repeat, dtype=data_type)[:BLOCK_LENGTH]
        input_b_data = np.array(list(inputs_b) * repeat, dtype=data_type)[:BLOCK_LENGTH]
        
        golden_sat = np.array([compute_golden(inputs_a[i], inputs_b[i], data_type, True) for i in range(num_combinations)] * repeat, dtype=data_type)[:BLOCK_LENGTH]
        with np.errstate(over='ignore', under='ignore', invalid='ignore'):
            golden_non_sat = np.array([compute_golden(inputs_a[i], inputs_b[i], data_type, False) for i in range(num_combinations)] * repeat, dtype=data_type)[:BLOCK_LENGTH]
        
        save_bin(input_a_data, f"./input/input_a_{dtype_name}.bin")
        save_bin(input_b_data, f"./input/input_b_{dtype_name}.bin")
        save_bin(golden_sat, f"./output/golden_sat_{dtype_name}.bin")
        save_bin(golden_non_sat, f"./output/golden_non_sat_{dtype_name}.bin")
        
        print(f"  {dtype_name}: {num_combinations} combinations, OK")
    
    print("Done!")


if __name__ == "__main__":
    gen_data()
