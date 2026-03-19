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
// VMaxImpl - Core function for vmax instruction (pure wrapper)
// ============================================================================
// Performs: dst = vmax(src0, src1) element-wise
// NOTE: This function does NOT set mask internally. Caller is responsible for:
//   1. Setting mask mode: SetMaskCount()
//   2. Setting vector mask: SetVectorMask<T, MaskMode::COUNTER>(0, count)
//   3. Using MASK_PLACEHOLDER as mask parameter
//   4. Calling PipeBarrier<PIPE_V>() after this function
template <typename T>
__aicore__ inline void VMaxImpl(const LocalTensor<T>& dst,
                                 const LocalTensor<T>& src0,
                                 const LocalTensor<T>& src1,
                                 uint64_t mask,
                                 const uint8_t repeatTime,
                                 const BinaryRepeatParams& repeatParams)
{
    // Pure vmax instruction call - no mask management
    vmax((__ubuf__ T*)dst.GetPhyAddr(),
         (__ubuf__ T*)src0.GetPhyAddr(),
         (__ubuf__ T*)src1.GetPhyAddr(),
         repeatTime,
         repeatParams.dstBlkStride,
         repeatParams.src0BlkStride,
         repeatParams.src1BlkStride,
         repeatParams.dstRepStride,
         repeatParams.src0RepStride,
         repeatParams.src1RepStride);
}

// ============================================================================
// BinaryReduceMax - Core function for binary reduction using vmax
// ============================================================================
// Performs: dst = vmax(src0, src1) element-wise
// This is used as the building block for binary reduction
// NOTE: Caller must set mask before calling this function
template <typename T>
__aicore__ inline void BinaryReduceMax(const LocalTensor<T>& dst,
                                        const LocalTensor<T>& src0,
                                        const LocalTensor<T>& src1,
                                        uint64_t mask,
                                        const uint8_t repeatTime,
                                        const BinaryRepeatParams& repeatParams)
{
    VMaxImpl<T>(dst, src0, src1, mask, repeatTime, repeatParams);
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
// NOTE: Caller must set mask before calling this function
template <typename T>
__aicore__ inline void BinaryReduceHalf(const LocalTensor<T>& dst,
                                         const LocalTensor<T>& src,
                                         uint32_t count,
                                         uint64_t mask,
                                         const uint8_t repeatTime,
                                         const BinaryRepeatParams& repeatParams)
{
    // Validate: count must be power of 2 and at least 2
    // Compare first half with second half
    uint32_t halfCount = count / 2;
    
    // src0 points to first half, src1 points to second half
    // dst[i] = max(src[i], src[i + halfCount])
    LocalTensor<T> src0 = src;
    LocalTensor<T> src1 = src[halfCount];
    
    BinaryReduceMax<T>(dst, src0, src1, mask, repeatTime, repeatParams);
}

// ============================================================================
// BrcbReduceToSingle - Reduce elements to single value using Brcb + vmax
// ============================================================================
// Core idea: Use Brcb to expand each element to a datablock (32 bytes),
//           then use vmax for block-wise reduction.
//
// Why this works:
// 1. vmax minimum granularity is one datablock (32 bytes = 8 int32_t elements)
// 2. When count < 8, vmax cannot work directly
// 3. Brcb expands each element to a full datablock, enabling vmax operations
//
// Advantages: Handles ANY count (including < 8 and non-power-of-2)
// Memory requirement: tmpBuffer >= 288 bytes (32 bytes prep + 256 bytes Brcb output)
//
// Process for count=6:
//   Input: [a, b, c, d, e, f]
//   Step 1: Pad to 8 elements: [a, b, c, d, e, f, MIN, MIN]
//   Step 2: Brcb expand to 8 datablocks:
//     Block 0: [a,a,a,a,a,a,a,a], Block 1: [b,b,b,b,b,b,b,b], ...
//   Step 3: vmax block-wise reduction to find max
//   Result: max(a, b, c, d, e, f)
template <typename T>
__aicore__ inline void BrcbReduceToSingle(const LocalTensor<T>& dst,
                                           const LocalTensor<T>& src,
                                           const LocalTensor<T>& tmpBuffer,
                                           uint32_t count)
{
    // Use repStride = 1 for small data to avoid NPU address check error
    // NPU hardware checks repStride address range even when repeatTime = 1
    BinaryRepeatParams smallDataParam;
    smallDataParam.dstBlkStride = 1;
    smallDataParam.src0BlkStride = 1;
    smallDataParam.src1BlkStride = 1;
    smallDataParam.dstRepStride = 1;   // Changed from DEFAULT_REPEAT_STRIDE (8)
    smallDataParam.src0RepStride = 1;
    smallDataParam.src1RepStride = 1;
    
    UnaryRepeatParams defaultUnaryParam;
    
    // Handle edge cases
    if (count == 0) {
        return;  // Empty input
    }
    
    if (count == 1) {
        // Single element: use Adds with mask to handle non-32B-aligned dst
        SetMaskCount();
        SetVectorMask<T, MaskMode::COUNTER>(0, 1);
        // Add 0 is equivalent to copy, works with any alignment
        Adds<T, false>(dst, src, static_cast<T>(0), MASK_PLACEHOLDER, 1, defaultUnaryParam);
        PipeBarrier<PIPE_V>();
        return;
    }
    
    // Step 1: Prepare 8 elements (pad with MIN_VAL if count < 8)
    // tmpBuffer layout: [0:8] = prepare buffer, [8:72] = Brcb output
    LocalTensor<T> prepareBuffer = tmpBuffer;  // First 32 bytes (8 elements)
    
    SetMaskCount();
    
    if (count < 8) {
        // Fill entire 8-element region with MIN_VAL first
        // Note: SetValue may not be available, use Duplicate to fill
        // For int32_t, MIN_VAL = -2147483648
        // We use a workaround: fill by copying from a pre-filled location

        // Alternative approach: Fill prepare buffer with MIN_VAL using Duplicate
        T minVal = static_cast<T>(-2147483647 - 1);  // INT32_MIN for int32_t, safe for other types

        // Use Duplicate to fill 8 elements with MIN_VAL
        SetVectorMask<T, MaskMode::COUNTER>(0, 8);
        Duplicate<T>(prepareBuffer, minVal, 8);
        PipeBarrier<PIPE_V>();

        // CRITICAL: Duplicate() internally calls SetMaskNorm(), must re-enable Count mode
        SetMaskCount();

        // Copy valid data (overwrites the MIN_VAL padding)
        SetVectorMask<T, MaskMode::COUNTER>(0, count);
        Adds<T, false>(prepareBuffer, src, static_cast<T>(0), MASK_PLACEHOLDER, 1, defaultUnaryParam);
        PipeBarrier<PIPE_V>();
    } else {
        // count >= 8: copy first 8 elements directly
        SetVectorMask<T, MaskMode::COUNTER>(0, 8);
        Adds<T, false>(prepareBuffer, src, static_cast<T>(0), MASK_PLACEHOLDER, 1, defaultUnaryParam);
        PipeBarrier<PIPE_V>();
    }
    
    // Step 2: Use Brcb to expand 8 elements to 8 datablocks
    // For int32_t: each element -> 8 copies in one datablock (32 bytes)
    // Output: 8 datablocks = 64 elements = 256 bytes
    LocalTensor<T> brcbDst = tmpBuffer[8];  // Starting at element 8 (offset 32 bytes)
    
    BrcbRepeatParams brcbParams;
    brcbParams.dstBlkStride = 1;   // Consecutive datablocks
    brcbParams.dstRepStride = 8;   // 8 datablocks between repeats
    
    Brcb<T>(brcbDst, prepareBuffer, 1, brcbParams);
    PipeBarrier<PIPE_V>();
    
    // Step 3: VMax block-wise reduction to find maximum
    // Each datablock contains 8 identical values
    // We need to find max across all 8 datablocks
    // CRITICAL: Use smallDataParam with repStride=1 to avoid NPU address check error
    
    SetMaskCount();
    
    // Round 1: Compare Block 0-3 with Block 4-7 -> Result in Block 0-3
    uint32_t halfElems = 4 * INT32_ELEM_PER_BLK;  // 32 elements (4 datablocks)
    SetVectorMask<T, MaskMode::COUNTER>(0, halfElems);
    VMaxImpl<T>(brcbDst, brcbDst, brcbDst[halfElems], MASK_PLACEHOLDER, 1, smallDataParam);
    PipeBarrier<PIPE_V>();
    
    // Round 2: Compare Block 0-1 with Block 2-3 -> Result in Block 0-1
    halfElems = 2 * INT32_ELEM_PER_BLK;  // 16 elements
    SetVectorMask<T, MaskMode::COUNTER>(0, halfElems);
    VMaxImpl<T>(brcbDst, brcbDst, brcbDst[halfElems], MASK_PLACEHOLDER, 1, smallDataParam);
    PipeBarrier<PIPE_V>();
    
    // Round 3: Compare Block 0 with Block 1 -> Result in Block 0
    halfElems = INT32_ELEM_PER_BLK;  // 8 elements
    SetVectorMask<T, MaskMode::COUNTER>(0, halfElems);
    VMaxImpl<T>(brcbDst, brcbDst, brcbDst[halfElems], MASK_PLACEHOLDER, 1, smallDataParam);
    PipeBarrier<PIPE_V>();
    
    // Step 4: Write result to dst using Adds (dst is guaranteed 32B aligned by caller)
    // Result is in brcbDst[0], copy to dst[0]
    SetMaskCount();
    SetVectorMask<T, MaskMode::COUNTER>(0, 1);
    Adds<T, false>(dst, brcbDst, static_cast<T>(0), MASK_PLACEHOLDER, 1, defaultUnaryParam);
    PipeBarrier<PIPE_V>();
}

// ============================================================================
// BinaryReduceToSingle - Repeatedly apply binary reduction until single value
// ============================================================================
// Input:  src with `count` elements (any count, including non-power-of-2)
// Output: dst[0] contains the maximum value
// Uses tmpBuffer for intermediate results
// IMPORTANT: This function does NOT use recursion (Ascend C does not support it)
// Reference: BinaryReduceByFirstAxis in reduce_common_util_v220_impl.h
//
// Modification: For count <= 8, uses BrcbReduceToSingle for correctness
// (handles non-power-of-2 counts correctly)
template <typename T>
__aicore__ inline void BinaryReduceToSingle(const LocalTensor<T>& dst,
                                             const LocalTensor<T>& src,
                                             const LocalTensor<T>& tmpBuffer,
                                             uint32_t count)
{
    BinaryRepeatParams defaultParam;  // Default stride parameters
    UnaryRepeatParams defaultUnaryParam;
    
    if (count <= 1) {
        // Already single element or empty
        if (count == 1) {
            // Use Adds instead of SetValue for mask consistency
            SetMaskCount();
            SetVectorMask<T, MaskMode::COUNTER>(0, 1);
            Adds<T, false>(dst, src, static_cast<T>(0), MASK_PLACEHOLDER, 1, defaultUnaryParam);
            PipeBarrier<PIPE_V>();
        }
        return;
    }
    
    // ========== Use BrcbReduceToSingle for count <= 8 ==========
    // This handles non-power-of-2 counts correctly (e.g., 3, 5, 6, 7)
    if (count <= 8) {
        BrcbReduceToSingle<T>(dst, src, tmpBuffer, count);
        return;
    }
    // ============================================================
    
    // Set mask to count mode for all operations
    SetMaskCount();
    
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
        
        // Set mask for this iteration
        SetVectorMask<T, MaskMode::COUNTER>(0, halfCount);
        
        // Perform binary reduction: compare first half with second half
        LocalTensor<T> src0 = currentSrc;
        LocalTensor<T> src1 = currentSrc[halfCount];
        
        VMaxImpl<T>(currentDst, src0, src1, MASK_PLACEHOLDER, 1, defaultParam);
        
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
    // Use BrcbReduceToSingle for correctness (handles any count)
    LocalTensor<T> finalSrc = useTmpDst ? tmpBuffer : dst;
    LocalTensor<T> finalTmp = useTmpDst ? dst : tmpBuffer;
    
    BrcbReduceToSingle<T>(dst, finalSrc, finalTmp, currentCount);
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
    BinaryRepeatParams defaultParam;
    UnaryRepeatParams defaultUnaryParam;
    
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
    
    SetMaskCount();
    
    uint32_t halfElems = 4 * INT32_ELEM_PER_BLK;  // 32 elements (4 blocks)
    
    // Compare first 4 blocks with last 4 blocks
    SetVectorMask<T, MaskMode::COUNTER>(0, halfElems);
    VMaxImpl<T>(tmpBuffer, tmpBuffer, tmpBuffer[halfElems], MASK_PLACEHOLDER, 1, defaultParam);
    
    PipeBarrier<PIPE_V>();
    
    // Now we have 4 blocks of results, compare first 2 blocks with next 2 blocks
    halfElems = 2 * INT32_ELEM_PER_BLK;  // 16 elements
    SetVectorMask<T, MaskMode::COUNTER>(0, halfElems);
    VMaxImpl<T>(tmpBuffer, tmpBuffer, tmpBuffer[halfElems], MASK_PLACEHOLDER, 1, defaultParam);
    
    PipeBarrier<PIPE_V>();
    
    // Now we have 2 blocks, compare first block with second block
    halfElems = INT32_ELEM_PER_BLK;  // 8 elements
    SetVectorMask<T, MaskMode::COUNTER>(0, halfElems);
    VMaxImpl<T>(tmpBuffer, tmpBuffer, tmpBuffer[halfElems], MASK_PLACEHOLDER, 1, defaultParam);
    
    PipeBarrier<PIPE_V>();
    
    // Now first element of tmpBuffer contains the maximum value
    // Use Adds for single element extraction
    SetVectorMask<T, MaskMode::COUNTER>(0, 1);
    Adds<T, false>(dst, tmpBuffer, static_cast<T>(0), MASK_PLACEHOLDER, 1, defaultUnaryParam);
    PipeBarrier<PIPE_V>();
}

// ============================================================================
// ReduceMaxInt32AR - Reduce by last axis (AR pattern)
// ============================================================================
// Pattern AR: [M, N] -> [M, 1], reduce along the last axis (N)
// Each row is reduced to a single maximum value
// Reference: reduce_prod_v220_impl.h ReduceProdByLastAxis
template <bool isReuseSource = false>
__aicore__ inline void ReduceMaxInt32AR(const LocalTensor<int32_t>& dstTensor,
                                         const LocalTensor<int32_t>& srcTensor,
                                         const LocalTensor<int32_t>& tmpTensor,
                                         uint32_t firstAxis,    // M (rows)
                                         uint32_t lastAxis,     // N (columns)
                                         uint32_t padLast)      // Padded N
{
    constexpr uint32_t elePerBlk = INT32_ELEM_PER_BLK;  // 8 elements per block
    BinaryRepeatParams defaultParam;
    UnaryRepeatParams defaultUnaryParam;
    
    // Allocate buffers based on isReuseSource
    // Reference: reduce_prod_v220_impl.h line 83
    LocalTensor<int32_t> rowResults = tmpTensor;
    LocalTensor<int32_t> workBuffer = isReuseSource ? tmpTensor : tmpTensor[firstAxis * elePerBlk];
    
    // finalResStored: source tensor for GatherMask extraction
    // isReuseSource = true: results stored in srcTensor (spaced by padLast)
    // isReuseSource = false: results stored in rowResults (compact)
    LocalTensor<int32_t> finalResStored = isReuseSource ? srcTensor : rowResults;
    
    // countstride for GatherMask: controls source address stride
    // isReuseSource = true: results at src[row * padLast], stride = padLast / elePerBlk
    // isReuseSource = false: results at rowResults[row * elePerBlk], stride = 1
    uint16_t countStride = isReuseSource ? static_cast<uint16_t>(padLast / elePerBlk) : 1;
    
    // Process each row independently
    for (uint32_t row = 0; row < firstAxis; row++) {
        LocalTensor<int32_t> rowSrc = srcTensor[row * padLast];
        // rowDst: destination for this row's result
        // isReuseSource = true: store in srcTensor[row * padLast] (reuse source space)
        // isReuseSource = false: store in rowResults[row * elePerBlk] (32B aligned)
        LocalTensor<int32_t> rowDst = isReuseSource ? rowSrc : rowResults[row * elePerBlk];
        
        // Reduce this row to a single value
        if (lastAxis <= elePerBlk) {
            // Small row: <= 8 elements, use direct binary reduction
            if (lastAxis == 1) {
                // Use Adds for single element
                SetMaskCount();
                SetVectorMask<int32_t, MaskMode::COUNTER>(0, 1);
                Adds<int32_t, false>(rowDst, rowSrc, static_cast<int32_t>(0), 
                                     MASK_PLACEHOLDER, 1, defaultUnaryParam);
                PipeBarrier<PIPE_V>();
            } else {
                BinaryReduceToSingle<int32_t>(rowDst, rowSrc, workBuffer, lastAxis);
            }
        } else {
            // Large row: > 8 elements
            // Step 1: Reduce within each block first
            uint32_t blkCount = lastAxis / elePerBlk;
            uint32_t blkTail = lastAxis % elePerBlk;
            
            // First, copy first block to workBuffer using Adds
            LocalTensor<int32_t> accumBuffer = workBuffer;
            
            SetMaskCount();
            SetVectorMask<int32_t, MaskMode::COUNTER>(0, elePerBlk);
            Adds<int32_t, false>(accumBuffer, rowSrc, static_cast<int32_t>(0), 
                                 MASK_PLACEHOLDER, 1, defaultUnaryParam);
            PipeBarrier<PIPE_V>();
            
            // Accumulate max from each subsequent block
            for (uint32_t blk = 1; blk < blkCount; blk++) {
                LocalTensor<int32_t> blkSrc = rowSrc[blk * elePerBlk];
                SetVectorMask<int32_t, MaskMode::COUNTER>(0, elePerBlk);
                VMaxImpl<int32_t>(accumBuffer, accumBuffer, blkSrc, 
                                  MASK_PLACEHOLDER, 1, defaultParam);
                PipeBarrier<PIPE_V>();
            }
            
            // Handle tail elements
            if (blkTail > 0) {
                // Compare accumBuffer[0:blkTail] with tail elements
                LocalTensor<int32_t> tailSrc = rowSrc[blkCount * elePerBlk];
                SetVectorMask<int32_t, MaskMode::COUNTER>(0, blkTail);
                VMaxImpl<int32_t>(accumBuffer, accumBuffer, tailSrc, 
                                  MASK_PLACEHOLDER, 1, defaultParam);
                PipeBarrier<PIPE_V>();
            }
            
            // Step 2: Reduce accumBuffer (8 elements) to single value
            BinaryReduceToSingle<int32_t>(rowDst, accumBuffer, workBuffer[elePerBlk], elePerBlk);
        }
        
        PipeBarrier<PIPE_V>();
    }
    
    // Finally, extract all row results to dstTensor using GatherMask
    // Reference: reduce_prod_v220_impl.h lines 126-131
    LocalTensor<uint32_t> patternTensor = workBuffer.template ReinterpretCast<uint32_t>();
    SetMaskCount();
    SetVectorMask<uint32_t, MaskMode::COUNTER>(0, elePerBlk);
    Duplicate<uint32_t>(patternTensor, 1u, elePerBlk);
    PipeBarrier<PIPE_V>();
    
    GatherMaskParams gatherMaskParam = {1, static_cast<uint16_t>(firstAxis), countStride, 0};
    uint64_t rsvdCnt;
    GatherMask<int32_t, uint32_t>(dstTensor, finalResStored, patternTensor, 
                                   true, elePerBlk, gatherMaskParam, rsvdCnt);
}

// ============================================================================
// ReduceMaxInt32RA - Reduce by first axis (RA pattern)
// ============================================================================
// Pattern RA: [M, N] -> [1, N], reduce along the first axis (M)
// Each column is reduced to a single maximum value
// Reference: BinaryReduceByFirstAxis in reduce_common_util_v220_impl.h
template <bool isReuseSource = false>
__aicore__ inline void ReduceMaxInt32RA(const LocalTensor<int32_t>& dstTensor,
                                         const LocalTensor<int32_t>& srcTensor,
                                         const LocalTensor<int32_t>& tmpTensor,
                                         uint32_t firstAxis,    // M (rows)
                                         uint32_t lastAxis,     // N (columns)
                                         uint32_t padLast)      // Padded N
{
    BinaryRepeatParams defaultParam;
    UnaryRepeatParams defaultUnaryParam;
    
    // Reference: BinaryReduceByFirstAxis line 475
    // isReuseSource = true: work directly on srcTensor
    // isReuseSource = false: use tmpTensor as work buffer
    LocalTensor<int32_t> currBuff = isReuseSource ? srcTensor : tmpTensor;
    
    // Find closest power of 2 for binary reduction
    uint32_t k = FindClosestPowerOfTwo(firstAxis);
    uint32_t splitK = 1 << k;
    uint32_t remain = firstAxis - splitK;
    
    SetMaskCount();
    
    if constexpr (isReuseSource) {
        // isReuseSource = true: reduce directly on srcTensor
        // Handle remaining rows (non-power-of-2 case)
        if (remain != 0) {
            // Combine remaining rows with the main block
            SetVectorMask<int32_t, MaskMode::COUNTER>(0, padLast * remain);
            VMaxImpl<int32_t>(srcTensor, srcTensor, srcTensor[splitK * padLast], 
                              MASK_PLACEHOLDER, 1, defaultParam);
            PipeBarrier<PIPE_V>();
        }
    } else {
        // isReuseSource = false: use tmpTensor as work buffer
        // Handle remaining rows (non-power-of-2 case)
        if (remain != 0) {
            // First copy main block to tmpTensor
            SetVectorMask<int32_t, MaskMode::COUNTER>(0, splitK * padLast);
            Adds<int32_t, false>(tmpTensor, srcTensor, static_cast<int32_t>(0), 
                                 MASK_PLACEHOLDER, 1, defaultUnaryParam);
            PipeBarrier<PIPE_V>();
            // Combine remaining rows
            SetVectorMask<int32_t, MaskMode::COUNTER>(0, padLast * remain);
            VMaxImpl<int32_t>(tmpTensor, tmpTensor, srcTensor[splitK * padLast], 
                              MASK_PLACEHOLDER, 1, defaultParam);
            PipeBarrier<PIPE_V>();
        } else if (splitK > 1) {
            // Binary reduce from srcTensor to tmpTensor
            splitK >>= 1;
            SetVectorMask<int32_t, MaskMode::COUNTER>(0, padLast * splitK);
            VMaxImpl<int32_t>(tmpTensor, srcTensor, srcTensor[splitK * padLast], 
                              MASK_PLACEHOLDER, 1, defaultParam);
            PipeBarrier<PIPE_V>();
        } else {
            // Single row, copy directly to dstTensor
            SetVectorMask<int32_t, MaskMode::COUNTER>(0, lastAxis);
            Adds<int32_t, false>(dstTensor, srcTensor, static_cast<int32_t>(0), 
                                 MASK_PLACEHOLDER, 1, defaultUnaryParam);
            PipeBarrier<PIPE_V>();
            return;
        }
    }
    
    // Binary reduce the remaining 2^k rows
    while (splitK > 1) {
        splitK >>= 1;
        SetVectorMask<int32_t, MaskMode::COUNTER>(0, padLast * splitK);
        VMaxImpl<int32_t>(currBuff, currBuff, currBuff[splitK * padLast], 
                          MASK_PLACEHOLDER, 1, defaultParam);
        PipeBarrier<PIPE_V>();
    }
    
    // Copy final result to dstTensor
    SetVectorMask<int32_t, MaskMode::COUNTER>(0, lastAxis);
    Adds<int32_t, false>(dstTensor, currBuff, static_cast<int32_t>(0), 
                         MASK_PLACEHOLDER, 1, defaultUnaryParam);
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
    // Brcb reduction requires: 32 bytes prep buffer + 256 bytes Brcb output
    constexpr uint32_t BRCB_TMP_SIZE = 288;
    
    if (isReuseSource) {
        // isReuseSource = true: reuse srcTensor for intermediate results
        // AR mode: still needs Brcb work buffer (288 bytes)
        // RA mode: no tmp buffer needed (binary reduction on srcTensor directly)
        if (pattern == ReducePattern::AR) {
            maxValue = minValue = BRCB_TMP_SIZE;  // 288 bytes for Brcb work buffer
        } else {
            maxValue = minValue = 0U;  // RA mode: no tmp buffer needed
        }
        return;
    }
    
    if (pattern == ReducePattern::AR) {
        // AR mode: reduce along last axis [M, N] -> [M]
        // Buffer layout: rowResults (first * elePerBlk) + workBuffer (288 bytes for Brcb)
        // - rowResults: aligned buffer for storing each row's result (first * 8 elements)
        // - workBuffer: Brcb reduction requires 32 bytes prep + 256 bytes Brcb output = 288 bytes
        uint32_t rowResultsSize = first * elePerBlk * INT32_BYTES;  // first * 32 bytes
        maxValue = minValue = rowResultsSize + BRCB_TMP_SIZE;  // 288 bytes for Brcb
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
        // Add 256 bytes for final Brcb reduction
        maxValue = minValue = k * padLast * INT32_BYTES + 256;
    }
}

} // namespace Internal
} // namespace AscendC

#endif // IMPL_REDUCE_MAX_V3_REDUCE_MAX_INT32_IMPL_H_
