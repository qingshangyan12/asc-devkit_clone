#!/usr/bin/env python3
"""
Generate test data and golden for block_reduce_max comprehensive example
"""

import numpy as np
import struct
import argparse
import os

def block_reduce_max_golden(src, dst, dtype, repeat, tailMask, srcBlkStride, dstRepStride, srcRepStride, enabelCounterMask=False):
    """Compute golden result for BlockReduceMax"""

    if dtype == np.float16:
        elems_per_block = 16
    else:
        elems_per_block = 8
    NUM_BLOCKS = 8
    
    if enabelCounterMask:
        # 忽略repeat、srcBlkStride等参数，只处理前tailMask个元素
        processedElems = 0
        dstEleNum = 0
        while processedElems < tailMask:
            elems_in_block = min(elems_per_block, tailMask - processedElems)
            block_data = src[processedElems:processedElems + elems_in_block]
            block_max = np.max(block_data)
            dst[dstEleNum] = block_max
            processedElems += elems_in_block
            dstEleNum += 1
    else:
        for rep_idx in range(repeat):
            src_offset = rep_idx * srcRepStride * elems_per_block
            dst_offset = rep_idx * dstRepStride * NUM_BLOCKS
            for blk_idx in range(NUM_BLOCKS):
                src_blk_offset = src_offset + blk_idx * srcBlkStride * elems_per_block
                print(f"src_blk_offset: {src_blk_offset}")
                block_data = src[src_blk_offset:src_blk_offset + elems_per_block]
                block_max = np.max(block_data)
                dst[dst_offset + blk_idx] = block_max
        
        if tailMask > 0:
            tail_blocks = (tailMask + elems_per_block - 1) // elems_per_block
            src_offset = repeat * srcRepStride * elems_per_block
            dst_offset = repeat * dstRepStride * NUM_BLOCKS
            for blk_idx in range(tail_blocks):
                src_blk_offset = src_offset + blk_idx * srcBlkStride * elems_per_block

                if blk_idx == tail_blocks - 1:
                    elems_in_block = tailMask - blk_idx * elems_per_block
                else:
                    elems_in_block = elems_per_block

                block_data = src[src_blk_offset:src_blk_offset + elems_in_block]
                block_max = np.max(block_data)
                dst[dst_offset + blk_idx] = block_max
    return dst

def main():
    parser = argparse.ArgumentParser(description='Generate test data for BlockReduceMax')
    parser.add_argument('--scenario', type=int, default=1, choices=range(1, 7),
                       help='Scenario number (1-4)')
    parser.add_argument('--repeatTime', type=int, help='Repeat times')
    parser.add_argument('--tailMask', type=int, help='Tail mask value')
    parser.add_argument('--src_size', type=int, help='Source data size')
    parser.add_argument('--dst_size', type=int, help='Destination data size')
    parser.add_argument('--srcBlkStride', type=int, help='Source block stride')
    parser.add_argument('--dstRepStride', type=int, help='Destination repeat stride') 
    parser.add_argument('--srcRepStride', type=int, help='Source repeat stride')

    args = parser.parse_args()

    scenario = args.scenario
    repeatTime = args.repeatTime
    tailMask = args.tailMask
    src_size = args.src_size
    dst_size = args.dst_size
    srcBlkStride = args.srcBlkStride
    dstRepStride = args.dstRepStride
    srcRepStride = args.srcRepStride

    if scenario <= 3:
        dtype = np.float16
    else:
        dtype = np.float32

    print(f"Scenario {scenario}")
    print(f"  Type: {dtype}")
    print(f"  RepeatTime: {repeatTime}")
    print(f"  TailMask: {tailMask}")
    print(f"  srcBlkStride: {srcBlkStride}")
    print(f"  dstRepStride: {dstRepStride}")
    print(f"  srcRepStride: {srcRepStride}")
    print(f"  Input size: {src_size}")
    print(f"  Output size: {dst_size}")
    os.makedirs('input', exist_ok=True)
    os.makedirs('output', exist_ok=True)

    np.random.seed(42)

    src_data = np.random.uniform(-10.0, 10.0, src_size).astype(dtype)
    print(src_data)
    print("-----------------------")
    golden_data = np.zeros(dst_size, dtype=dtype)
    print(golden_data)
    with open(f'input/input_src.bin', 'wb') as f:
        f.write(src_data.tobytes())

    block_reduce_max_golden(src_data, golden_data, 
        dtype, repeatTime, tailMask, srcBlkStride, dstRepStride, srcRepStride,
        enabelCounterMask=(scenario != 1 and scenario != 4)
    )
    print("-----------------------")
    print(golden_data)

    with open(f'output/golden.bin', 'wb') as f:
        f.write(golden_data.tobytes())

if __name__ == '__main__':
    main()
