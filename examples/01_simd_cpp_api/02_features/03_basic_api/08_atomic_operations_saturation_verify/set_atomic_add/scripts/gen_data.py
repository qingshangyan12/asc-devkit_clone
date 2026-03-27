#!/usr/bin python3
# coding=utf-8

import os
import sys
import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from gen_data_utils import (
    DTYPE_NAME_MAP,
    get_dtype_limits,
    construct_special_values,
    save_bin,
    get_special_names,
    print_unary_api_table
)

NUM_CORES = 3
BLOCK_LENGTH = 256


def compute_golden(input_val, data_type):
    result = input_val * NUM_CORES
    # limits = get_dtype_limits(data_type)
    
    # if np.issubdtype(data_type, np.floating):
    #     if not np.isnan(result) and not np.isinf(result) and result != 0:
    #         if result > limits['max']:
    #             result = limits['max']
    #         elif result < limits['min']:
    #             result = limits['min']
    # else:
    #     if result > limits['max']:
    #         result = limits['max']
    #     elif result < limits['min']:
    #         result = limits['min']
    
    return data_type(result)


def gen_test_data(dtype_name):
    data_type = DTYPE_NAME_MAP[dtype_name]
    
    special_vals = construct_special_values(data_type)
    special_names = get_special_names(data_type)
    
    num_special = len(special_vals)
    repeat = (BLOCK_LENGTH + num_special - 1) // num_special
    
    input_data = np.array(special_vals * repeat, dtype=data_type)[:BLOCK_LENGTH]
    golden_data = np.array([compute_golden(v, data_type) for v in special_vals] * repeat, dtype=data_type)[:BLOCK_LENGTH]
    
    return input_data, golden_data, special_names


def gen_all_data():
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    
    for dtype_name in DTYPE_NAME_MAP.keys():
        print(f"\nGenerating test data for {dtype_name}...")
        
        input_data, golden_data, special_names = gen_test_data(dtype_name)
        
        input_path = f"./input/input_x_{dtype_name}.bin"
        golden_path = f"./output/golden_{dtype_name}.bin"
        
        save_bin(input_data, input_path)
        save_bin(golden_data, golden_path)
        
        print(f"  Input: {input_path} ({input_data.nbytes} bytes)")
        print(f"  Golden: {golden_path} ({golden_data.nbytes} bytes)")
        
        type_name = np.dtype(DTYPE_NAME_MAP[dtype_name]).name
        print_unary_api_table("SetAtomicAdd", type_name, special_names, input_data, golden_data)


if __name__ == "__main__":
    gen_all_data()
    print("\nAll data generated successfully!")
