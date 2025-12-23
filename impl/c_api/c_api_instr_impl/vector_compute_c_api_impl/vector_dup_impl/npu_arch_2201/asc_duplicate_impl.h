/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/* !
 * \file duplicate_impl.h
 * \brief
 */
#ifndef IMPL_INSTR_VECTOR_COMPUTE_VECTOR_DUP_IMPL_NPU_ARCH_2201_ASC_DUPLICATE_IMPL_H
#define IMPL_INSTR_VECTOR_COMPUTE_VECTOR_DUP_IMPL_NPU_ARCH_2201_ASC_DUPLICATE_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

__aicore__ inline void asc_duplicate_impl(__ubuf__ half* dst, half src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vector_dup(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_duplicate_impl(__ubuf__ half* dst, half src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_duplicate_impl(dst, src, ASC_DEFAULT_REPEAT, ASC_DEFAULT_BLOCK_STRIDE, ASC_DEFAULT_BLOCK_STRIDE,
        ASC_DEFAULT_REPEAT_STRIDE, ASC_DEFAULT_REPEAT_STRIDE);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_duplicate_sync_impl(__ubuf__ half* dst, half src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_duplicate_impl(__ubuf__ int16_t* dst, int16_t src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vector_dup(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_duplicate_impl(__ubuf__ int16_t* dst, int16_t src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_duplicate_impl(dst, src, ASC_DEFAULT_REPEAT, ASC_DEFAULT_BLOCK_STRIDE, ASC_DEFAULT_BLOCK_STRIDE,
        ASC_DEFAULT_REPEAT_STRIDE, ASC_DEFAULT_REPEAT_STRIDE);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_duplicate_sync_impl(__ubuf__ int16_t* dst, int16_t src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_duplicate_impl(__ubuf__ uint16_t* dst, uint16_t src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vector_dup(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_duplicate_impl(__ubuf__ uint16_t* dst, uint16_t src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_duplicate_impl(dst, src, ASC_DEFAULT_REPEAT, ASC_DEFAULT_BLOCK_STRIDE, ASC_DEFAULT_BLOCK_STRIDE,
        ASC_DEFAULT_REPEAT_STRIDE, ASC_DEFAULT_REPEAT_STRIDE);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_duplicate_sync_impl(__ubuf__ uint16_t* dst, uint16_t src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_duplicate_impl(__ubuf__ bfloat16_t* dst, bfloat16_t src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vector_dup(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_duplicate_impl(__ubuf__ bfloat16_t* dst, bfloat16_t src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_duplicate_impl(dst, src, ASC_DEFAULT_REPEAT, ASC_DEFAULT_BLOCK_STRIDE, ASC_DEFAULT_BLOCK_STRIDE,
        ASC_DEFAULT_REPEAT_STRIDE, ASC_DEFAULT_REPEAT_STRIDE);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_duplicate_sync_impl(__ubuf__ bfloat16_t* dst, bfloat16_t src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_duplicate_impl(__ubuf__ float* dst, float src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vector_dup(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_duplicate_impl(__ubuf__ float* dst, float src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_duplicate_impl(dst, src, ASC_DEFAULT_REPEAT, ASC_DEFAULT_BLOCK_STRIDE, ASC_DEFAULT_BLOCK_STRIDE,
        ASC_DEFAULT_REPEAT_STRIDE, ASC_DEFAULT_REPEAT_STRIDE);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_duplicate_sync_impl(__ubuf__ float* dst, float src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_duplicate_impl(__ubuf__ int32_t* dst, int32_t src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vector_dup(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_duplicate_impl(__ubuf__ int32_t* dst, int32_t src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_duplicate_impl(dst, src, ASC_DEFAULT_REPEAT, ASC_DEFAULT_BLOCK_STRIDE, ASC_DEFAULT_BLOCK_STRIDE,
        ASC_DEFAULT_REPEAT_STRIDE, ASC_DEFAULT_REPEAT_STRIDE);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_duplicate_sync_impl(__ubuf__ int32_t* dst, int32_t src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_duplicate_impl(__ubuf__ uint32_t* dst, uint32_t src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vector_dup(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_duplicate_impl(__ubuf__ uint32_t* dst, uint32_t src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_duplicate_impl(dst, src, ASC_DEFAULT_REPEAT, ASC_DEFAULT_BLOCK_STRIDE, ASC_DEFAULT_BLOCK_STRIDE,
        ASC_DEFAULT_REPEAT_STRIDE, ASC_DEFAULT_REPEAT_STRIDE);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_duplicate_sync_impl(__ubuf__ uint32_t* dst, uint32_t src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
    asc_sync_post_process();
}
#endif