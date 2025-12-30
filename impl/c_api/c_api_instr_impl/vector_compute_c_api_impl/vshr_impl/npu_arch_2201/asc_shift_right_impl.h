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
 * \file asc_shift_right_impl.h
 * \brief
 */
#ifndef IMPL_C_API_C_API_INSTR_IMPL_VECTOR_COMPUTE_C_API_IMPL_VSHR_IMPL_NPU_ARCH_2201_ASC_SHIFT_RIGHT_IMPL_H
#define IMPL_C_API_C_API_INSTR_IMPL_VECTOR_COMPUTE_C_API_IMPL_VSHR_IMPL_NPU_ARCH_2201_ASC_SHIFT_RIGHT_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

// asc_shift_right int16_t
__aicore__ inline void asc_shift_right_impl(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vshr(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride, false);
    }
}

__aicore__ inline void asc_shift_right_impl(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_shift_right_impl(dst, src, value, ASC_DEFAULT_REPEAT, ASC_DEFAULT_BLOCK_STRIDE, ASC_DEFAULT_BLOCK_STRIDE,
        ASC_DEFAULT_REPEAT_STRIDE, ASC_DEFAULT_REPEAT_STRIDE);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_shift_right_sync_impl(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint32_t count)
{
    asc_shift_right_impl(dst, src, value, count);
    asc_sync_post_process();
}

// asc_shift_right int32_t
__aicore__ inline void asc_shift_right_impl(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vshr(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride, false);
    }
}

__aicore__ inline void asc_shift_right_impl(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_shift_right_impl(dst, src, value, ASC_DEFAULT_REPEAT, ASC_DEFAULT_BLOCK_STRIDE, ASC_DEFAULT_BLOCK_STRIDE,
        ASC_DEFAULT_REPEAT_STRIDE, ASC_DEFAULT_REPEAT_STRIDE);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_shift_right_sync_impl(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_shift_right_impl(dst, src, value, count);
    asc_sync_post_process();
}

// asc_shift_right uint16_t
__aicore__ inline void asc_shift_right_impl(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vshr(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride, false);
    }
}

__aicore__ inline void asc_shift_right_impl(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_shift_right_impl(dst, src, value, ASC_DEFAULT_REPEAT, ASC_DEFAULT_BLOCK_STRIDE, ASC_DEFAULT_BLOCK_STRIDE,
        ASC_DEFAULT_REPEAT_STRIDE, ASC_DEFAULT_REPEAT_STRIDE);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_shift_right_sync_impl(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t value, uint32_t count)
{
    asc_shift_right_impl(dst, src, value, count);
    asc_sync_post_process();
}

// asc_shift_right uint32_t
__aicore__ inline void asc_shift_right_impl(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vshr(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride, false);
    }
}

__aicore__ inline void asc_shift_right_impl(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_shift_right_impl(dst, src, value, ASC_DEFAULT_REPEAT, ASC_DEFAULT_BLOCK_STRIDE, ASC_DEFAULT_BLOCK_STRIDE,
        ASC_DEFAULT_REPEAT_STRIDE, ASC_DEFAULT_REPEAT_STRIDE);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_shift_right_sync_impl(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t value, uint32_t count)
{
    asc_shift_right_impl(dst, src, value, count);
    asc_sync_post_process();
}

#endif