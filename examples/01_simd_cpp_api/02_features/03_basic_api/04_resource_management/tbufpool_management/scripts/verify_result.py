#!/usr/bin/python3
# -*- coding: utf-8 -*-

import os
import argparse
import numpy as np

def verify_result(scenarioNum, output_file, golden_file):
    """
    验证输出结果与真值是否一致
    Args:
        scenarioNum: 场景编号(1=TBufPool内存复用,2=TBufPool资源细分,3=自定义TBufPool)
        output_file: 输出文件路径
        golden_file: 真值文件路径
    """
    if not os.path.exists(output_file):
        print(f"Output file {output_file} not found!")
        return False
    
    if not os.path.exists(golden_file):
        print(f"Golden file {golden_file} not found!")
        return False

    output = np.fromfile(output_file, dtype=np.float16)
    golden = np.fromfile(golden_file, dtype=np.float16)

    if output.shape != golden.shape:
        print(f"Shape mismatch: output {output.shape}, golden {golden.shape}")
        return False

    diff = np.abs(output - golden)
    max_diff = np.max(diff)
    mean_diff = np.mean(diff)

    threshold = 0.001
    if max_diff < threshold:
        print("test pass!")
        print(f"Max diff: {max_diff}")
        print(f"Mean diff: {mean_diff}")
        return True
    else:
        print("test failed!")
        print(f"Max diff: {max_diff}")
        print(f"Mean diff: {mean_diff}")
        return False

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 4))
    parser.add_argument('output_file', type=str, help='Output file path')
    parser.add_argument('golden_file', type=str, help='Golden file path')
    args = parser.parse_args()
    verify_result(args.scenarioNum, args.output_file, args.golden_file)