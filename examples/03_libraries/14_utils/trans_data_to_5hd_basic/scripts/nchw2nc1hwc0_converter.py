#!/usr/bin/env python3
# coding=utf-8

"""
NCHW → NC1HWC0 格式转换器

核心函数:
    1. TransDataTo5HD(dstList, srcList, nchwconvParams) - 单次 scatter 转置操作
    2. NCHW2NC1HWC0(dst, src, nSize, cSize, hSize, wSize) - 完整的格式转换

转换逻辑:
    - TransDataTo5HD: 处理16个HW平面的转置，经过repeatTimes次循环完成
    - NCHW2NC1HWC0: 循环调用TransDataTo5HD，每次选取新的dstList/srcList，直到所有数据转置完成
"""

import numpy as np
from dataclasses import dataclass
from typing import List


# 数据类型映射: dtype_str -> (numpy_dtype, elements_per_block, C0_size)
DTYPE_MAP = {
    'half':   (np.float16, 16, 16),   # b16: C0=16
    'float':  (np.float32, 8, 16),    # b32: C0=16
    'int16':  (np.int16, 16, 16),     # b16: C0=16
    'uint16': (np.uint16, 16, 16),    # b16: C0=16
    'int32':  (np.int32, 8, 16),      # b32: C0=16
    'uint32': (np.uint32, 8, 16),     # b32: C0=16
    'int8':   (np.int8, 32, 32),      # b8:  C0=32
    'uint8':  (np.uint8, 32, 32),     # b8:  C0=32
}

# 常量
NCHW_CONV_ADDR_LIST_SIZE = 16  # 地址列表大小
ONE_BLK_SIZE = 32               # 每个block 32字节


@dataclass
class NchwConvParams:
    """
    TransDataTo5HD 接口参数结构体
    
    与 C++ TransDataTo5HDParams 对应:
        - dstHighHalf: 目标block使用高半区 (仅b8类型)
        - srcHighHalf: 源block使用高半区 (仅b8类型)
        - repeatTimes: 重复次数
        - dstRepStride: 目标重复步长 (block数)
        - srcRepStride: 源重复步长 (block数)
    """
    repeatTimes: int = 1
    dstRepStride: int = 16
    srcRepStride: int = 16
    dstHighHalf: bool = False
    srcHighHalf: bool = False
    dtype: str = 'half'  # 数据类型字符串


def get_elem_index(byte_addr: int, dtype_size: int) -> int:
    """根据字节地址计算元素索引"""
    return byte_addr // dtype_size


def TransDataTo5HD(dst: np.ndarray, src: np.ndarray,
                   dstList: List[int], srcList: List[int],
                   nchwconvParams: NchwConvParams) -> None:
    """
    单次 TransDataTo5HD scatter 操作
    
    功能: 将 srcList[0..15] 指向的 16 个 HW 平面数据，
          通过 scatter 转置写入 dstList[0..15] 指向的位置。
    
    转换语义:
        - 输入: 16 个 HW 平面，每个平面 H*W 个元素
        - scatter 转置后: 将 C 维度的 16 个通道重排到 C0 维度
        
    参数:
        dst: 输出数据数组 (1D, 就地修改)
        src: 输入数据数组 (1D)
        dstList: 16 个目标 block 的字节地址 [uint64_t]
        srcList: 16 个源 block 的字节地址 [uint64_t]
        nchwconvParams: TransDataTo5HD 参数结构体
    
    示例:
        # 处理16个HW平面，repeatTimes=H*W次完成所有位置
        params = NchwConvParams(repeatTimes=256, dtype='half')
        TransDataTo5HD(dst, src, dstList, srcList, params)
    """
    dtype_str = nchwconvParams.dtype
    dtype, elems_per_block, C0 = DTYPE_MAP[dtype_str]
    dtype_size = np.dtype(dtype).itemsize
    
    repeatTimes = nchwconvParams.repeatTimes
    dstRepStride = nchwconvParams.dstRepStride
    srcRepStride = nchwconvParams.srcRepStride
    dstHighHalf = nchwconvParams.dstHighHalf
    srcHighHalf = nchwconvParams.srcHighHalf
    
    # 对每次 repeat 执行 scatter
    for rep in range(repeatTimes):
        src_block_offset = rep * srcRepStride
        dst_block_offset = rep * dstRepStride
        
        if dtype_str in ['int8', 'uint8']:
            # b8 类型: 每个 block 32 元素
            # scatter 循环 16 次，每次从 16 个 src block 各取 1 个元素，组成 dstList[j] 的 16 个元素
            # 但 b8 的 scatter 输出每个 block 只有 16 元素（半块）
            # 
            # 对于 NCHW→NC1HWC0 (b8), C0=32:
            #   NC1HWC0[n, c1, h, w, 0..15] = NCHW[n, c1*32+0..15, h, w]
            #   NC1HWC0[n, c1, h, w, 16..31] = NCHW[n, c1*32+16..31, h, w]
            # 
            # scatter[j] 输出:
            #   dstList[j] = [src[0][j], src[1][j], ..., src[15][j]] (16 元素)
            # 
            # 所以:
            #   dstList[j] → NC1HWC0[n, c1, 0, j, 0..15] (低半块)
            #   高半块 NC1HWC0[n, c1, 0, j, 16..31] 需要另一次 scatter (使用 srcHighHalf=True)
            for j in range(16):  # scatter 循环 16 次
                for i in range(16):  # 16 个源 block
                    src_base_idx = get_elem_index(srcList[i], dtype_size)
                    src_block_offset_actual = src_block_offset * elems_per_block
                    
                    # 读取源数据
                    if srcHighHalf:
                        src_elem_idx = src_base_idx + src_block_offset_actual + 16 + j
                    else:
                        src_elem_idx = src_base_idx + src_block_offset_actual + j
                    
                    dst_base_idx = get_elem_index(dstList[j], dtype_size)
                    dst_block_offset_actual = dst_block_offset * elems_per_block
                    
                    # 写入目标位置
                    if dstHighHalf:
                        dst_elem_idx = dst_base_idx + dst_block_offset_actual + 16 + i
                    else:
                        dst_elem_idx = dst_base_idx + dst_block_offset_actual + i
                    
                    if src_elem_idx < len(src) and dst_elem_idx < len(dst):
                        dst[dst_elem_idx] = src[src_elem_idx]
        
        else:
            # b16 类型: 每个 block 16 元素，scatter 循环 16 次
            # b32 类型: 每个 block 8 元素，scatter 循环 8 次
            num_loops = elems_per_block  # b16=16, b32=8
            
            for j in range(num_loops):
                for i in range(16):
                    # 源地址
                    src_base_idx = get_elem_index(srcList[i], dtype_size)
                    src_block_offset_actual = src_block_offset * elems_per_block
                    src_elem_idx = src_base_idx + src_block_offset_actual + j
                    
                    # 目标地址
                    if dtype_str in ['half', 'int16', 'uint16']:
                        # b16: dstList[j] 的第 i 个位置
                        # scatter[j] = [src[0][j], src[1][j], ..., src[15][j]]
                        dst_base_idx = get_elem_index(dstList[j], dtype_size)
                        dst_block_offset_actual = dst_block_offset * elems_per_block
                        dst_elem_idx = dst_base_idx + dst_block_offset_actual + i
                    else:
                        # b32: 每次循环产生 2 个 block，每个 8 元素
                        # scatter[j] 输出:
                        #   dstList[2*j] = [src[0][j], src[1][j], ..., src[7][j]]
                        #   dstList[2*j+1] = [src[8][j], src[9][j], ..., src[15][j]]
                        # 
                        # 对于 NCHW→NC1HWC0，C0=16，需要将两个 8 元素 block 组合成 16 元素 C0:
                        #   dstList[2*w] → NC1HWC0[n,c1,h,w,0..7]
                        #   dstList[2*w+1] → NC1HWC0[n,c1,h,w,8..15]
                        if i < 8:
                            # 前 8 个 src block → dstList[2*j]
                            dst_base_idx = get_elem_index(dstList[2 * j], dtype_size)
                            dst_elem_idx = dst_base_idx + dst_block_offset * elems_per_block + i
                        else:
                            # 后 8 个 src block → dstList[2*j+1]
                            dst_base_idx = get_elem_index(dstList[2 * j + 1], dtype_size)
                            dst_elem_idx = dst_base_idx + dst_block_offset * elems_per_block + (i - 8)
                    
                    if src_elem_idx < len(src) and dst_elem_idx < len(dst):
                        dst[dst_elem_idx] = src[src_elem_idx]


def NCHW2NC1HWC0(dst: np.ndarray, src: np.ndarray,
                 nSize: int, cSize: int, hSize: int, wSize: int,
                 dtype: str = 'half',
                 repeatTimes: int = None,
                 dstRepStride: int = None,
                 srcRepStride: int = None) -> None:
    """
    完整的 NCHW → NC1HWC0 格式转换
    
    转换逻辑:
        1. 计算转换所需的参数 (C0, C1, repeatTimes 等)
        2. 按 (N, C1) 分组，每组 C0 个 C 通道
        3. 对每组按 W 分段调用 TransDataTo5HD 完成转置
        
    scatter 操作语义:
        - 输入: 16 个 HW 平面 (srcList[0..15] 指向各平面起点)
        - scatter 循环 elems_per_block 次，每次处理一个 W 位置
        - 第 j 次循环: 从 16 个 src block 的第 j 个元素取出，组成 dstList[j] 的 16 个元素
        - repeat 参数用于处理 H 维度
        
    参数:
        dst: 输出数据数组 (NC1HWC0 格式, 1D, 就地修改)
        src: 输入数据数组 (NCHW 格式, 1D)
        nSize, cSize, hSize, wSize: 数据维度
        dtype: 数据类型字符串
        repeatTimes: 重复次数 (默认 = H，每次 repeat 处理一行)
        dstRepStride: 目标重复步长 (block 数)
        srcRepStride: 源重复步长 (block 数)
    
    内存布局:
        - NCHW: [N, C, H, W] - 数据按 N-C-H-W 顺序连续存储
        - NC1HWC0: [N, C1, H, W, C0] - C0 维度的数据被分块重排
            C0 = 16 (b16/b32) 或 32 (b8)
            C1 = (C + C0 - 1) // C0 (向上取整)
    
    W 维度分段处理:
        - b16: scatter 循环 16 次，每次处理 1 个 W 位置，共 16 个 W 位置
        - b32: scatter 循环 8 次，每次处理 1 个 W 位置，共 8 个 W 位置
        - 若 wSize > scatter 处理范围，需多次调用 TransDataTo5HD
    """
    _, elems_per_block, C0 = DTYPE_MAP[dtype]
    dtype_size = np.dtype(DTYPE_MAP[dtype][0]).itemsize
    
    # C1 向上取整
    C1 = (cSize + C0 - 1) // C0
    
    # scatter 循环次数 = elems_per_block (b16=16, b32=8)
    # 每次 scatter 处理的 W 位置数 = elems_per_block
    w_per_scatter = elems_per_block
    
    # b16/b32 类型: 使用 scatter 转置
    # b8 类型: 使用直接内存复制
    if dtype in ['int8', 'uint8']:
        # b8 类型: C0=32，scatter 输出每个 block 只有 16 元素（半块）
        # 使用直接内存复制
        np_dtype = DTYPE_MAP[dtype][0]
        src_reshaped = src.reshape(nSize, cSize, hSize, wSize)
        dst_reshaped = dst.reshape(nSize, C1, hSize, wSize, C0)
        
        for n in range(nSize):
            for c1 in range(C1):
                c_start = c1 * C0
                for h in range(hSize):
                    for w in range(wSize):
                        for c0 in range(C0):
                            c = c_start + c0
                            if c < cSize:
                                dst_reshaped[n, c1, h, w, c0] = src_reshaped[n, c, h, w]
        return
    
    # b16/b32 类型: 使用 scatter 转置
    # 默认参数:
    # - repeatTimes = H (每次 repeat 处理一行，共 H 行)
    # - srcRepStride = W / elems_per_block (每次 repeat 移动 W 个元素)
    # - dstRepStride = W (每次 repeat 移动 W*C0 个元素 = W*C0/elems_per_block 个 block)
    if repeatTimes is None:
        repeatTimes = hSize
    if srcRepStride is None:
        srcRepStride = wSize // elems_per_block
    if dstRepStride is None:
        # dstRepStride = W * C0 / elems_per_block
        dstRepStride = wSize * C0 // elems_per_block
    
    # 按 (N, C1) 分组处理
    for n in range(nSize):
        for c1 in range(C1):
            # 计算当前组的起始 C 通道索引
            c_start = c1 * C0
            
            # 按 W 分段处理
            # b16: 每次 scatter 处理 16 个 W 位置 (j=0..15)
            # b32: 每次 scatter 处理 8 个 W 位置 (j=0..7)
            num_w_segments = (wSize + w_per_scatter - 1) // w_per_scatter
            
            for w_seg in range(num_w_segments):
                w_start = w_seg * w_per_scatter
                w_count = min(w_per_scatter, wSize - w_start)
                
                # 生成 srcList: 指向输入数据中 16 个 HW 平面的当前 W 分段起点
                # srcList[i] 指向 NCHW[n, c_start+i, 0, w_start] 的位置
                srcList = []
                for i in range(NCHW_CONV_ADDR_LIST_SIZE):
                    c_idx = c_start + i
                    # NCHW 元素索引 = n * C * H * W + c * H * W + 0 * W + w_start
                    elem_idx = n * cSize * hSize * wSize + c_idx * hSize * wSize + w_start
                    byte_addr = elem_idx * dtype_size
                    srcList.append(byte_addr)
                
                # 生成 dstList: 指向输出数据中的相应位置
                # 
                # b16 类型 (elems_per_block=16):
                #   scatter[j] 产生 dstList[j]，包含 16 个元素
                #   dstList[j] 指向 NC1HWC0[n, c1, h, w_start+j, 0]
                # 
                # b32 类型 (elems_per_block=8):
                #   scatter[j] 产生 dstList[2*j] 和 dstList[2*j+1]，各 8 个元素
                #   dstList[2*j] 指向 NC1HWC0[n, c1, h, w_start+j, 0] (C0 维度的前 8 个)
                #   dstList[2*j+1] 指向 NC1HWC0[n, c1, h, w_start+j, 8] (C0 维度的后 8 个)
                
                dstList = []
                if dtype in ['half', 'int16', 'uint16']:
                    # b16: dstList[j] → NC1HWC0[n, c1, 0, w_start+j, 0]
                    for j in range(w_count):
                        w = w_start + j
                        elem_idx = n * C1 * hSize * wSize * C0 + c1 * hSize * wSize * C0 + w * C0
                        byte_addr = elem_idx * dtype_size
                        dstList.append(byte_addr)
                    
                    # 补齐到 16 个地址（scatter 需要 16 个 dstList 元素）
                    # 但我们只处理 w_count 个位置，多余的 dstList 不会写入（scatter 循环只到 w_count）
                    # 注意: TransDataTo5HD 中的 scatter 循环次数是 elems_per_block=16
                    # 我们需要控制 scatter 只处理 w_count 次
                    
                    # 修改 params 使 scatter 只处理 w_count 次
                    # 但这需要修改 TransDataTo5HD 的接口...
                    # 
                    # 另一种方法: 保持 scatter 循环 16 次，但 dstList[j>=w_count] 指向无效位置
                    # 更好的方法: 让 TransDataTo5HD 支持自定义循环次数
                    
                    # 暂时使用: 只在 wSize >= w_per_scatter 时使用 scatter
                    # 对于 wSize < w_per_scatter 的情况，使用直接内存复制
                    
                    if w_count < w_per_scatter:
                        # wSize < 16 (b16) 或 wSize < 8 (b32): 使用直接内存复制
                        np_dtype = DTYPE_MAP[dtype][0]
                        src_reshaped = src.reshape(nSize, cSize, hSize, wSize)
                        dst_reshaped = dst.reshape(nSize, C1, hSize, wSize, C0)
                        
                        for h in range(hSize):
                            for w in range(wSize):
                                for c0 in range(C0):
                                    c = c_start + c0
                                    if c < cSize:
                                        dst_reshaped[n, c1, h, w, c0] = src_reshaped[n, c, h, w]
                        # 跳过 scatter 处理
                        break
                    
                    # 补齐 dstList 到 16 个
                    while len(dstList) < NCHW_CONV_ADDR_LIST_SIZE:
                        # 使用最后一个有效地址（这些位置不会被 scatter 写入）
                        dstList.append(dstList[-1] if dstList else 0)
                    
                    # 创建参数结构体
                    params = NchwConvParams(
                        repeatTimes=repeatTimes,
                        dstRepStride=dstRepStride,
                        srcRepStride=srcRepStride,
                        dtype=dtype
                    )
                    
                    # 调用 TransDataTo5HD 完成这一组的转置
                    TransDataTo5HD(dst, src, dstList, srcList, params)
                
                elif dtype in ['float', 'int32', 'uint32']:
                    # b32: dstList[2*j] → NC1HWC0[n, c1, 0, w_start+j, 0]
                    #      dstList[2*j+1] → NC1HWC0[n, c1, 0, w_start+j, 8]
                    #      scatter 只处理 8 个 W 位置
                    
                    if w_count < w_per_scatter:
                        # wSize < 8: 使用直接内存复制
                        np_dtype = DTYPE_MAP[dtype][0]
                        src_reshaped = src.reshape(nSize, cSize, hSize, wSize)
                        dst_reshaped = dst.reshape(nSize, C1, hSize, wSize, C0)
                        
                        for h in range(hSize):
                            for w in range(wSize):
                                for c0 in range(C0):
                                    c = c_start + c0
                                    if c < cSize:
                                        dst_reshaped[n, c1, h, w, c0] = src_reshaped[n, c, h, w]
                        break
                    
                    for j in range(w_count):
                        w = w_start + j
                        # dstList[2*j]
                        elem_idx = n * C1 * hSize * wSize * C0 + c1 * hSize * wSize * C0 + w * C0
                        byte_addr = elem_idx * dtype_size
                        dstList.append(byte_addr)
                        # dstList[2*j+1]
                        elem_idx = n * C1 * hSize * wSize * C0 + c1 * hSize * wSize * C0 + w * C0 + 8
                        byte_addr = elem_idx * dtype_size
                        dstList.append(byte_addr)
                    
                    # 补齐 dstList 到 16 个
                    while len(dstList) < NCHW_CONV_ADDR_LIST_SIZE:
                        dstList.append(dstList[-1] if dstList else 0)
                    
                    # 创建参数结构体
                    params = NchwConvParams(
                        repeatTimes=repeatTimes,
                        dstRepStride=dstRepStride,
                        srcRepStride=srcRepStride,
                        dtype=dtype
                    )
                    
                    # 调用 TransDataTo5HD 完成这一组的转置
                    TransDataTo5HD(dst, src, dstList, srcList, params)


# ============================================================================
# 辅助函数: 创建数据、打印结果
# ============================================================================

def create_nchw_data(nSize: int, cSize: int, hSize: int, wSize: int,
                     dtype: str = 'half', pattern: str = 'sequential') -> np.ndarray:
    """
    创建 NCHW 格式的测试数据
    
    参数:
        pattern: 
            'sequential' - 顺序递增
            'hw_plane' - 每个 HW 平面用相同值填充
    """
    np_dtype = DTYPE_MAP[dtype][0]
    total_size = nSize * cSize * hSize * wSize
    
    if pattern == 'hw_plane':
        data = np.zeros(total_size, dtype=np_dtype)
        for n in range(nSize):
            for c in range(cSize):
                hw_plane_idx = n * cSize + c
                start = hw_plane_idx * hSize * wSize
                end = start + hSize * wSize
                data[start:end] = hw_plane_idx
    else:
        data = np.arange(1, total_size + 1, dtype=np_dtype) * 0.5 if dtype in ['half', 'float'] \
              else np.arange(1, total_size + 1, dtype=np_dtype)
    
    return data


def print_nchw_data(data: np.ndarray, nSize: int, cSize: int, hSize: int, wSize: int,
                    dtype: str = 'half', max_elements: int = None):
    """打印 NCHW 格式数据"""
    np_dtype = DTYPE_MAP[dtype][0]
    data = data.astype(np_dtype).reshape(nSize, cSize, hSize, wSize)
    
    print(f"NCHW Shape: [{nSize}, {cSize}, {hSize}, {wSize}]")
    
    result = "["
    for n in range(nSize):
        if n > 0:
            result += ","
        result += "["
        for c in range(cSize):
            if c > 0:
                result += ","
            result += "["
            for h in range(hSize):
                if h > 0:
                    result += ","
                result += str(list(data[n, c, h, :].astype(int)))
            result += "]"
        result += "]"
    result += "]"
    
    print(result)


def print_nc1hwc0_data(data: np.ndarray, nSize: int, cSize: int, hSize: int, wSize: int,
                       dtype: str = 'half'):
    """打印 NC1HWC0 格式数据"""
    np_dtype = DTYPE_MAP[dtype][0]
    C0 = DTYPE_MAP[dtype][2]
    C1 = (cSize + C0 - 1) // C0
    
    data = data.astype(np_dtype).reshape(nSize, C1, hSize, wSize, C0)
    
    print(f"NC1HWC0 Shape: [{nSize}, {C1}, {hSize}, {wSize}, {C0}]")
    
    result = "["
    for n in range(nSize):
        if n > 0:
            result += ","
        result += "["
        for c1 in range(C1):
            if c1 > 0:
                result += ","
            result += "["
            for h in range(hSize):
                if h > 0:
                    result += ","
                result += "["
                for w in range(wSize):
                    if w > 0:
                        result += ","
                    result += str(list(data[n, c1, h, w, :].astype(int)))
                result += "]"
            result += "]"
        result += "]"
    result += "]"
    
    print(result)


# ============================================================================
# 测试入口
# ============================================================================

if __name__ == "__main__":
    import argparse
    
    parser = argparse.ArgumentParser(description="NCHW → NC1HWC0 Converter")
    parser.add_argument('-N', type=int, default=2, help='Batch size')
    parser.add_argument('-C', type=int, default=32, help='Channel size')
    parser.add_argument('-H', type=int, default=16, help='Height')
    parser.add_argument('-W', type=int, default=16, help='Width')
    parser.add_argument('-dtype', type=str, default='half', 
                        choices=['half', 'float', 'int16', 'int32', 'int8', 'uint16', 'uint32', 'uint8'],
                        help='Data type')
    parser.add_argument('-pattern', type=str, default='hw_plane',
                        choices=['sequential', 'hw_plane'],
                        help='Data pattern')
    parser.add_argument('-print', action='store_true', help='Print input/output data')
    
    args = parser.parse_args()
    
    # 获取类型信息
    np_dtype, elems_per_block, C0 = DTYPE_MAP[args.dtype]
    C1 = (args.C + C0 - 1) // C0
    
    print("=" * 70)
    print("NCHW → NC1HWC0 转换测试")
    print("=" * 70)
    print(f"输入 NCHW:    [{args.N}, {args.C}, {args.H}, {args.W}]")
    print(f"输出 NC1HWC0: [{args.N}, {C1}, {args.H}, {args.W}, {C0}]")
    print(f"数据类型:     {args.dtype}")
    print(f"C0 = {C0}, C1 = {C1}")
    print()
    
    # 创建输入数据
    src = create_nchw_data(args.N, args.C, args.H, args.W, args.dtype, args.pattern)
    
    # 创建输出数据
    dst_size = args.N * C1 * args.H * args.W * C0
    dst = np.zeros(dst_size, dtype=np_dtype)
    
    # 打印输入数据
    if args.print:
        print("-" * 70)
        print("输入数据 (NCHW):")
        print_nchw_data(src, args.N, args.C, args.H, args.W, args.dtype)
        print()
    
    # 执行转换
    print("-" * 70)
    print("执行 NCHW2NC1HWC0 转换...")
    NCHW2NC1HWC0(dst, src, args.N, args.C, args.H, args.W, args.dtype)
    print("转换完成!")
    print()
    
    # 打印输出数据
    if args.print:
        print("-" * 70)
        print("输出数据 (NC1HWC0):")
        print_nc1hwc0_data(dst, args.N, args.C, args.H, args.W, args.dtype)
        print()
    
    # 验证结果
    print("-" * 70)
    print("验证结果:")
    src_reshape = src.reshape(args.N, args.C, args.H, args.W)
    dst_reshape = dst.reshape(args.N, C1, args.H, args.W, C0)
    
    # 验证关键位置
    test_cases = [
        (0, 0, 0, 0),                     # N=0, C=0
        (0, min(15, args.C-1), 0, 0),     # N=0, C=15 or max C
        (0, min(16, args.C-1), 0, 0),     # N=0, C=16 (if C>16)
        (0, min(31, args.C-1), 0, 0),     # N=0, C=31 (if C>31)
        (min(1, args.N-1), 0, 0, 0),      # N=1 (if N>1), C=0
        (min(1, args.N-1), min(31, args.C-1), args.H-1, args.W-1),  # Last element
    ]
    
    all_passed = True
    for n, c, h, w in test_cases:
        if c >= args.C or n >= args.N or h >= args.H or w >= args.W:
            continue
        
        c1 = c // C0
        c0 = c % C0
        
        in_val = int(src_reshape[n, c, h, w])
        out_val = int(dst_reshape[n, c1, h, w, c0])
        
        status = "OK" if in_val == out_val else "FAIL"
        if in_val != out_val:
            all_passed = False
        
        print(f"  {status} NCHW[{n},{c},{h},{w}]={in_val:3d} -> "
              f"NC1HWC0[{n},{c1},{h},{w},{c0}]={out_val:3d}")
    
    print()
    if all_passed:
        print("验证通过! NCHW → NC1HWC0 转换正确。")
    else:
        print("验证失败! 存在转换错误。")
