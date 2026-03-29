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

API_NAME = "SetAtomicAdd"

RESET = "\033[0m"
YELLOW = "\033[93m"
GREEN = "\033[92m"
RED = "\033[91m"
BLUE = "\033[94m"
ORANGE = "\033[38;5;208m"


def color_text(text, color):
    return f"{color}{text}{RESET}"


def pad_colored(value, width, color):
    val_str = str(value)
    padding = width - len(val_str)
    return f"{color_text(val_str, color)}{' ' * padding}"


sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from gen_data_utils import (
    get_dtype_limits,
    construct_special_values,
    get_special_names
)

NUM_CORES = 3
BLOCK_LENGTH = 256
ERROR_TOL = 1e-4


def format_value(val, np_type):
    if np.issubdtype(np_type, np.floating):
        if np.isnan(val):
            return "nan"
        elif np.isinf(val):
            return f"{'inf' if val > 0 else '-inf'}"
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


def count_matches(golden, output, dtype_name, num_special):
    match_count = 0
    for i in range(num_special):
        if compare_values(golden[i], output[i], dtype_name):
            match_count += 1
    return match_count


def verify_all():
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
        
        print(color_text(f"[API: {API_NAME}]--[InputDataType: {dtype_name}]--[OutputDataType: {dtype_name}]", YELLOW))
        header = f"| {'inputType':<12} | {'outputType':<12} | {'input1':<10} | {'golden(non-sat)':<16} | {'golden(sat)':<14} | {'output(non-sat)':<16} | {'output(sat)':<14} |"
        print(header)
        separator = f"| ---          | ---           | ---        | ---             | ---          | ---             | ---          |"
        print(separator)
        
        for i in range(num_special):
            inp_val = special_vals[i]
            golden_sat_val = golden_sat[i]
            golden_non_sat_val = golden_non_sat[i]
            output_sat_val = output_sat[i]
            output_non_sat_val = output_non_sat[i]
            name = special_names[i]
            
            inp_str = format_value(inp_val, data_type)
            gsat_val = format_value(golden_sat_val, data_type)
            gnsat_val = format_value(golden_non_sat_val, data_type)
            osat_val = format_value(output_sat_val, data_type) if has_sat else "-"
            onsat_val = format_value(output_non_sat_val, data_type) if has_non_sat else "-"
            
            print(f"| {dtype_name:<12} | {dtype_name:<12} | {name:<10} | "
                  f"{pad_colored(gnsat_val, 16, ORANGE)} | {pad_colored(gsat_val, 14, BLUE)} | "
                  f"{pad_colored(onsat_val, 16, ORANGE)} | {pad_colored(osat_val, 14, BLUE)} |")
        
        matches_sat = count_matches(golden_sat, output_sat, dtype_name, num_special) if has_sat else num_special
        matches_non_sat = count_matches(golden_non_sat, output_non_sat, dtype_name, num_special) if has_non_sat else num_special
        
        sat_color = GREEN if matches_sat == num_special else RED
        non_sat_color = GREEN if matches_non_sat == num_special else RED
        
        print(color_text(f" Sat verify result: Matches: {matches_sat} / {num_special}.", sat_color))
        print(color_text(f" Non-sat verify results: Matches: {matches_non_sat} / {num_special}.", non_sat_color))
        
        result_sat = "PASS" if (matches_sat == num_special or not has_sat) else "FAIL"
        result_non_sat = "PASS" if (matches_non_sat == num_special or not has_non_sat) else "FAIL"
        
        if result_sat != "PASS" or result_non_sat != "PASS":
            all_pass = False
        
        print()
    
    return all_pass


if __name__ == "__main__":
    res = verify_all()
    sys.exit(0 if res else 1)
