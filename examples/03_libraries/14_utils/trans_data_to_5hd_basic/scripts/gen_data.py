#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------


import os
import numpy as np
import argparse

# 数据类型映射: dtype_str -> (numpy_dtype, elements_per_block, C0_size)
DTYPE_MAP = {
    'half':   (np.float16, 16, 16),   # b16: C0=16
    'float':  (np.float32, 8, 16),    # b32: C0=16
    'int8':   (np.int8, 32, 32),      # b8:  C0=32
}

ONE_BLK_SIZE = 32  # 每个block 32字节


def trans_data_to_5hd_scatter_b8(src: np.ndarray, srcHighHalf: bool = False,
                                  dstHighHalf: bool = False) -> np.ndarray:
    """
    b8 类型 (int8/uint8) 的 TransDataTo5HD scatter 操作
    
    特点:
        - 每个 block 32 元素
        - scatter 循环 16 次，每次从 16 个 src block 各取 1 个元素
        - 输出每个 block 只有 16 元素（半块）
        - 需要根据 dstHighHalf 和 srcHighHalf 决定读取/写入前半部分还是后半部分
    
    参数:
        srcHighHalf: 源 block 使用高半区 (True: 读取 src[16..31], False: 读取 src[0..15])
        dstHighHalf: 目标 block 使用高半区 (True: 写入 dst[16..31], False: 写入 dst[0..15])
    
    输出大小: 16 * 32 = 512 元素 (完整 block 大小，但只填充 16 个元素)
    """
    total_elems = 16 * 32  # 16 个 block，每个 32 元素
    dst = np.zeros(total_elems, dtype=np.int8)
    
    for j in range(16):  # scatter 循环 16 次
        for i in range(16):  # 16 个源 block
            # 根据 srcHighHalf 决定读取前半部分还是后半部分
            if srcHighHalf:
                src_elem_idx = i * 32 + 16 + j
            else:
                src_elem_idx = i * 32 + j
            
            # 根据 dstHighHalf 决定写入前半部分还是后半部分
            if dstHighHalf:
                dst_elem_idx = j * 32 + 16 + i
            else:
                dst_elem_idx = j * 32 + i
            
            dst[dst_elem_idx] = src[src_elem_idx]
    
    return dst


def trans_data_to_5hd_scatter_b16(src: np.ndarray) -> np.ndarray:
    """
    b16 类型 (half/int16/uint16) 的 TransDataTo5HD scatter 操作
    
    特点:
        - 每个 block 16 元素
        - scatter 循环 16 次
        - dst[j] = [src[0][j], src[1][j], ..., src[15][j]]
        - 即 dst[j*16 + i] = src[i*16 + j]
    
    输出大小: 16 * 16 = 256 元素
    """
    total_elems = 16 * 16
    dst = np.zeros(total_elems, dtype=np.float16)
    
    for j in range(16):
        for i in range(16):
            dst[j * 16 + i] = src[i * 16 + j]
    
    return dst


def trans_data_to_5hd_scatter_b32(src: np.ndarray) -> np.ndarray:
    """
    b32 类型 (float/int32/uint32) 的 TransDataTo5HD scatter 操作
    
    特点:
        - 每个 block 8 元素
        - scatter 循环 8 次，每次产生 2 个 block
        - 前 8 个 src block → dstList[2*j]
        - 后 8 个 src block → dstList[2*j+1]
    
    输出大小: 16 * 8 = 128 元素
    """
    total_elems = 16 * 8
    dst = np.zeros(total_elems, dtype=np.float32)
    
    for j in range(8):
        for i in range(8):
            # 前 8 个 src block → dstList[2*j]
            dst[2 * j * 8 + i] = src[i * 8 + j]
        for i in range(8):
            # 后 8 个 src block → dstList[2*j+1]
            dst[(2 * j + 1) * 8 + i] = src[(8 + i) * 8 + j]
    
    return dst


def trans_data_to_5hd_scatter(src: np.ndarray, dtype: str,
                               srcHighHalf: bool = False, dstHighHalf: bool = False) -> np.ndarray:
    """
    TransDataTo5HD scatter 操作 (16个DataBlock转置)
    
    功能: 将16个连续DataBlock进行scatter转置
    - 输入: 16个DataBlock,每个DataBlock 32字节
    - 输出: scatter转置后的16个DataBlock
    
    scatter语义:
    - b16 (half): 每个block 16元素,scatter循环16次
      dst[j][i] = src[i][j], j=0..15, i=0..15
    - b32 (float): 每个block 8元素,scatter循环8次
      dst[2*j][i] = src[i][j] for i=0..7
      dst[2*j+1][i-8] = src[i][j] for i=8..15
    - b8 (int8): 每个block 32元素,scatter循环16次(输出半块16元素)
      根据 srcHighHalf 和 dstHighHalf 决定读写位置
    
    参数:
        src: 输入数据
        dtype: 数据类型 ('half', 'float', 'int8')
        srcHighHalf: 仅 b8 类型使用，源 block 使用高半区
        dstHighHalf: 仅 b8 类型使用，目标 block 使用高半区
    """
    if dtype == 'int8':
        return trans_data_to_5hd_scatter_b8(src, srcHighHalf, dstHighHalf)
    elif dtype == 'half':
        return trans_data_to_5hd_scatter_b16(src)
    elif dtype == 'float':
        return trans_data_to_5hd_scatter_b32(src)
    else:
        raise ValueError(f"Unsupported dtype: {dtype}")


def gen_golden_data_trans_half():
    """
    场景1: 16*16 的 half 类型 TransDataTo5HD 转置
    - 16个DataBlock,每个16元素,共256元素
    """
    dtype = 'half'
    np_dtype = np.float16
    data_size = 16 * 16  # 256 elements
    
    # 输入数据: 0, 1, 2, ..., 255
    input_data = np.arange(data_size, dtype=np_dtype)
    
    # TransDataTo5HD scatter转置
    golden = trans_data_to_5hd_scatter(input_data, dtype)
    
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_data.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")
    
    print(f"[场景1] half类型 16*16 TransDataTo5HD转置")
    print(f"  输入大小: {data_size} 元素 ({data_size * 2} 字节)")
    print(f"  输出大小: {len(golden)} 元素 ({len(golden) * 2} 字节)")


def gen_golden_data_trans_float():
    """
    场景2: 16*8 的 float 类型 TransDataTo5HD 转置
    - 16个DataBlock,每个8元素,共128元素
    """
    dtype = 'float'
    np_dtype = np.float32
    data_size = 16 * 8  # 128 elements
    
    # 输入数据: 0.0, 1.0, 2.0, ..., 127.0
    input_data = np.arange(data_size, dtype=np_dtype)
    
    # TransDataTo5HD scatter转置
    golden = trans_data_to_5hd_scatter(input_data, dtype)
    
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_data.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")
    
    print(f"[场景2] float类型 16*8 TransDataTo5HD转置")
    print(f"  输入大小: {data_size} 元素 ({data_size * 4} 字节)")
    print(f"  输出大小: {len(golden)} 元素 ({len(golden) * 4} 字节)")


def gen_golden_data_trans_int8():
    """
    场景3: 16*32 的 int8 类型 TransDataTo5HD 转置
    - 16个DataBlock,每个32元素,共512元素
    - b8 类型需要调用两次 scatter:
      - 第一次: srcHighHalf=False, dstHighHalf=False (处理前半部分)
      - 第二次: srcHighHalf=True, dstHighHalf=True (处理后半部分)
    """
    dtype = 'int8'
    np_dtype = np.int8
    data_size = 16 * 32  # 512 elements
    
    # 输入数据: 0, 1, 2, ..., 255 (循环)
    input_data = np.array([i % 256 for i in range(data_size)], dtype=np_dtype)
    
    # b8 类型需要两次 scatter 完成完整转置
    # 第一次 scatter: 处理前半部分数据
    golden_low = trans_data_to_5hd_scatter(input_data, dtype, 
                                            srcHighHalf=False, dstHighHalf=False)
    # 第二次 scatter: 处理后半部分数据
    golden_high = trans_data_to_5hd_scatter(input_data, dtype,
                                             srcHighHalf=True, dstHighHalf=True)
    
    # 合并结果: golden_low 写入低半块，golden_high 写入高半块
    golden = np.zeros(16 * 32, dtype=np_dtype)
    for j in range(16):
        # 低半块 [0..15]
        for i in range(16):
            golden[j * 32 + i] = golden_low[j * 32 + i]
        # 高半块 [16..31]
        for i in range(16):
            golden[j * 32 + 16 + i] = golden_high[j * 32 + 16 + i]
    
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_data.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")
    
    print(f"[场景3] int8类型 16*32 TransDataTo5HD转置")
    print(f"  输入大小: {data_size} 元素 ({data_size} 字节)")
    print(f"  输出大小: {len(golden)} 元素 ({len(golden)} 字节)")
    print(f"  说明: b8类型需要两次scatter完成完整转置")


def nchw_to_nc1hwc0(src: np.ndarray, nSize: int, cSize: int, hSize: int, wSize: int,
                    dtype: str = 'half') -> np.ndarray:
    """
    NCHW → NC1HWC0 格式转换
    
    转换公式:
        NC1HWC0[n, c1, h, w, c0] = NCHW[n, c1*C0 + c0, h, w]
    
    其中:
        C0 = 16 (b16/b32) 或 32 (b8)
        C1 = ceil(C / C0)
    """
    np_dtype, elems_per_block, C0 = DTYPE_MAP[dtype]
    C1 = (cSize + C0 - 1) // C0  # 向上取整
    
    # 输出大小
    dst_size = nSize * C1 * hSize * wSize * C0
    dst = np.zeros(dst_size, dtype=np_dtype)
    
    # 重塑为多维数组
    src_reshaped = src.reshape(nSize, cSize, hSize, wSize)
    dst_reshaped = dst.reshape(nSize, C1, hSize, wSize, C0)
    
    # 执行转换
    for n in range(nSize):
        for c1 in range(C1):
            c_start = c1 * C0
            for h in range(hSize):
                for w in range(wSize):
                    for c0 in range(C0):
                        c = c_start + c0
                        if c < cSize:
                            dst_reshaped[n, c1, h, w, c0] = src_reshaped[n, c, h, w]
    
    return dst


def gen_golden_data_nchw2nc1hwc0():
    """
    场景4: NCHW → NC1HWC0 格式转换
    - 输入: N=2, C=32, H=16, W=16, half类型
    - 输出: NC1HWC0格式
    """
    dtype = 'half'
    np_dtype = np.float16
    nSize, cSize, hSize, wSize = 2, 32, 16, 16
    C0 = DTYPE_MAP[dtype][2]  # 16
    C1 = (cSize + C0 - 1) // C0  # 2
    
    # 输入数据: 每个HW平面用相同值填充
    # NCHW[n,c,h,w] = (n*C + c) 的 H*W 个连续值
    input_size = nSize * cSize * hSize * wSize
    input_data = np.zeros(input_size, dtype=np_dtype)
    
    idx = 0
    for n in range(nSize):
        for c in range(cSize):
            # 每个HW平面填充相同的通道索引值
            hw_value = n * cSize + c
            for h in range(hSize):
                for w in range(wSize):
                    input_data[idx] = hw_value
                    idx += 1
    
    # NCHW → NC1HWC0 转换
    golden = nchw_to_nc1hwc0(input_data, nSize, cSize, hSize, wSize, dtype)
    
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_data.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")
    
    print(f"[场景4] NCHW → NC1HWC0 格式转换")
    print(f"  输入: NCHW [{nSize}, {cSize}, {hSize}, {wSize}]")
    print(f"  输出: NC1HWC0 [{nSize}, {C1}, {hSize}, {wSize}, {C0}]")
    print(f"  输入大小: {input_size} 元素 ({input_size * 2} 字节)")
    print(f"  输出大小: {len(golden)} 元素 ({len(golden) * 2} 字节)")


def gen_golden_data(scenarioNum):
    """根据场景生成测试数据"""
    if scenarioNum == 1:
        gen_golden_data_trans_half()
    elif scenarioNum == 2:
        gen_golden_data_trans_float()
    elif scenarioNum == 3:
        gen_golden_data_trans_int8()
    elif scenarioNum == 4:
        gen_golden_data_nchw2nc1hwc0()
    else:
        raise ValueError(f"Unsupported scenario: {scenarioNum}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="TransDataTo5HD 测试数据生成")
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 5),
                        help='场景编号: 1=half转置, 2=float转置, 3=int8转置, 4=NCHW2NC1HWC0')
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum)
