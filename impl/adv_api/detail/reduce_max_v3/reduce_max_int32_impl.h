/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/**
 * @file reduce_max_int32_impl.h
 * @brief int32_t ReduceMax implementation using vmax binary reduction.
 * 
 * Since vcgmax does not support int32_t, we use vmax (dual-input comparison)
 * with a binary reduction strategy.
 * 
 * Key constants for int32_t (4 bytes per element):
 * - ONE_BLK_SIZE = 32 bytes = 8 int32_t elements per block
 * - ONE_REPEAT_BYTE_SIZE = 256 bytes = 64 int32_t elements per repeat
 * - DEFAULT_BLK_NUM = 8 blocks per repeat
 * 
 * vmax instruction signature:
 * vmax(dst, src0, src1, repeatTime, 
 *      dstBlkStride, src0BlkStride, src1BlkStride, 
 *      dstRepStride, src0RepStride, src1RepStride)
 */

#ifndef IMPL_REDUCE_MAX_V3_REDUCE_MAX_INT32_IMPL_H_
#define IMPL_REDUCE_MAX_V3_REDUCE_MAX_INT32_IMPL_H_

// Include necessary interfaces
#include "kernel_operator_vec_binary_scalar_intf_impl.h"  // For Adds
#include "kernel_operator_vec_brcb_intf_impl.h"     // For Brcb
#include "kernel_basic_intf.h"                       // For SetMaskNorm, ResetMask, PipeBarrier

namespace AscendC {
namespace Internal {

// ============================================================================
// Constants for int32_t reduction
// ============================================================================
constexpr uint32_t INT32_BYTES = 4;
constexpr uint32_t INT32_ELEM_PER_BLK = ONE_BLK_SIZE / INT32_BYTES;       // 8 elements per block
constexpr uint32_t INT32_ELEM_PER_REPEAT = ONE_REPEAT_BYTE_SIZE / INT32_BYTES;  // 64 elements per repeat

// ============================================================================
// VMaxImpl - Core function for vmax instruction with count
// ============================================================================
// Performs: dst = vmax(src0, src1) element-wise for `count` elements
// Uses set_mask_count mode to handle arbitrary element count
template <typename T>
__aicore__ inline void VMaxImpl(const LocalTensor<T>& dst,
                                 const LocalTensor<T>& src0,
                                 const LocalTensor<T>& src1,
                                 uint32_t count)
{
    // Use count mode mask
    SetMaskCount();
    SetVectorMask<T>(0, count);
    
    // Call vmax instruction with default strides
    // vmax(dst, src0, src1, repeatTime, 
    //      dstBlkStride, src0BlkStride, src1BlkStride, 
    //      dstRepStride, src0RepStride, src1RepStride)
    vmax((__ubuf__ T*)dst.GetPhyAddr(), 
         (__ubuf__ T*)src0.GetPhyAddr(), 
         (__ubuf__ T*)src1.GetPhyAddr(),
         1,  // repeatTime = 1 (use mask to control element count)
         DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE,
         DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE);
    
    // Reset to normal mask mode
    SetMaskNorm();
    ResetMask();
}

// ============================================================================
// BinaryReduceMax - Core function for binary reduction using vmax
// ============================================================================
// Performs: dst = vmax(src0, src1) element-wise
// This is used as the building block for binary reduction
template <typename T>
__aicore__ inline void BinaryReduceMax(const LocalTensor<T>& dst,
                                        const LocalTensor<T>& src0,
                                        const LocalTensor<T>& src1,
                                        uint32_t count)
{
    VMaxImpl<T>(dst, src0, src1, count);
}

// ============================================================================
// BinaryReduceHalf - Reduce by comparing first half with second half
// ============================================================================
// Input:  src with `count` elements (must be power of 2)
// Output: dst with count/2 elements (max of each pair)
// 
// Example: count=8, src=[a,b,c,d,e,f,g,h]
//          dst = [max(a,e), max(b,f), max(c,g), max(d,h)]
//          (comparing elements offset by count/2)
template <typename T>
__aicore__ inline void BinaryReduceHalf(const LocalTensor<T>& dst,
                                         const LocalTensor<T>& src,
                                         uint32_t count)
{
    // Validate: count must be power of 2 and at least 2
    // Compare first half with second half
    uint32_t halfCount = count / 2;
    
    // src0 points to first half, src1 points to second half
    // dst[i] = max(src[i], src[i + halfCount])
    LocalTensor<T> src0 = src;
    LocalTensor<T> src1 = src[halfCount];
    
    BinaryReduceMax<T>(dst, src0, src1, halfCount);
}

// ============================================================================
// BinaryReduceToSingle - Repeatedly apply binary reduction until single value
// ============================================================================
// Input:  src with `count` elements (must be power of 2)
// Output: dst[0] contains the maximum value
// Uses tmpBuffer for intermediate results
// Note: count must be multiple of 8 (32-byte aligned for int32_t)
// IMPORTANT: This function does NOT use recursion (Ascend C does not support it)
template <typename T>
__aicore__ inline void BinaryReduceToSingle(const LocalTensor<T>& dst,
                                             const LocalTensor<T>& src,
                                             const LocalTensor<T>& tmpBuffer,
                                             uint32_t count)
{
    if (count <= 1) {
        // Already single element or empty
        if (count == 1) {
            dst.SetValue(0, src.GetValue(0));
        }
        return;
    }
    
    // Special case: exactly 8 elements - reduce directly
    if (count == 8) {
        // Reduce 8 elements to 1 using binary reduction
        // Step 1: Compare elements 0-3 with 4-7, result in tmpBuffer[0-3]
        VMaxImpl<T>(tmpBuffer, src, src[4], 4);
        PipeBarrier<PIPE_V>();
        
        // Step 2: Compare elements 0-1 with 2-3, result in tmpBuffer[0-1]
        VMaxImpl<T>(tmpBuffer, tmpBuffer, tmpBuffer[2], 2);
        PipeBarrier<PIPE_V>();
        
        // Step 3: Compare element 0 with 1, result in tmpBuffer[0]
        VMaxImpl<T>(tmpBuffer, tmpBuffer, tmpBuffer[1], 1);
        PipeBarrier<PIPE_V>();
        
        // Copy result to dst using SetValue/GetValue for single element
        dst.SetValue(0, tmpBuffer.GetValue(0));
        return;
    }
    
    // Multi-stage binary reduction for larger counts
    // Use iterative approach (NO recursion allowed in Ascend C)
    LocalTensor<T> currentSrc = src;
    LocalTensor<T> currentDst = tmpBuffer;
    uint32_t currentCount = count;
    
    // Ping-pong between buffers
    bool useTmpDst = true;
    
    // Continue until we have <= 8 elements
    while (currentCount > 8) {
        uint32_t halfCount = currentCount / 2;
        
        // Perform binary reduction: compare first half with second half
        LocalTensor<T> src0 = currentSrc;
        LocalTensor<T> src1 = currentSrc[halfCount];
        
        BinaryReduceMax<T>(currentDst, src0, src1, halfCount);
        
        PipeBarrier<PIPE_V>();
        
        // Update count
        currentCount = halfCount;
        
        // Swap buffers for next iteration
        if (useTmpDst) {
            currentSrc = tmpBuffer;
            currentDst = dst;  // Use dst as ping-pong buffer
        } else {
            currentSrc = dst;
            currentDst = tmpBuffer;
        }
        useTmpDst = !useTmpDst;
    }
    
    // Final step: reduce remaining elements (<=8) to 1
    // Do NOT call BinaryReduceToSingle recursively - handle directly
    if (currentCount == 1) {
        // Already single element
        if (useTmpDst) {
            // Result is in tmpBuffer
            dst.SetValue(0, tmpBuffer.GetValue(0));
        } else {
            // Result is in dst
            // Already in place, nothing to do
        }
    } else {
        // currentCount is 2, 4, or 8 - reduce to 1
        LocalTensor<T> finalSrc = useTmpDst ? tmpBuffer : dst;
        LocalTensor<T> finalTmp = useTmpDst ? dst : tmpBuffer;
        
        // Perform binary reduction for final elements
        while (currentCount > 1) {
            uint32_t halfCount = currentCount / 2;
            
            VMaxImpl<T>(finalTmp, finalSrc, finalSrc[halfCount], halfCount);
            PipeBarrier<PIPE_V>();
            
            // Swap for next iteration
            LocalTensor<T> temp = finalSrc;
            finalSrc = finalTmp;
            finalTmp = temp;
            
            currentCount = halfCount;
        }
        
        // Copy result to dst
        dst.SetValue(0, finalSrc.GetValue(0));
    }
}

// ============================================================================
// BlockLevelReduce - Reduce 8 values using Brcb + Max
// ============================================================================
// Input:  src with 8 elements (one from each block position)
// Output: dst[0] contains the maximum value
// 
// Process:
// 1. Use Brcb to broadcast 8 values to 8 blocks (each block has one value duplicated)
// 2. Use Max to find the maximum across blocks
template <typename T>
__aicore__ inline void BlockLevelReduce(const LocalTensor<T>& dst,
                                         const LocalTensor<T>& src,
                                         const LocalTensor<T>& tmpBuffer)
{
    // Step 1: Use Brcb to broadcast 8 elements to 8 blocks
    // Brcb takes 8 elements and broadcasts each to a 32-byte block
    // Result: 8 blocks, each containing 8 copies of one element
    BrcbRepeatParams brcbParams;
    brcbParams.dstBlkStride = 1;  // Blocks are consecutive
    brcbParams.dstRepStride = 8;  // 8 blocks stride between repeats
    
    // Broadcast 8 elements to tmpBuffer (8 blocks = 256 bytes)
    Brcb<T>(tmpBuffer, src, 1, brcbParams);
    
    PipeBarrier<PIPE_V>();
    
    // Step 2: Binary reduce within the 8 blocks
    // Each block has one unique value, need to find max across all 8
    // We can use binary reduction: compare first 4 blocks with last 4 blocks
    // Then compare first 2 blocks with next 2 blocks, etc.
    
    uint32_t halfElems = 4 * INT32_ELEM_PER_BLK;  // 32 elements (4 blocks)
    
    // Compare first 4 blocks with last 4 blocks
    VMaxImpl<T>(tmpBuffer, tmpBuffer, tmpBuffer[halfElems], halfElems);
    
    PipeBarrier<PIPE_V>();
    
    // Now we have 4 blocks of results, compare first 2 blocks with next 2 blocks
    halfElems = 2 * INT32_ELEM_PER_BLK;  // 16 elements
    VMaxImpl<T>(tmpBuffer, tmpBuffer, tmpBuffer[halfElems], halfElems);
    
    PipeBarrier<PIPE_V>();
    
    // Now we have 2 blocks, compare first block with second block
    halfElems = INT32_ELEM_PER_BLK;  // 8 elements
    VMaxImpl<T>(tmpBuffer, tmpBuffer, tmpBuffer[halfElems], halfElems);
    
    PipeBarrier<PIPE_V>();
    
    // Now first element of tmpBuffer contains the maximum value
    // Use SetValue/GetValue for single element extraction
    dst.SetValue(0, tmpBuffer.GetValue(0));
}

// ============================================================================
// ReduceMaxInt32AR - Reduce by last axis (AR pattern)
// ============================================================================
// Pattern AR: [M, N] -> [M, 1], reduce along the last axis (N)
// Each row is reduced to a single maximum value
template <bool isReuseSource = false>
__aicore__ inline void ReduceMaxInt32AR(const LocalTensor<int32_t>& dstTensor,
                                         const LocalTensor<int32_t>& srcTensor,
                                         const LocalTensor<int32_t>& tmpTensor,
                                         uint32_t firstAxis,    // M (rows)
                                         uint32_t lastAxis,     // N (columns)
                                         uint32_t padLast)      // Padded N
{
    constexpr uint32_t elePerBlk = INT32_ELEM_PER_BLK;  // 8 elements per block
    
    // Process each row independently
    for (uint32_t row = 0; row < firstAxis; row++) {
        LocalTensor<int32_t> rowSrc = srcTensor[row * padLast];
        LocalTensor<int32_t> rowDst = dstTensor[row];
        LocalTensor<int32_t> rowTmp = tmpTensor;
        
        // Reduce this row to a single value
        if (lastAxis <= elePerBlk) {
            // Small row: <= 8 elements, use direct binary reduction
            if (lastAxis == 1) {
                rowDst.SetValue(0, rowSrc.GetValue(0));
            } else {
                BinaryReduceToSingle<int32_t>(rowDst, rowSrc, rowTmp, lastAxis);
            }
        } else {
            // Large row: > 8 elements
            // Step 1: Reduce within each block first
            uint32_t blkCount = lastAxis / elePerBlk;
            uint32_t blkTail = lastAxis % elePerBlk;
            
            // First, copy first block to tmpTensor
            // Then compare with subsequent blocks
            LocalTensor<int32_t> accumBuffer = rowTmp;
            
            // Copy first block elements to accumBuffer
            for (uint32_t i = 0; i < elePerBlk; i++) {
                accumBuffer.SetValue(i, rowSrc.GetValue(i));
            }
            PipeBarrier<PIPE_V>();
            
            // Accumulate max from each subsequent block
            for (uint32_t blk = 1; blk < blkCount; blk++) {
                LocalTensor<int32_t> blkSrc = rowSrc[blk * elePerBlk];
                VMaxImpl<int32_t>(accumBuffer, accumBuffer, blkSrc, elePerBlk);
                PipeBarrier<PIPE_V>();
            }
            
            // Handle tail elements
            if (blkTail > 0) {
                // Compare accumBuffer[0:blkTail] with tail elements
                LocalTensor<int32_t> tailSrc = rowSrc[blkCount * elePerBlk];
                VMaxImpl<int32_t>(accumBuffer, accumBuffer, tailSrc, blkTail);
                PipeBarrier<PIPE_V>();
            }
            
            // Step 2: Reduce accumBuffer (8 elements) to single value
            BinaryReduceToSingle<int32_t>(rowDst, accumBuffer, rowTmp[elePerBlk], elePerBlk);
        }
        
        PipeBarrier<PIPE_V>();
    }
}

// ============================================================================
// ReduceMaxInt32RA - Reduce by first axis (RA pattern)
// ============================================================================
// Pattern RA: [M, N] -> [1, N], reduce along the first axis (M)
// Each column is reduced to a single maximum value
template <bool isReuseSource = false>
__aicore__ inline void ReduceMaxInt32RA(const LocalTensor<int32_t>& dstTensor,
                                         const LocalTensor<int32_t>& srcTensor,
                                         const LocalTensor<int32_t>& tmpTensor,
                                         uint32_t firstAxis,    // M (rows)
                                         uint32_t lastAxis,     // N (columns)
                                         uint32_t padLast)      // Padded N
{
    // Binary reduction along the first axis
    // Each iteration halves the number of rows
    
    uint32_t currentRows = firstAxis;
    uint32_t halfRows = currentRows / 2;
    uint32_t remainRows = currentRows % 2;
    
    // Use ping-pong buffers for intermediate results
    LocalTensor<int32_t> currentSrc = srcTensor;
    LocalTensor<int32_t> currentDst = tmpTensor;
    bool useTmpDst = true;
    
    // Handle odd number of rows: combine the last row with the result
    // First, we'll process pairs of rows
    
    while (currentRows > 1) {
        halfRows = currentRows / 2;
        
        // Compare each pair of rows: row[i] and row[i + halfRows]
        // Result stored in currentDst[0:halfRows * padLast]
        for (uint32_t row = 0; row < halfRows; row++) {
            LocalTensor<int32_t> src0 = currentSrc[row * padLast];
            LocalTensor<int32_t> src1 = currentSrc[(row + halfRows) * padLast];
            LocalTensor<int32_t> dstRow = currentDst[row * padLast];
            
            // Compare full row (use lastAxis for actual data, not padLast)
            VMaxImpl<int32_t>(dstRow, src0, src1, lastAxis);
        }
        
        // Handle odd row if exists
        if (currentRows % 2 == 1) {
            // Copy the last row to dstBuffer
            LocalTensor<int32_t> lastSrcRow = currentSrc[(currentRows - 1) * padLast];
            LocalTensor<int32_t> lastDstRow = currentDst[halfRows * padLast];
            
            // Copy last row using Adds with 0 as a copy operation
            Adds<int32_t, false>(lastDstRow, lastSrcRow, static_cast<int32_t>(0), lastAxis);
            halfRows++;  // Include the odd row in the count
        }
        
        PipeBarrier<PIPE_V>();
        
        // Update for next iteration
        currentRows = halfRows;
        
        // Swap buffers
        if (useTmpDst) {
            currentSrc = tmpTensor;
            currentDst = srcTensor;  // Reuse source as intermediate buffer
        } else {
            currentSrc = srcTensor;
            currentDst = tmpTensor;
        }
        useTmpDst = !useTmpDst;
    }
    
    // Final result is in currentSrc[0:padLast]
    // Copy to dstTensor using Adds with 0 as a copy operation
    if (!useTmpDst) {
        // Result is in tmpTensor, copy to dstTensor
        Adds<int32_t, false>(dstTensor, tmpTensor, static_cast<int32_t>(0), lastAxis);
    } else {
        // Result is in srcTensor (was used as intermediate buffer)
        Adds<int32_t, false>(dstTensor, srcTensor, static_cast<int32_t>(0), lastAxis);
    }
    
    PipeBarrier<PIPE_V>();
}

// ============================================================================
// ReduceMaxInt32Impl - Main entry point for int32_t ReduceMax
// ============================================================================
template <class pattern, bool isReuseSource = false>
__aicore__ inline void ReduceMaxInt32Impl(const LocalTensor<int32_t>& dstTensor,
                                           const LocalTensor<int32_t>& srcTensor,
                                           const LocalTensor<uint8_t>& sharedTmpBuffer,
                                           const uint32_t srcShape[],
                                           bool srcInnerPad)
{
    // Parse shape
    uint32_t firstAxis = srcShape[0];  // M
    uint32_t lastAxis = srcShape[1];   // N
    
    // Calculate padded last axis (32-byte aligned)
    constexpr uint32_t elePerBlk = INT32_ELEM_PER_BLK;
    uint32_t padLast = (lastAxis + elePerBlk - 1) / elePerBlk * elePerBlk;
    
    // Reinterpret tmpBuffer as int32_t
    LocalTensor<int32_t> tmpTensor = sharedTmpBuffer.ReinterpretCast<int32_t>();
    
    // Dispatch based on pattern
    if constexpr (IsSameType<pattern, Pattern::Reduce::AR>::value) {
        // AR: Reduce along last axis (rows -> single value per row)
        ReduceMaxInt32AR<isReuseSource>(dstTensor, srcTensor, tmpTensor, 
                                         firstAxis, lastAxis, padLast);
    } else {
        // RA: Reduce along first axis (columns -> single value per column)
        ReduceMaxInt32RA<isReuseSource>(dstTensor, srcTensor, tmpTensor,
                                         firstAxis, lastAxis, padLast);
    }
    
    // Reset mask to normal mode
    SetMaskNorm();
    ResetMask();
}

// ============================================================================
// GetReduceMaxInt32TmpSize - Calculate temporary buffer size for int32_t ReduceMax
// ============================================================================
// For int32_t, we need different tmp buffer calculation than vcgmax-based impl
// 
// AR mode (reduce along last axis [M, N] -> [M]):
//   - If N <= 8: Need space for binary reduction intermediate results (max 8 elements)
//   - If N > 8: Need first * 8 elements for block-level accumulation
//   - tmpSize = first * elePerBlk * sizeof(int32_t) = first * 32 bytes
//
// RA mode (reduce along first axis [M, N] -> [N]):
//   - Need k * padLast elements where k = FindK(first) / 2
//   - tmpSize = k * padLast * sizeof(int32_t)
inline void GetReduceMaxInt32TmpSize(const uint32_t srcShape[2], 
                                      bool srcInnerPad,
                                      bool isReuseSource,
                                      ReducePattern pattern,
                                      uint32_t& maxValue,
                                      uint32_t& minValue)
{
    const uint32_t first = srcShape[0];
    const uint32_t last = srcShape[1];
    constexpr uint32_t elePerBlk = ONE_BLK_SIZE / INT32_BYTES;  // 8 elements
    constexpr uint32_t elePerRep = ONE_REPEAT_BYTE_SIZE / INT32_BYTES;  // 64 elements
    
    if (isReuseSource) {
        maxValue = minValue = 0U;
        return;
    }
    
    if (pattern == ReducePattern::AR) {
        // AR mode: reduce along last axis [M, N] -> [M]
        // Need tmp buffer for each row's intermediate reduction
        if (last <= elePerBlk) {
            // Small row: <= 8 elements, need minimal tmp space for binary reduction
            // Max 8 elements for intermediate results
            maxValue = minValue = elePerBlk * INT32_BYTES;  // 32 bytes
        } else if (last <= elePerRep) {
            // Medium row: 9-64 elements
            // Need first * 8 elements for block accumulation
            maxValue = minValue = first * elePerBlk * INT32_BYTES;
        } else {
            // Large row: > 64 elements
            // Need first * 64 elements for block accumulation
            maxValue = minValue = first * elePerRep * INT32_BYTES;
        }
    } else {
        // RA mode: reduce along first axis [M, N] -> [N]
        // Need tmp buffer for ping-pong during binary reduction
        // Find the largest power of 2 <= first, then divide by 2
        uint32_t k = first;
        // Find largest power of 2 <= k
        if (k > 0) {
            k |= (k >> 1);
            k |= (k >> 2);
            k |= (k >> 4);
            k |= (k >> 8);
            k |= (k >> 16);
            k = (k + 1) >> 1;  // Largest power of 2 <= first
        }
        // For binary reduction, we need half of that
        if (k > 1) {
            k >>= 1;
        }
        
        // Calculate padded last axis
        uint32_t padLast = (last + elePerBlk - 1) / elePerBlk * elePerBlk;
        maxValue = minValue = k * padLast * INT32_BYTES;
    }
}

} // namespace Internal
} // namespace AscendC

#endif // IMPL_REDUCE_MAX_V3_REDUCE_MAX_INT32_IMPL_H_
