#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------


import numpy as np

def WholeReduceMax(dst, src, repeatTime, mask, dstRepStride, srcBlkStride, srcRepStride, order):
    elems_per_block = 16 # 16 for half
    blocks_per_repeat = 8

    for rep in range(repeatTime):
        repeat_data = []
        src_offset = rep * srcRepStride * elems_per_block
        dst_offset = rep * dstRepStride * 2
        for blk in range(blocks_per_repeat):
            blk_offset = src_offset + blk * srcBlkStride * elems_per_block
            block_data = src[blk_offset : blk_offset + elems_per_block]
            repeat_data.extend(block_data)
        dst[dst_offset] = max(repeat_data)
        dst[dst_offset + 1] = np.argmax(repeat_data)


if __name__ == "__main__":
    np.set_printoptions(threshold=np.inf)
    dtype= np.float16 # half数据类型
    src = np.random.uniform(-1000, 1000, 1024).astype(dtype)
    dst = np.zeros(16).astype(dtype)
    WholeReduceMax(dst, src, repeatTime=8, mask=128, dstRepStride=1, srcBlkStride=1, srcRepStride=8,\
        order=0)    # 使用默认order，获得最大值和索引
    print("dst:", dst)