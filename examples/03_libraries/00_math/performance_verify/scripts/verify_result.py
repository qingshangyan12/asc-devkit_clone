#!/usr/bin/env python3
# coding=utf-8
"""
Ascend C API 性能验证结果校验模块
功能：比对 kernel 输出与 golden 数据，验证精度正确性
"""

import sys
import argparse
import numpy as np

from gen_data_utils import (
    API_NAME_MAP, DATA_TYPE_NP_MAP, THEORETICAL_PERFORMANCE, REPEAT_TIMES
)

RELATIVE_TOL = 1e-4      # 相对误差容限
ABSOLUTE_TOL = 1e-4      # 绝对误差容限
ERROR_TOL = 1e-3         # 错误率容限
SAMPLE_SIZE = 10         # 抽样显示数量


def get_input_data(api_type, output_type):
    """读取输入数据文件"""
    np_type = DATA_TYPE_NP_MAP.get(output_type, np.float16)
    input_x, input_y = None, None
    try:
        if api_type in [2, 3]:  # Binary or BinaryScalar 类型需要两个输入
            input_x = np.fromfile("./input/input_x.bin", dtype=np_type).reshape(-1)
            input_y = np.fromfile("./input/input_y.bin", dtype=np_type).reshape(-1)
        else:  # Unary 类型只需一个输入
            input_x = np.fromfile("./input/input_x.bin", dtype=np_type).reshape(-1)
    except:
        pass
    return input_x, input_y


def format_value(val, is_float=True):
    """格式化数值显示，保留2位小数"""
    if is_float:
        return f"{float(val):.2f}"
    return str(int(val))


def verify_result(output_type, api_id, api_type=1):
    """
    验证 kernel 输出与 golden 数据的一致性
    
    Args:
        output_type: 数据类型 (1=half, 2=float, 3=int16, 4=int32)
        api_id: API 编号
        api_type: API 类型 (1=Unary, 2=Binary, 3=BinaryScalar)
    
    Returns:
        bool: 验证是否通过
    """
    np_type = DATA_TYPE_NP_MAP.get(output_type, np.float16)
    type_name = np_type.__name__
    api_name = API_NAME_MAP.get(api_id, "unknown")
    
    try:
        # 读取输出和 golden 数据
        output = np.fromfile("./output/output.bin", dtype=np_type).reshape(-1)
        golden = np.fromfile("./output/golden.bin", dtype=np_type).reshape(-1)
        input_x, input_y = get_input_data(api_type, output_type)
        
        # 检查数据长度一致性
        if len(output) != len(golden):
            print(f"\n  \033[31m✗ ERROR: Length mismatch - output={len(output)}, golden={len(golden)}\033[0m")
            return False
        
        # 遍历比较，统计匹配数和错误索引
        valid_count = 0
        failed_indices = []
        sample_indices = []
        
        for i in range(len(golden)):
            actual = output[i]
            expected = golden[i]
            
            # 根据数据类型选择匹配判定方式
            if np.isnan(expected):
                is_match = np.isnan(actual)
            elif np.isinf(expected):
                is_match = np.isinf(actual) and np.sign(actual) == np.sign(expected)
            else:
                is_match = np.isclose(actual, expected, rtol=RELATIVE_TOL, atol=ABSOLUTE_TOL)
            
            if is_match:
                valid_count += 1
            else:
                failed_indices.append(i)
                if len(sample_indices) < SAMPLE_SIZE:
                    sample_indices.append(i)
        
        error_ratio = 1.0 - (valid_count / len(golden))
        is_float = np.issubdtype(np_type, np.floating)
        
        # 显示验证结果头部
        status_icon = "\033[32m PASS\033[0m" if error_ratio <= ERROR_TOL else "\033[31m✗ FAIL\033[0m"
        dtype_str = "float16" if output_type == 1 else "float32" if output_type == 2 else "int16" if output_type == 3 else "int32"
        print(f"\n  [DataType] {dtype_str}")
        print(f"  \033[32m✓ Precision Verification\033[0m: {status_icon}")
        print(f"  Matched: \033[32m{valid_count}\033[0m / {len(golden)}, Error Ratio: {error_ratio:.6f}")
        
        # 失败时显示错误详情
        if failed_indices:
            print(f"\n  \033[31mErrors ({len(failed_indices)}):\033[0m")
            print(f"  {'idx':<8} {'input':<12} {'expected':<12} {'actual':<12}")
            print(f"  {'-'*44}")
            for idx in sample_indices[:SAMPLE_SIZE]:
                inp = format_value(input_x[idx], is_float) if input_x is not None else "N/A"
                exp = format_value(golden[idx], is_float)
                act = format_value(output[idx], is_float)
                print(f"  {idx:<8} {inp:<12} {exp:<12} {act:<12}")
            if len(failed_indices) > SAMPLE_SIZE:
                print(f"  \033[90m... and {len(failed_indices) - SAMPLE_SIZE} more errors\033[0m")
        else:
            # 通过时抽样显示结果
            print(f"\n  \033[90mSample results:\033[0m")
            print(f"  {'idx':<8} {'input':<12} {'expected':<12} {'actual':<12}")
            print(f"  {'-'*44}")
            for idx in range(min(SAMPLE_SIZE, len(golden))):
                inp = format_value(input_x[idx], is_float) if input_x is not None else "N/A"
                exp = format_value(golden[idx], is_float)
                act = format_value(output[idx], is_float)
                print(f"  {idx:<8} {inp:<12} {exp:<12} {act:<12}")
        
        return error_ratio <= ERROR_TOL
            
    except Exception as e:
        print(f"\n  \033[31m✗ ERROR: {e}\033[0m")
        return False


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Verify kernel output vs golden data")
    parser.add_argument('-api_id', type=int, default=1, choices=[1, 2, 3, 4])
    parser.add_argument('-api_type', type=int, default=1, choices=[1, 2, 3])
    parser.add_argument('-output_type', type=int, default=1, choices=[1, 2, 3, 4])
    args = parser.parse_args()
    
    res = verify_result(args.output_type, args.api_id, args.api_type)
    if not res:
        sys.exit(1)
