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
 * \file mul_cast_half2uint8_impl.h
 * \brief
 */
#ifndef C_API_INSTR_IMPL_VECTOR_COMPUTE_C_API_IMPL_VMULCONV_C_API_IMPL_MUL_CAST_HALF2UINT8_IMPL_H
#define C_API_INSTR_IMPL_VECTOR_COMPUTE_C_API_IMPL_VMULCONV_C_API_IMPL_MUL_CAST_HALF2UINT8_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

__aicore__ inline void asc_mul_cast_half2uint8_impl(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1,
    uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vmulconv_f162u8(dst, src0, src1, repeat, 
            dst_block_stride, src0_block_stride, src1_block_stride,
            dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_mul_cast_half2uint8_impl(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    constexpr uint8_t dst_repeat_stride = static_cast<uint8_t>(ASC_DEFAULT_REPEAT_STRIDE / (sizeof(half) / sizeof(uint8_t)));
    asc_mul_cast_half2uint8_impl(dst, src0, src1, ASC_DEFAULT_REPEAT,
        ASC_DEFAULT_BLOCK_STRIDE, ASC_DEFAULT_BLOCK_STRIDE, ASC_DEFAULT_BLOCK_STRIDE,
        dst_repeat_stride, ASC_DEFAULT_REPEAT_STRIDE, ASC_DEFAULT_REPEAT_STRIDE);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_mul_cast_half2uint8_sync_impl(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_cast_half2uint8_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

#endif