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

FLOAT_DTYPES = {"half", "bfloat16_t", "float"}

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from gen_data_utils import (
    DTYPE_NAME_MAP,
    get_dtype_limits,
    construct_special_values,
    get_special_names,
    save_bin
)

NUM_CORES = 3
BLOCK_LENGTH = 256
ERROR_TOL = 1e-4


def compute_golden(input_val, data_type, saturation=True):
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
    
    return data_type(result)


def format_value(val, np_type):
    if np.issubdtype(np_type, np.floating):
        return f"{float(val):.4e}"
    return str(val)


def gen_and_verify(dtype_name):
    data_type = DTYPE_MAP[dtype_name]
    
    special_vals = construct_special_values(data_type)
    special_names = get_special_names(data_type)
    num_special = len(special_vals)
    
    repeat = (BLOCK_LENGTH + num_special - 1) // num_special
    
    input_data = np.array(special_vals * repeat, dtype=data_type)[:BLOCK_LENGTH]
    
    input_path = f"./input/input_x_{dtype_name}.bin"
    save_bin(input_data, input_path)
    
    type_name = np.dtype(data_type).name
    
    print(f"\n{'='*120}")
    print(f" DataType: {type_name:<10} | BLOCK_LENGTH: {BLOCK_LENGTH} | NUM_CORES: {NUM_CORES}")
    print(f"{'='*120}")
    print(f" {'name':<8} | {'input':<14} | {'golden(sat)':<16} | {'golden(non-sat)':<16} | {'out(sat)':<16} | {'out(non-sat)':<16} | {'status'}")
    print(f"{'-'*120}")
    
    golden_sat = np.array([compute_golden(v, data_type, True) for v in special_vals] * repeat, dtype=data_type)[:BLOCK_LENGTH]
    golden_non_sat = np.array([compute_golden(v, data_type, False) for v in special_vals] * repeat, dtype=data_type)[:BLOCK_LENGTH]
    
    save_bin(golden_sat, f"./output/golden_sat_{dtype_name}.bin")
    save_bin(golden_non_sat, f"./output/golden_non_sat_{dtype_name}.bin")
    
    output_sat_path = f"./output/output_sat_{dtype_name}.bin"
    output_non_sat_path = f"./output/output_non_sat_{dtype_name}.bin"
    has_sat = os.path.exists(output_sat_path)
    has_non_sat = os.path.exists(output_non_sat_path)
    
    if has_sat:
        output_sat = np.fromfile(output_sat_path, dtype=data_type).reshape(-1)
    else:
        output_sat = golden_sat
    
    if has_non_sat:
        output_non_sat = np.fromfile(output_non_sat_path, dtype=data_type).reshape(-1)
    else:
        output_non_sat = golden_non_sat
    
    pass_count = 0
    for i in range(num_special):
        inp_val = input_data[i]
        golden_sat_val = golden_sat[i]
        golden_non_sat_val = golden_non_sat[i]
        output_sat_val = output_sat[i]
        output_non_sat_val = output_non_sat[i]
        name = special_names[i]
        
        is_match_sat = compare_values(golden_sat_val, output_sat_val, dtype_name)
        is_match_non_sat = compare_values(golden_non_sat_val, output_non_sat_val, dtype_name)
        
        if is_match_sat and is_match_non_sat:
            pass_count += 1
            status = "✓ PASS"
        elif not has_sat and not has_non_sat:
            status = "-"
        else:
            status = "✗ FAIL"
        
        inp_str = format_value(inp_val, data_type)
        gsat_str = format_value(golden_sat_val, data_type)
        gnsat_str = format_value(golden_non_sat_val, data_type)
        osat_str = format_value(output_sat_val, data_type) if has_sat else "-"
        onsat_str = format_value(output_non_sat_val, data_type) if has_non_sat else "-"
        
        print(f" {name:<8} | {inp_str:<14} | {gsat_str:<16} | {gnsat_str:<16} | {osat_str:<16} | {onsat_str:<16} | {status}")
    
    print(f"{'-'*120}")
    
    diff_sat = count_diff(golden_sat, output_sat, dtype_name) if has_sat else 0
    diff_non_sat = count_diff(golden_non_sat, output_non_sat, dtype_name) if has_non_sat else 0
    
    print(f" Summary: total={BLOCK_LENGTH}, special={num_special}, matched={pass_count}/{num_special}")
    print(f"   Saturation:     mismatches={diff_sat}")
    print(f"   Non-saturation: mismatches={diff_non_sat}")
    print(f"{'='*120}")
    
    result_sat = "PASS" if (diff_sat == 0 or not has_sat) else "FAIL"
    result_non_sat = "PASS" if (diff_non_sat == 0 or not has_non_sat) else "FAIL"
    print(f" Result: sat={result_sat}, non-sat={result_non_sat}")
    print(f"{'='*120}\n")
    
    return (result_sat == "PASS") and (result_non_sat == "PASS")


def compare_values(golden, output, dtype_name):
    if dtype_name in FLOAT_DTYPES:
        if np.isnan(golden):
            return np.isnan(output)
        elif np.isinf(golden):
            return np.isinf(output) and np.sign(output) == np.sign(golden)
        else:
            return np.isclose(float(output), float(golden), rtol=1e-6, atol=1e-9)
    else:
        return output == golden


def count_diff(golden, output, dtype_name):
    diff_count = 0
    for i in range(len(golden)):
        if not compare_values(golden[i], output[i], dtype_name):
            diff_count += 1
    return diff_count


def run_all():
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    
    all_pass = True
    for dtype_name in DTYPE_MAP.keys():
        if not gen_and_verify(dtype_name):
            all_pass = False
    
    return all_pass


if __name__ == "__main__":
    run_all()
    print("Done!")
