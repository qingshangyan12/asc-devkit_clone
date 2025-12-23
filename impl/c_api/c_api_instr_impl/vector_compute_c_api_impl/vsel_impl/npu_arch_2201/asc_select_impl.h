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
 * \file asc_select_impl.h
 * \brief
 */
#ifndef IMPL_INSTR_VECTOR_COMPUTE_VSEL_IMPL_NPU_ARCH_2201_ASC_SELECT_IMPL_H
#define IMPL_INSTR_VECTOR_COMPUTE_VSEL_IMPL_NPU_ARCH_2201_ASC_SELECT_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

// asc_select half
__aicore__ inline void asc_select_impl(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vsel(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
             dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_select_impl(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_select_impl(dst, src0, src1, ASC_DEFAULT_REPEAT, ASC_DEFAULT_BLOCK_STRIDE, ASC_DEFAULT_BLOCK_STRIDE,
        ASC_DEFAULT_BLOCK_STRIDE, ASC_DEFAULT_REPEAT_STRIDE, ASC_DEFAULT_REPEAT_STRIDE, ASC_DEFAULT_REPEAT_STRIDE);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_select_sync_impl(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{  
    asc_select_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

// asc_select float
__aicore__ inline void asc_select_impl(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vsel(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
             dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_select_impl(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_select_impl(dst, src0, src1, ASC_DEFAULT_REPEAT, ASC_DEFAULT_BLOCK_STRIDE, ASC_DEFAULT_BLOCK_STRIDE,
        ASC_DEFAULT_BLOCK_STRIDE, ASC_DEFAULT_REPEAT_STRIDE, ASC_DEFAULT_REPEAT_STRIDE, ASC_DEFAULT_REPEAT_STRIDE);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_select_sync_impl(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{  
    asc_select_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

#endif