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
 * \file asc_half2int4_impl.h
 * \brief
 */
#ifndef IMPL_INSTR_VECTOR_COMPUTE_VCONV_HALF2INT4_IMPL_NPU_ARCH_2201_ASC_HALF2INT4_IMPL_H
#define IMPL_INSTR_VECTOR_COMPUTE_VCONV_HALF2INT4_IMPL_NPU_ARCH_2201_ASC_HALF2INT4_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

// asc_half2int4
__aicore__ inline void asc_half2int4_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s4((__ubuf__ void*)dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int4_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int4_impl(dst, src, ASC_DEFAULT_REPEAT, static_cast<uint16_t>(ASC_DEFAULT_BLOCK_STRIDE),
                        static_cast<uint16_t>(ASC_DEFAULT_BLOCK_STRIDE), static_cast<uint16_t>(ASC_ONE_FOURTH_DEFAULT_REPEAT_STRIDE),
                        static_cast<uint16_t>(ASC_DEFAULT_REPEAT_STRIDE));
    asc_set_mask_count_end();
}

__aicore__ inline void asc_half2int4_sync_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_impl(dst, src, count);
    asc_sync_post_process();
}

// asc_half2int4_a
__aicore__ inline void asc_half2int4_a_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s4a((__ubuf__ void*)dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int4_a_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int4_a_impl(dst, src, ASC_DEFAULT_REPEAT, static_cast<uint16_t>(ASC_DEFAULT_BLOCK_STRIDE),
                        static_cast<uint16_t>(ASC_DEFAULT_BLOCK_STRIDE), static_cast<uint16_t>(ASC_ONE_FOURTH_DEFAULT_REPEAT_STRIDE),
                        static_cast<uint16_t>(ASC_DEFAULT_REPEAT_STRIDE));
    asc_set_mask_count_end();
}

__aicore__ inline void asc_half2int4_a_sync_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_a_impl(dst, src, count);
    asc_sync_post_process();
}

// asc_half2int4_c
__aicore__ inline void asc_half2int4_c_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s4c((__ubuf__ void*)dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int4_c_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int4_c_impl(dst, src, ASC_DEFAULT_REPEAT, static_cast<uint16_t>(ASC_DEFAULT_BLOCK_STRIDE),
                        static_cast<uint16_t>(ASC_DEFAULT_BLOCK_STRIDE), static_cast<uint16_t>(ASC_ONE_FOURTH_DEFAULT_REPEAT_STRIDE),
                        static_cast<uint16_t>(ASC_DEFAULT_REPEAT_STRIDE));
    asc_set_mask_count_end();
}

__aicore__ inline void asc_half2int4_c_sync_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_c_impl(dst, src, count);
    asc_sync_post_process();
}

// asc_half2int4_f
__aicore__ inline void asc_half2int4_f_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s4f((__ubuf__ void*)dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int4_f_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int4_f_impl(dst, src, ASC_DEFAULT_REPEAT, static_cast<uint16_t>(ASC_DEFAULT_BLOCK_STRIDE),
                        static_cast<uint16_t>(ASC_DEFAULT_BLOCK_STRIDE), static_cast<uint16_t>(ASC_ONE_FOURTH_DEFAULT_REPEAT_STRIDE),
                        static_cast<uint16_t>(ASC_DEFAULT_REPEAT_STRIDE));
    asc_set_mask_count_end();
}

__aicore__ inline void asc_half2int4_f_sync_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_f_impl(dst, src, count);
    asc_sync_post_process();
}

// asc_half2int4_r
__aicore__ inline void asc_half2int4_r_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s4r((__ubuf__ void*)dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int4_r_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int4_r_impl(dst, src, ASC_DEFAULT_REPEAT, static_cast<uint16_t>(ASC_DEFAULT_BLOCK_STRIDE),
                        static_cast<uint16_t>(ASC_DEFAULT_BLOCK_STRIDE), static_cast<uint16_t>(ASC_ONE_FOURTH_DEFAULT_REPEAT_STRIDE),
                        static_cast<uint16_t>(ASC_DEFAULT_REPEAT_STRIDE));
    asc_set_mask_count_end();
}

__aicore__ inline void asc_half2int4_r_sync_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_r_impl(dst, src, count);
    asc_sync_post_process();
}

// asc_half2int4_z
__aicore__ inline void asc_half2int4_z_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s4z((__ubuf__ void*)dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int4_z_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int4_z_impl(dst, src, ASC_DEFAULT_REPEAT, static_cast<uint16_t>(ASC_DEFAULT_BLOCK_STRIDE),
                        static_cast<uint16_t>(ASC_DEFAULT_BLOCK_STRIDE), static_cast<uint16_t>(ASC_ONE_FOURTH_DEFAULT_REPEAT_STRIDE),
                        static_cast<uint16_t>(ASC_DEFAULT_REPEAT_STRIDE));
    asc_set_mask_count_end();
}

__aicore__ inline void asc_half2int4_z_sync_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_z_impl(dst, src, count);
    asc_sync_post_process();
}

#endif