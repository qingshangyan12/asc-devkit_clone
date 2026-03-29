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
    construct_special_values,
    save_bin
)

NUM_CORES = 3
BLOCK_LENGTH = 256


def compute_golden(input_val, data_type, saturation=True):
    with np.errstate(over='ignore', under='ignore', invalid='ignore'):
        result = input_val * NUM_CORES
    limits = get_dtype_limits(data_type)
    
    if np.issubdtype(data_type, np.floating):
        if saturation and not np.isnan(result) and not np.isinf(result) and result != 0:
            if result > limits['max']:
                result = limits['max']
            elif result < limits['min']:
                result = limits['min']
        elif saturation and np.isnan(result):
            result = 0.0
        elif saturation and np.isinf(result):
            if result > limits['max']:
                result = limits['max']
            elif result < limits['min']:
                result = limits['min']
    else:
        if saturation:
            if result > limits['max']:
                result = limits['max']
            elif result < limits['min']:
                result = limits['min']
    
    return result


def gen_data():
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    
    print("Generating input and golden data...")
    
    for dtype_name in DTYPE_MAP.keys():
        data_type = DTYPE_MAP[dtype_name]
        
        special_vals = construct_special_values(data_type)
        num_special = len(special_vals)
        
        repeat = (BLOCK_LENGTH + num_special - 1) // num_special
        
        input_data = np.array(special_vals * repeat, dtype=data_type)[:BLOCK_LENGTH]
        
        golden_sat = np.array([compute_golden(v, data_type, True) for v in special_vals] * repeat, dtype=data_type)[:BLOCK_LENGTH]
        with np.errstate(over='ignore', under='ignore', invalid='ignore'):
            golden_non_sat = np.array([compute_golden(v, data_type, False) for v in special_vals] * repeat, dtype=data_type)[:BLOCK_LENGTH]
        
        save_bin(input_data, f"./input/input_x_{dtype_name}.bin")
        save_bin(golden_sat, f"./output/golden_sat_{dtype_name}.bin")
        save_bin(golden_non_sat, f"./output/golden_non_sat_{dtype_name}.bin")
        
        print(f"  {dtype_name}: OK")
    
    print("Done!")


if __name__ == "__main__":
    gen_data()
