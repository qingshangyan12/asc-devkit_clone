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
    format_float16
)

ERROR_TOL = 1e-4
RELATIVE_TOL = 1e-6
ABSOLUTE_TOL = 1e-9


def is_special_float(val):
    return np.isnan(val) or np.isinf(val)


def format_value(val, np_type):
    if np.issubdtype(np_type, np.floating):
        return f"{float(val):.6e}"
    return str(val)


def verify_single_api(dtype_name, output_path, golden_path, api_name="AddAtomicAdd"):
    np_type = DTYPE_MAP.get(dtype_name)
    if np_type is None:
        print(f"[ERROR] Unsupported dtype: {dtype_name}")
        return False
    
    if not os.path.exists(output_path):
        print(f"[ERROR] Output file not found: {output_path}")
        return False
    
    if not os.path.exists(golden_path):
        print(f"[ERROR] Golden file not found: {golden_path}")
        return False
    
    output = np.fromfile(output_path, dtype=np_type).reshape(-1)
    golden = np.fromfile(golden_path, dtype=np_type).reshape(-1)
    
    if len(output) == 0:
        print(f"[ERROR] Output file is empty: {output_path}")
        return False
    
    if len(golden) == 0:
        print(f"[ERROR] Golden file is empty: {golden_path}")
        return False
    
    type_name = np.dtype(np_type).name
    special_names = get_special_names(np_type)
    special_vals = construct_special_values(np_type)
    num_special = len(special_vals)
    
    print(f"\n{'='*116}")
    print(f"API: {api_name} | Data Type: {type_name} | Verification Results")
    print(f"{'='*116}")
    print(f"{'type':<10} | {'name':<8} | {'src':<20} | {'expected':<20} | {'actual':<20} | {'status'}")
    print(f"{'-'*116}")
    
    valid_count = 0
    
    for i in range(num_special):
        inp_val = special_vals[i]
        golden_val = golden[i] if i < len(golden) else 0
        output_val = output[i] if i < len(output) else 0
        name = special_names[i] if i < len(special_names) else f"idx_{i}"
        
        is_match = False
        if dtype_name in FLOAT_DTYPES:
            if is_special_float(golden_val):
                if np.isnan(golden_val):
                    is_match = np.isnan(output_val)
                else:
                    is_match = np.isinf(output_val) and np.sign(output_val) == np.sign(golden_val)
            else:
                is_match = np.isclose(float(output_val), float(golden_val), rtol=RELATIVE_TOL, atol=ABSOLUTE_TOL, equal_nan=True)
        else:
            is_match = (output_val == golden_val)
        
        if is_match:
            valid_count += 1
        
        status = "PASS" if is_match else "FAIL"
        inp_str = format_value(inp_val, np_type)
        golden_str = format_value(golden_val, np_type)
        output_str = format_value(output_val, np_type)
        
        print(f"{type_name:<10} | {name:<8} | {inp_str:<20} | {golden_str:<20} | {output_str:<20} | {status}")
    
    print(f"{'-'*116}")
    
    different_count = 0
    for i in range(len(golden)):
        golden_val = golden[i]
        output_val = output[i]
        
        is_diff = False
        if dtype_name in FLOAT_DTYPES:
            if is_special_float(golden_val):
                if not (np.isnan(output_val) or np.isinf(output_val)):
                    is_diff = True
            elif not np.isclose(float(output_val), float(golden_val), rtol=RELATIVE_TOL, atol=ABSOLUTE_TOL, equal_nan=True):
                is_diff = True
        else:
            if output_val != golden_val:
                is_diff = True
        
        if is_diff:
            different_count += 1
    
    error_ratio = different_count / golden.size if golden.size > 0 else 1.0
    
    print(f"Total: {len(golden)}, Matched: {valid_count}/{num_special}, Mismatches: {different_count}, Error Ratio: {error_ratio:.4f}")
    print(f"{'='*116}\n")
    
    if error_ratio <= ERROR_TOL:
        print(f"*** Result: PASS ***")
        return True
    else:
        print(f"*** Result: FAIL ***")
        return False


def verify_result(output_path, golden_path, dtype_name):
    return verify_single_api(dtype_name, output_path, golden_path, "SetAtomicAdd")


if __name__ == '__main__':
    if len(sys.argv) < 4:
        print("Usage: verify_result.py <output> <golden> <dtype>")
        print("  dtype: int8_t, int16_t, half, bfloat16_t, int32_t, float")
        sys.exit(1)
    
    try:
        res = verify_result(sys.argv[1], sys.argv[2], sys.argv[3])
        if not res:
            sys.exit(1)
    except Exception as e:
        print(f"[ERROR] {e}")
        sys.exit(1)
