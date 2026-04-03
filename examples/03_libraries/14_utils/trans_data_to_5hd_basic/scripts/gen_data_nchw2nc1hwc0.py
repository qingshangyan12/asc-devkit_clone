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

"""
NCHW → NC1HWC0 格式转换真值生成脚本

接口参数与 TransDataTo5HD 一致：
    - srcList[16]: uint64_t[16] - 16个源block的字节地址
    - dstList[16]: uint64_t[16] - 16个目标block的字节地址
    - TransDataTo5HDParams:
        - dstHighHalf: 目标block使用高半区 (仅b8类型)
        - srcHighHalf: 源block使用高半区 (仅b8类型)
        - repeatTimes: 重复次数
        - dstRepStride: 目标重复步长 (block数)
        - srcRepStride: 源重复步长 (block数)

格式说明：
    - NCHW: [N, C, H, W] - 数据按 N-C-H-W 顺序连续存储
    - NC1HWC0: [N, C1, H, W, C0] - C0维度的数据被分块重排
        - C0 = 16 (b16类型: half/int16/uint16, 以及b32类型: float/int32/uint32)
        - C0 = 32 (b8类型: int8/uint8)
        - C1 = (C + C0 - 1) // C0 (向上取整)

转换核心逻辑：
    将16个连续C通道的HW平面数据，转置为C0维度的分块格式
    每16个C通道对应一次scatter操作
"""

import os
import numpy as np
import argparse


# 数据类型映射：dtype_str -> (numpy_dtype, elements_per_block, C0_size)
# 注意：b16和b32的C0都是16，b8的C0是32
DTYPE_MAP = {
    'half':   (np.float16, 16, 16),   # b16: 16元素/block, C0=16
    'float':  (np.float32, 8, 16),    # b32: 8元素/block, C0=16 !!!
    'int16':  (np.int16, 16, 16),     # b16: 16元素/block, C0=16
    'uint16': (np.uint16, 16, 16),    # b16: 16元素/block, C0=16
    'int32':  (np.int32, 8, 16),      # b32: 8元素/block, C0=16 !!!
    'uint32': (np.uint32, 8, 16),     # b32: 8元素/block, C0=16 !!!
    'int8':   (np.int8, 32, 32),      # b8: 32元素/block, C0=32
    'uint8':  (np.uint8, 32, 32),     # b8: 32元素/block, C0=32
}

# 场景映射
SCENARIO_DTYPE_MAP = {
    1: 'half',
    2: 'float',
    3: 'int16',
    4: 'int8',
    5: 'uint16',
    6: 'uint32',
    7: 'uint8',
    8: 'int32',
}

# 常量
NCHW_CONV_ADDR_LIST_SIZE = 16  # 地址列表大小
ONE_BLK_SIZE = 32               # 每个block 32字节


def generate_addr_list_from_nchw(n_idx, c_start_idx, H, W, dtype_size, base_addr=0):
    """
    根据 NCHW 形状生成 16 个 HW 平面起点的字节地址数组
    
    用于 NCHW → NC1HWC0 转换时，srcList 应指向连续 16 个 C 通道的 HW 平面起点
    
    NCHW 内存布局：
        - 数据按 N-C-H-W 顺序连续存储
        - 每个 HW 平面大小 = H * W 个元素
        - 相邻 C 通道的 HW 平面起点间隔 = H * W 个元素
        
    地址计算：
        srcList[i] = base_addr + (n_idx * C_total + c_start_idx + i) * H * W * sizeof(dtype)
        
    参数:
        n_idx: batch 索引
        c_start_idx: 起始 C 通道索引 (当前处理的 C1 分组中的起始位置)
        H: 高度
        W: 宽度
        dtype_size: 数据类型大小 (字节)
        base_addr: 基地址 (默认0)
    
    返回:
        addr_list: 16 个字节地址的列表
    """
    hw_plane_size = H * W  # HW 平面大小 (元素数)
    hw_plane_bytes = hw_plane_size * dtype_size  # HW 平面大小 (字节数)
    
    addr_list = []
    for i in range(NCHW_CONV_ADDR_LIST_SIZE):
        # 字节地址 = 基地址 + n偏移 + c偏移 + h偏移 + w偏移
        # 对于 HW 平面起点，h=0, w=0
        byte_addr = base_addr + n_idx * hw_plane_bytes * NCHW_CONV_ADDR_LIST_SIZE + (c_start_idx + i) * hw_plane_bytes
        addr_list.append(byte_addr)
    
    return addr_list


def generate_dst_addr_list_from_nc1hwc0(n_idx, c1_idx, H, W, C0, dtype_size, base_addr=0):
    """
    根据 NC1HWC0 形状生成 16 个目标地址的字节数组
    
    NC1HWC0 内存布局：
        - 数据按 N-C1-H-W-C0 顺序连续存储
        - 对于固定 (n, c1, h, w)，C0 维度连续存储 16 个元素
        
    地址计算：
        dstList[j] 指向 NC1HWC0 中 (n, c1, h=0, w=j/stride, c0_start) 的位置
        实际上 dstList 需要根据 scatter 输出格式来设置
        
    参数:
        n_idx: batch 索引
        c1_idx: C1 维度索引
        H: 高度
        W: 宽度
        C0: C0 维度大小 (16 或 32)
        dtype_size: 数据类型大小 (字节)
        base_addr: 基地址
    
    返回:
        addr_list: 16 个字节地址的列表
    """
    # NC1HWC0: [N, C1, H, W, C0]
    # 每个 (n, c1, h, w) 位置有 C0 个元素
    # scatter 输出时，dstList[j] 指向某个 HW 位置的起点
    
    # 简化处理：假设 dst 和 src 使用相同的地址布局策略
    # 实际使用时需要根据具体 scatter 输出格式调整
    
    hw_plane_size_c0 = H * W * C0  # 一个 C1 分组的 HW*C0 大小
    hw_plane_size_c0_bytes = hw_plane_size_c0 * dtype_size
    
    addr_list = []
    for j in range(NCHW_CONV_ADDR_LIST_SIZE):
        # 指向每个 W 位置的起点（或根据实际 scatter 语义调整）
        byte_addr = base_addr + n_idx * hw_plane_size_c0_bytes + c1_idx * hw_plane_size_c0_bytes + j * C0 * dtype_size
        addr_list.append(byte_addr)
    
    return addr_list


def get_block_elem_index(byte_addr, dtype_size):
    """
    根据字节地址计算元素索引
    
    参数:
        byte_addr: 字节地址 (uint64_t)
        dtype_size: 数据类型大小 (字节)
    
    返回:
        元素索引
    """
    return byte_addr // dtype_size


def trans_data_to_5hd_scatter(input_data, output_data, dtype_str,
                              srcList, dstList,
                              repeatTimes=1, dstRepStride=16, srcRepStride=16,
                              dstHighHalf=False, srcHighHalf=False):
    """
    单次 scatter 操作的真值计算
    
    入参与 TransDataTo5HD 接口一致：
        srcList[16]: uint64_t[16] - 16个源block的字节地址
        dstList[16]: uint64_t[16] - 16个目标block的字节地址
    
    参数:
        input_data: 输入数据 (1D array)
        output_data: 输出数据 (1D array, 会就地修改)
        dtype_str: 数据类型字符串
        srcList: 16个源block的字节地址列表
        dstList: 16个目标block的字节地址列表
        repeatTimes: 重复次数
        dstRepStride: 目标重复步长 (block数)
        srcRepStride: 源重复步长 (block数)
        dstHighHalf: 目标block使用高半区
        srcHighHalf: 源block使用高半区
    """
    dtype, elems_per_block, C0 = DTYPE_MAP[dtype_str]
    dtype_size = np.dtype(dtype).itemsize
    
    # 对每次repeat执行scatter
    for rep in range(repeatTimes):
        src_block_offset = rep * srcRepStride
        dst_block_offset = rep * dstRepStride
        
        # 根据 scatter 规则处理
        if dtype_str in ['int8', 'uint8']:
            # b8类型：每个block 32元素，scatter循环16次，每次处理半块
            # srcList[i] 的第 j 个半块 -> dstList[j] 的第 i 个半块
            for j in range(16):  # 16次循环
                # 计算目标地址
                dst_base_idx = get_block_elem_index(dstList[j], dtype_size)
                dst_block_offset_actual = dst_block_offset * elems_per_block
                
                for i in range(16):  # 16个源block
                    # 计算源地址
                    src_base_idx = get_block_elem_index(srcList[i], dtype_size)
                    src_block_offset_actual = src_block_offset * elems_per_block
                    
                    # 从源读取半块数据
                    if srcHighHalf:
                        src_start = src_base_idx + src_block_offset_actual + 16
                    else:
                        src_start = src_base_idx + src_block_offset_actual
                    
                    # 写入目标半块数据
                    if dstHighHalf:
                        dst_start = dst_base_idx + dst_block_offset_actual + 16 + i
                    else:
                        dst_start = dst_base_idx + dst_block_offset_actual + i
                    
                    # scatter: dstList[j][i] = srcList[i][j]
                    # 对于b8: 每次循环从16个src block各取1个元素，组成16个元素写入dst block
                    src_elem_idx = src_start + j
                    dst_elem_idx = dst_start
                    
                    if src_elem_idx < len(input_data) and dst_elem_idx < len(output_data):
                        output_data[dst_elem_idx] = input_data[src_elem_idx]
        
        else:
            # b16类型：每个block 16元素，scatter循环16次，每次取1个元素组成新block
            # b32类型：每个block 8元素，scatter循环8次，每次取1个元素组成2个新block
            num_loops = elems_per_block  # b16=16次, b32=8次
            
            for j in range(num_loops):
                for i in range(16):
                    # 计算源地址
                    src_base_idx = get_block_elem_index(srcList[i], dtype_size)
                    src_block_offset_actual = src_block_offset * elems_per_block
                    src_elem_idx = src_base_idx + src_block_offset_actual + j
                    
                    # 计算目标地址
                    # b16: dstList[j] 的第 i 个位置
                    # b32: dstList[2*j] 和 dstList[2*j+1] 各取一半
                    if dtype_str in ['half', 'int16', 'uint16']:
                        dst_base_idx = get_block_elem_index(dstList[j], dtype_size)
                        dst_block_offset_actual = dst_block_offset * elems_per_block
                        dst_elem_idx = dst_base_idx + dst_block_offset_actual + i
                    else:  # b32
                        # b32: 每次循环产生2个block，每个block8元素
                        # dstList[2*j] 的前8个位置放前8个src的第j个元素
                        # dstList[2*j+1] 的前8个位置放后8个src的第j个元素
                        if i < 8:
                            dst_base_idx = get_block_elem_index(dstList[2 * j], dtype_size)
                            dst_elem_idx = dst_base_idx + dst_block_offset * elems_per_block + i
                        else:
                            dst_base_idx = get_block_elem_index(dstList[2 * j + 1], dtype_size)
                            dst_elem_idx = dst_base_idx + dst_block_offset * elems_per_block + (i - 8)
                    
                    if src_elem_idx < len(input_data) and dst_elem_idx < len(output_data):
                        output_data[dst_elem_idx] = input_data[src_elem_idx]


def nchw_to_nc1hwc0_full(nchw_data, N, C, H, W, dtype_str):
    """
    完整的 NCHW → NC1HWC0 格式转换
    
    参数:
        nchw_data: 输入数据 (NCHW格式, 1D array)
        N, C, H, W: 数据维度
        dtype_str: 数据类型字符串
    
    返回:
        nc1hwc0_data: 转换后的数据
    """
    dtype, elems_per_block, C0 = DTYPE_MAP[dtype_str]
    nchw_data = nchw_data.astype(dtype)
    
    # C1 向上取整
    C1 = (C + C0 - 1) // C0
    
    # NC1HWC0 输出形状
    output_size = N * C1 * H * W * C0
    nc1hwc0_data = np.zeros(output_size, dtype=dtype)
    
    # NCHW: [N, C, H, W] -> 元素索引: n*C*H*W + c*H*W + h*W + w
    # NC1HWC0: [N, C1, H, W, C0] -> 元素索引: n*C1*H*W*C0 + c1*H*W*C0 + h*W*C0 + w*C0 + c0
    
    for n in range(N):
        for c1 in range(C1):
            for h in range(H):
                for w in range(W):
                    for c0 in range(C0):
                        # NCHW索引
                        c = c1 * C0 + c0
                        if c >= C:
                            # 超出原始C范围，填充0
                            continue
                        nchw_idx = n * C * H * W + c * H * W + h * W + w
                        
                        # NC1HWC0索引
                        nc1hwc0_idx = n * C1 * H * W * C0 + c1 * H * W * C0 + h * W * C0 + w * C0 + c0
                        
                        if nchw_idx < len(nchw_data):
                            nc1hwc0_data[nc1hwc0_idx] = nchw_data[nchw_idx]
    
    return nc1hwc0_data


def gen_golden_data_nchw2nc1hwc0(scenarioNum, N=1, C=16, H=4, W=4,
                                  repeatTimes=1, dstRepStride=16, srcRepStride=16,
                                  dstHighHalf=False, srcHighHalf=False):
    """
    生成 NCHW → NC1HWC0 转换的测试数据和真值
    
    参数:
        scenarioNum: 场景编号 (1-8)
        N, C, H, W: 数据维度 (C不一定是C0的倍数)
        repeatTimes: 重复次数
        dstRepStride: 目标重复步长 (block数)
        srcRepStride: 源重复步长 (block数)
        dstHighHalf: 目标block使用高半区 (仅b8类型)
        srcHighHalf: 源block使用高半区 (仅b8类型)
    """
    if scenarioNum not in SCENARIO_DTYPE_MAP:
        raise ValueError(f"Unsupported scenario: {scenarioNum}")
    
    dtype_str = SCENARIO_DTYPE_MAP[scenarioNum]
    dtype, elems_per_block, C0 = DTYPE_MAP[dtype_str]
    
    # C1 向上取整
    C1 = (C + C0 - 1) // C0
    
    # 生成 NCHW 格式的输入数据
    nchw_size = N * C * H * W
    if dtype_str in ['half', 'float']:
        input_data = np.arange(1, nchw_size + 1, dtype=dtype) * 0.5
    else:
        input_data = np.arange(1, nchw_size + 1, dtype=dtype)
    
    # 计算真值 (NCHW → NC1HWC0)
    golden = nchw_to_nc1hwc0_full(input_data, N, C, H, W, dtype_str)
    
    # 创建目录并保存
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_data.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")
    
    print(f"Generated NCHW → NC1HWC0 data:")
    print(f"  Scenario: {scenarioNum} ({dtype_str})")
    print(f"  Input shape (NCHW):  [{N}, {C}, {H}, {W}]")
    print(f"  Output shape (NC1HWC0): [{N}, {C1}, {H}, {W}, {C0}]")
    print(f"  C0 (block size in C dim): {C0}")
    print(f"  C1 (ceil div): {C1}")
    print(f"  TransDataTo5HD params:")
    print(f"    repeatTimes: {repeatTimes}")
    print(f"    dstRepStride: {dstRepStride}")
    print(f"    srcRepStride: {srcRepStride}")
    print(f"    dstHighHalf: {dstHighHalf}")
    print(f"    srcHighHalf: {srcHighHalf}")
    print(f"  Input size: {nchw_size} elements ({nchw_size * dtype.itemsize} bytes)")
    print(f"  Output size: {golden.size} elements ({golden.size * dtype.itemsize} bytes)")


def gen_golden_data_block_mode(scenarioNum, srcList, dstList,
                                srcDataSize=256, dstDataSize=None,
                                repeatTimes=1, dstRepStride=16, srcRepStride=16,
                                dstHighHalf=False, srcHighHalf=False):
    """
    Block模式：直接指定 srcList/dstList 地址
    
    入参与 TransDataTo5HD 接口一致：
        srcList[16]: uint64_t[16] - 16个源block的字节地址
        dstList[16]: uint64_t[16] - 16个目标block的字节地址
    
    参数:
        scenarioNum: 场景编号
        srcList: 16个源block的字节地址列表 (uint64_t)
        dstList: 16个目标block的字节地址列表 (uint64_t)
        srcDataSize: 输入数据大小 (元素数)
        dstDataSize: 输出数据大小 (元素数), None则自动计算
        repeatTimes: 重复次数
        dstRepStride: 目标重复步长
        srcRepStride: 源重复步长
        dstHighHalf, srcHighHalf: b8类型的高低半区控制
    """
    if scenarioNum not in SCENARIO_DTYPE_MAP:
        raise ValueError(f"Unsupported scenario: {scenarioNum}")
    
    dtype_str = SCENARIO_DTYPE_MAP[scenarioNum]
    dtype, elems_per_block, C0 = DTYPE_MAP[dtype_str]
    dtype_size = np.dtype(dtype).itemsize
    
    # 确保srcList和dstList长度为16
    if len(srcList) != NCHW_CONV_ADDR_LIST_SIZE or len(dstList) != NCHW_CONV_ADDR_LIST_SIZE:
        raise ValueError(f"srcList and dstList must have exactly {NCHW_CONV_ADDR_LIST_SIZE} elements")
    
    # 生成输入数据
    if dtype_str in ['half', 'float']:
        input_data = np.arange(1, srcDataSize + 1, dtype=dtype) * 0.5
    else:
        input_data = np.arange(1, srcDataSize + 1, dtype=dtype)
    
    # 计算输出大小
    if dstDataSize is None:
        # 根据最大dst地址和repeat计算
        max_dst_block = max(dstList) // dtype_size // elems_per_block
        total_dst_blocks = max_dst_block + NCHW_CONV_ADDR_LIST_SIZE + (repeatTimes - 1) * dstRepStride
        dstDataSize = total_dst_blocks * elems_per_block
    
    output_data = np.zeros(dstDataSize, dtype=dtype)
    
    # 计算真值
    trans_data_to_5hd_scatter(
        input_data, output_data, dtype_str,
        srcList, dstList,
        repeatTimes, dstRepStride, srcRepStride,
        dstHighHalf, srcHighHalf
    )
    
    # 保存
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_data.tofile("./input/input_x.bin")
    output_data.tofile("./output/golden.bin")
    
    print(f"Generated TransDataTo5HD block mode data:")
    print(f"  Scenario: {scenarioNum} ({dtype_str})")
    print(f"  srcList (byte addrs): {srcList}")
    print(f"  dstList (byte addrs): {dstList}")
    print(f"  repeatTimes: {repeatTimes}")
    print(f"  dstRepStride: {dstRepStride}")
    print(f"  srcRepStride: {srcRepStride}")
    print(f"  dstHighHalf: {dstHighHalf}")
    print(f"  srcHighHalf: {srcHighHalf}")
    print(f"  Input size: {srcDataSize} elements")
    print(f"  Output size: {dstDataSize} elements")


def parse_addr_list(addr_str, dtype_size, H=None, W=None, is_nchw_mode=False):
    """
    解析地址列表字符串
    
    支持多种格式：
    1. 逗号分隔字节地址: "0,32,64,96,..."
    2. 连续block格式: "0:16" 表示从block 0开始的16个连续block
    3. NCHW格式: "nchw:n_idx,c_start_idx" 表示从NCHW形状生成的HW平面起点地址
       需要 H 和 W 参数
    
    参数:
        addr_str: 地址字符串
        dtype_size: 数据类型大小 (字节)
        H: 高度 (NCHW模式需要)
        W: 宽度 (NCHW模式需要)
        is_nchw_mode: 是否使用NCHW模式生成地址
    
    返回:
        addr_list: 16个字节地址的列表
    """
    if addr_str is None:
        return None
    
    if addr_str.startswith('nchw:'):
        # 格式: "nchw:n_idx,c_start_idx" 或 "nchw:n_idx,c_start_idx,base_addr"
        # 生成指向HW平面起点的地址
        if H is None or W is None:
            raise ValueError("NCHW mode requires H and W parameters")
        parts = addr_str[5:].split(',')
        n_idx = int(parts[0])
        c_start_idx = int(parts[1])
        base_addr = int(parts[2]) if len(parts) > 2 else 0
        return generate_addr_list_from_nchw(n_idx, c_start_idx, H, W, dtype_size, base_addr)
    
    elif addr_str.startswith('nc1hwc0:'):
        # 格式: "nc1hwc0:n_idx,c1_idx,base_addr"
        # 生成NC1HWC0格式的目标地址
        if H is None or W is None:
            raise ValueError("NC1HWC0 mode requires H and W parameters")
        parts = addr_str[8:].split(',')
        n_idx = int(parts[0])
        c1_idx = int(parts[1])
        base_addr = int(parts[2]) if len(parts) > 2 else 0
        # C0 从外部传入，这里暂时用16
        return generate_dst_addr_list_from_nc1hwc0(n_idx, c1_idx, H, W, 16, dtype_size, base_addr)
    
    elif ':' in addr_str:
        # 格式: "start:count" - 连续block
        parts = addr_str.split(':')
        start_block = int(parts[0])
        count = int(parts[1])
        block_size_bytes = ONE_BLK_SIZE
        return [start_block * block_size_bytes + i * block_size_bytes for i in range(count)]
    
    else:
        # 格式: 逗号分隔的字节地址
        return [int(x.strip()) for x in addr_str.split(',')]


def gen_golden_data_with_nchw_addr(scenarioNum, N, C, H, W, n_idx=0, c1_idx=0,
                                   repeatTimes=1, dstRepStride=16, srcRepStride=16,
                                   dstHighHalf=False, srcHighHalf=False):
    """
    根据NCHW形状自动生成地址数组并计算真值
    
    地址数组指向C维度上各HW平面的起点：
    - srcList[i] 指向输入数据中第 c1_idx*16 + i 个C通道的HW平面起点
    - dstList[j] 指向输出数据中的相应位置
    
    参数:
        scenarioNum: 场景编号
        N, C, H, W: NCHW形状
        n_idx: 当前处理的batch索引
        c1_idx: 当前处理的C1分组索引 (每组16个C通道)
        repeatTimes, dstRepStride, srcRepStride: TransDataTo5HD参数
        dstHighHalf, srcHighHalf: b8类型的高低半区控制
    """
    if scenarioNum not in SCENARIO_DTYPE_MAP:
        raise ValueError(f"Unsupported scenario: {scenarioNum}")
    
    dtype_str = SCENARIO_DTYPE_MAP[scenarioNum]
    dtype, elems_per_block, C0 = DTYPE_MAP[dtype_str]
    dtype_size = np.dtype(dtype).itemsize
    
    # C1 向上取整
    C1 = (C + C0 - 1) // C0
    
    # 生成输入数据 (NCHW格式)
    nchw_size = N * C * H * W
    if dtype_str in ['half', 'float']:
        input_data = np.arange(1, nchw_size + 1, dtype=dtype) * 0.5
    else:
        input_data = np.arange(1, nchw_size + 1, dtype=dtype)
    
    # 计算 NC1HWC0 格式的真值
    golden_nc1hwc0 = nchw_to_nc1hwc0_full(input_data, N, C, H, W, dtype_str)
    
    # 生成地址数组 (指向HW平面起点)
    # srcList: 指向输入数据中连续16个C通道的HW平面起点
    c_start_idx = c1_idx * NCHW_CONV_ADDR_LIST_SIZE
    srcList = generate_addr_list_from_nchw(n_idx, c_start_idx, H, W, dtype_size, base_addr=0)
    
    # dstList: 指向输出数据中的相应位置
    dstList = generate_dst_addr_list_from_nc1hwc0(n_idx, c1_idx, H, W, C0, dtype_size, base_addr=0)
    
    # 保存
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_data.tofile("./input/input_x.bin")
    golden_nc1hwc0.tofile("./output/golden.bin")
    
    print(f"Generated NCHW → NC1HWC0 data with auto-generated addresses:")
    print(f"  Scenario: {scenarioNum} ({dtype_str})")
    print(f"  Input shape (NCHW):  [{N}, {C}, {H}, {W}]")
    print(f"  Output shape (NC1HWC0): [{N}, {C1}, {H}, {W}, {C0}]")
    print(f"  Current processing: n_idx={n_idx}, c1_idx={c1_idx}")
    print(f"  srcList (HW plane start addrs): {srcList}")
    print(f"  dstList: {dstList}")
    print(f"  HW plane size: {H * W} elements ({H * W * dtype_size} bytes)")
    print(f"  TransDataTo5HD params:")
    print(f"    repeatTimes: {repeatTimes}")
    print(f"    dstRepStride: {dstRepStride}")
    print(f"    srcRepStride: {srcRepStride}")
    print(f"    dstHighHalf: {dstHighHalf}")
    print(f"    srcHighHalf: {srcHighHalf}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="NCHW → NC1HWC0 golden data generator")
    
    # 场景选择
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 9),
                        help='Scenario: 1=half, 2=float, 3=int16, 4=int8, 5=uint16, 6=uint32, 7=uint8, 8=int32')
    
    # NCHW维度参数
    parser.add_argument('-N', type=int, default=1, help='Batch size')
    parser.add_argument('-C', type=int, default=16, help='Channel size (C1 = ceil(C/C0))')
    parser.add_argument('-H', type=int, default=4, help='Height')
    parser.add_argument('-W', type=int, default=4, help='Width')
    
    # 地址生成参数 (用于指定当前处理的n和c1索引)
    parser.add_argument('-n_idx', type=int, default=0, help='Current batch index for address generation')
    parser.add_argument('-c1_idx', type=int, default=0, help='Current C1 group index for address generation')
    
    # TransDataTo5HD参数
    parser.add_argument('-repeatTimes', type=int, default=1, help='Repeat times')
    parser.add_argument('-dstRepStride', type=int, default=16, help='Destination repeat stride (blocks)')
    parser.add_argument('-srcRepStride', type=int, default=16, help='Source repeat stride (blocks)')
    parser.add_argument('-dstHighHalf', action='store_true', help='Use high half of dst block (b8 only)')
    parser.add_argument('-srcHighHalf', action='store_true', help='Use high half of src block (b8 only)')
    
    # Block模式参数 (uint64_t地址数组)
    parser.add_argument('-blockMode', action='store_true', 
                        help='Use block mode (specify srcList/dstList as uint64_t addresses)')
    parser.add_argument('-srcList', type=str, default=None,
                        help='16 src block byte addresses. Formats: "0,32,..." or "0:16" or "nchw:n_idx,c_start_idx"')
    parser.add_argument('-dstList', type=str, default=None,
                        help='16 dst block byte addresses. Formats: "0,32,..." or "0:16" or "nc1hwc0:n_idx,c1_idx"')
    parser.add_argument('-srcDataSize', type=int, default=256, help='Source data size in elements')
    parser.add_argument('-dstDataSize', type=int, default=None, help='Dest data size in elements (auto-calc if None)')
    
    # NCHW地址模式
    parser.add_argument('-nchwAddrMode', action='store_true',
                        help='Auto-generate addresses based on NCHW shape (pointing to HW plane starts)')
    
    args = parser.parse_args()
    
    # 获取数据类型大小
    dtype_str = SCENARIO_DTYPE_MAP[args.scenarioNum]
    dtype_size = np.dtype(DTYPE_MAP[dtype_str][0]).itemsize
    
    if args.nchwAddrMode:
        # NCHW地址模式：自动生成指向HW平面起点的地址数组
        gen_golden_data_with_nchw_addr(
            args.scenarioNum, args.N, args.C, args.H, args.W,
            args.n_idx, args.c1_idx,
            args.repeatTimes, args.dstRepStride, args.srcRepStride,
            args.dstHighHalf, args.srcHighHalf
        )
    
    elif args.blockMode:
        # 解析srcList和dstList为字节地址
        srcList = parse_addr_list(args.srcList, dtype_size, args.H, args.W)
        dstList = parse_addr_list(args.dstList, dtype_size, args.H, args.W)
        
        if srcList is None:
            # 默认: 根据NCHW形状生成地址 (指向HW平面起点)
            c_start_idx = args.c1_idx * NCHW_CONV_ADDR_LIST_SIZE
            srcList = generate_addr_list_from_nchw(args.n_idx, c_start_idx, args.H, args.W, dtype_size)
        
        if dstList is None:
            # 默认: 根据NC1HWC0形状生成地址
            C0 = DTYPE_MAP[dtype_str][2]
            dstList = generate_dst_addr_list_from_nc1hwc0(args.n_idx, args.c1_idx, args.H, args.W, C0, dtype_size)
        
        if len(srcList) < NCHW_CONV_ADDR_LIST_SIZE:
            srcList.extend([srcList[-1] + ONE_BLK_SIZE] * (NCHW_CONV_ADDR_LIST_SIZE - len(srcList)))
        if len(dstList) < NCHW_CONV_ADDR_LIST_SIZE:
            dstList.extend([dstList[-1] + ONE_BLK_SIZE] * (NCHW_CONV_ADDR_LIST_SIZE - len(dstList)))
        
        gen_golden_data_block_mode(
            args.scenarioNum, srcList, dstList,
            args.srcDataSize, args.dstDataSize,
            args.repeatTimes, args.dstRepStride, args.srcRepStride,
            args.dstHighHalf, args.srcHighHalf
        )
    
    else:
        # 默认模式：完整NCHW→NC1HWC0转换
        gen_golden_data_nchw2nc1hwc0(
            args.scenarioNum, args.N, args.C, args.H, args.W,
            args.repeatTimes, args.dstRepStride, args.srcRepStride,
            args.dstHighHalf, args.srcHighHalf
        )
