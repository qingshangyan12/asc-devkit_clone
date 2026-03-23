#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import argparse
import numpy as np


def get_dtype_by_scenario(scenario_num):
    """
    根据场景编号获取输出数据类型
    
    参数:
        scenario_num: 测试场景编号
        
    返回:
        numpy dtype: 输出数据类型
    """
    if scenario_num == 1:
        return np.float16
    elif scenario_num == 2:
        return np.float32
    elif scenario_num == 3:
        return np.float32
    elif scenario_num == 4:
        return np.float32
    elif scenario_num == 5:
        return np.int32
    elif scenario_num == 6:
        return np.float32
    else:
        raise ValueError(f"Unsupported scenario_num: {scenario_num}")


def compare_data(output_file, golden_file, scenario_num=1, epsilon=0.001):
    """
    比较输出数据和真值数据
    
    参数:
        output_file: 输出数据文件路径
        golden_file: 真值数据文件路径
        scenario_num: 测试场景编号
        epsilon: 允许的误差范围
        
    返回:
        bool: 比较结果
    """
    # 获取输出数据类型
    output_dtype = get_dtype_by_scenario(scenario_num)
    
    # 读取输出数据
    output_data = np.fromfile(output_file, dtype=output_dtype)
    
    # 读取真值数据
    golden_data = np.fromfile(golden_file, dtype=output_dtype)
    
    # 检查数据长度是否一致
    if len(output_data) != len(golden_data):
        print(f"Data length mismatch: output={len(output_data)}, golden={len(golden_data)}")
        return False
    
    # 根据数据类型选择比较方式
    if output_dtype in [np.int32]:
        # 整数类型使用精确比较
        diff = np.abs(output_data.astype(np.int64) - golden_data.astype(np.int64))
        max_diff = np.max(diff)
        mean_diff = np.mean(diff)
        
        print(f"Max difference: {max_diff}")
        print(f"Mean difference: {mean_diff}")
        
        if max_diff == 0:
            print("test pass!")
            return True
        else:
            print(f"test failed! max_diff={max_diff}")
            mismatch_indices = np.where(diff > 0)[0]
            print(f"First {min(10, len(mismatch_indices))} mismatched elements:")
            for i, idx in enumerate(mismatch_indices[:10]):
                print(f"  Index {idx}: output={output_data[idx]}, golden={golden_data[idx]}, diff={diff[idx]}")
            return False
    else:
        # 浮点类型使用误差比较
        output_float = output_data.astype(np.float64)
        golden_float = golden_data.astype(np.float64)
        diff = np.abs(output_float - golden_float)
        max_diff = np.max(diff)
        mean_diff = np.mean(diff)
        
        print(f"Max difference: {max_diff}")
        print(f"Mean difference: {mean_diff}")
        
        if max_diff <= epsilon:
            print("test pass!")
            return True
        else:
            print(f"test failed! max_diff={max_diff} > epsilon={epsilon}")
            mismatch_indices = np.where(diff > epsilon)[0]
            print(f"First {min(10, len(mismatch_indices))} mismatched elements:")
            for i, idx in enumerate(mismatch_indices[:10]):
                print(f"  Index {idx}: output={output_data[idx]}, golden={golden_data[idx]}, diff={diff[idx]}")
            return False


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Verify output result for Cast operator.")
    parser.add_argument('output_file', type=str, help="Output data file path")
    parser.add_argument('golden_file', type=str, help="Golden data file path")
    parser.add_argument('-scenarioNum', type=int, default=1,
                        help="Test scenario number (1=float->half, 2=half->float, 3=float->bfloat16, 4=bfloat16->float, 5=float->int32, 6=int32->float)")
    parser.add_argument('-epsilon', type=float, default=0.001,
                        help="Allowed error range (default: 0.001)")
    
    args = parser.parse_args()
    
    # 检查文件是否存在
    if not os.path.exists(args.output_file):
        print(f"Output file not found: {args.output_file}")
        exit(1)
    
    if not os.path.exists(args.golden_file):
        print(f"Golden file not found: {args.golden_file}")
        exit(1)
    
    # 比较数据
    result = compare_data(args.output_file, args.golden_file, args.scenarioNum, args.epsilon)
    
    exit(0 if result else 1)
