#!/usr/bin/python3
# -*- coding: utf-8 -*-

import os
import argparse
import numpy as np

def gen_golden_data(scenarioNum):
    """
    生成测试输入数据和真值数据
    Args:
        scenarioNum: 场景编号(1=TBufPool内存复用,2=TBufPool资源细分,3=自定义TBufPool)
    """
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    if scenarioNum == 1:
        # 场景1：TBufPool内存复用，shape=[3, 65536]
        total_size = 3 * 65536
        x = np.random.uniform(1, 10, [total_size]).astype(np.float16)
        y = np.random.uniform(1, 10, [total_size]).astype(np.float16)
        golden = (x.astype(np.float32) + y.astype(np.float32)).astype(np.float16)
    elif scenarioNum == 2:
        # 场景2：TBufPool资源细分，shape=[4, 32768]
        total_size = 4 * 32768
        x = np.random.uniform(1, 10, [total_size]).astype(np.float16)
        y = np.random.uniform(1, 10, [total_size]).astype(np.float16)
        golden = (x.astype(np.float32) + y.astype(np.float32)).astype(np.float16)
    elif scenarioNum == 3:
        # 场景3：自定义TBufPool，shape=[1, 65536]
        total_size = 65536
        x = np.random.uniform(1, 10, [total_size]).astype(np.float16)
        y = np.random.uniform(1, 10, [total_size]).astype(np.float16)
        golden = (x.astype(np.float32) + y.astype(np.float32)).astype(np.float16)
    else:
        print(f"scenarioNum {scenarioNum} is not supported!")
        return

    x.tofile("./input/input_x.bin")
    y.tofile("./input/input_y.bin")
    golden.tofile("./output/golden.bin")

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 4))
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum)
