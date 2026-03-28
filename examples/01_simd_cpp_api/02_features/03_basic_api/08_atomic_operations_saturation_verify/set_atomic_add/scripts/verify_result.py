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
    get_dtype_limits,
    construct_special_values,
    get_special_names
)

NUM_CORES = 3
BLOCK_LENGTH = 256
ERROR_TOL = 1e-4

RESET = "\033[0m"
BOLD = "\033[1m"
GREEN = "\033[92m"
RED = "\033[91m"
YELLOW = "\033[93m"
BLUE = "\033[94m"
CYAN = "\033[96m"


def color_text(text, color):
    return f"{color}{text}{RESET}"


def format_value(val, np_type):
    if np.issubdtype(np_type, np.floating):
        return f"{float(val):.4e}"
    return str(val)


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


def verify_all():
    print(f"\n{BLUE}{'='*70}{RESET}")
    print(f"{BLUE}  Verify Output vs Golden{RESET}")
    print(f"{BLUE}{'='*70}{RESET}")
    
    all_pass = True
    
    for dtype_name in DTYPE_MAP.keys():
        data_type = DTYPE_MAP[dtype_name]
        
        special_vals = construct_special_values(data_type)
        special_names = get_special_names(data_type)
        num_special = len(special_vals)
        
        golden_sat_path = f"./output/golden_sat_{dtype_name}.bin"
        golden_non_sat_path = f"./output/golden_non_sat_{dtype_name}.bin"
        output_sat_path = f"./output/output_sat_{dtype_name}.bin"
        output_non_sat_path = f"./output/output_non_sat_{dtype_name}.bin"
        
        if not os.path.exists(golden_sat_path) or not os.path.exists(golden_non_sat_path):
            print(f"  {dtype_name}: golden data not found, skip")
            continue
        
        golden_sat = np.fromfile(golden_sat_path, dtype=data_type).reshape(-1)
        golden_non_sat = np.fromfile(golden_non_sat_path, dtype=data_type).reshape(-1)
        
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
        
        type_name = np.dtype(data_type).name
        
        print(f"\n{CYAN}[{type_name}]{RESET} BLOCK={BLOCK_LENGTH} CORES={NUM_CORES}")
        print(f"{'-'*70}")
        print(f" {'name':<8} | {'input':<14} | {'golden(sat)':<14} | {'golden(non-sat)':<14} | {'output(sat)':<14} | {'output(non-sat)':<14}")
        print(f"{'-'*70}")
        
        for i in range(num_special):
            inp_val = special_vals[i]
            golden_sat_val = golden_sat[i]
            golden_non_sat_val = golden_non_sat[i]
            output_sat_val = output_sat[i]
            output_non_sat_val = output_non_sat[i]
            name = special_names[i]
            
            inp_str = format_value(inp_val, data_type)
            gsat_str = format_value(golden_sat_val, data_type)
            gnsat_str = format_value(golden_non_sat_val, data_type)
            osat_str = format_value(output_sat_val, data_type) if has_sat else "-"
            onsat_str = format_value(output_non_sat_val, data_type) if has_non_sat else "-"
            
            print(f" {name:<8} | {inp_str:<14} | {gsat_str:<14} | {gnsat_str:<14} | {osat_str:<14} | {onsat_str:<14}")
        
        print(f"{'-'*70}")
        
        diff_sat = count_diff(golden_sat, output_sat, dtype_name) if has_sat else 0
        diff_non_sat = count_diff(golden_non_sat, output_non_sat, dtype_name) if has_non_sat else 0
        
        sat_status = color_text("OK", GREEN) if diff_sat == 0 else color_text("FAIL", RED)
        non_sat_status = color_text("OK", GREEN) if diff_non_sat == 0 else color_text("FAIL", RED)
        
        print(f" Total: {BLOCK_LENGTH}, Special: {num_special}")
        print(f"   Saturation:     {sat_status} (mismatches={diff_sat})")
        print(f"   Non-saturation: {non_sat_status} (mismatches={diff_non_sat})")
        
        result_sat = "PASS" if (diff_sat == 0 or not has_sat) else "FAIL"
        result_non_sat = "PASS" if (diff_non_sat == 0 or not has_non_sat) else "FAIL"
        
        if result_sat == "PASS" and result_non_sat == "PASS":
            result_str = color_text(f"[PASS]", GREEN)
        else:
            result_str = color_text(f"[FAIL]", RED)
        
        print(f" Result: sat={result_sat}, non-sat={result_non_sat} {result_str}")
        
        if result_sat != "PASS" or result_non_sat != "PASS":
            all_pass = False
    
    print(f"\n{BLUE}{'='*70}{RESET}")
    if all_pass:
        print(f"  {color_text('All tests PASSED!', GREEN)}")
    else:
        print(f"  {color_text('Some tests FAILED!', RED)}")
    print(f"{BLUE}{'='*70}{RESET}\n")
    
    return all_pass


if __name__ == "__main__":
    res = verify_all()
    sys.exit(0 if res else 1)
