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
 * \file asc_addreluconv_f162s8_impl.h
 * \brief
 */
#ifndef IMPL_INSTR_VECTOR_COMPUTE_VADDRELUCONV_F162S8_NPU_ARCH_2201_ADDRELUCONV_F162S8_IMPL_H
#define IMPL_INSTR_VECTOR_COMPUTE_VADDRELUCONV_F162S8_NPU_ARCH_2201_ADDRELUCONV_F162S8_IMPL_H
#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

__aicore__ inline void asc_add_relu_cast_f162s8_impl(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, 
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    vaddreluconv_f162s8(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride, false);
}

__aicore__ inline void asc_add_relu_cast_f162s8_impl(__ubuf__ int8_t* dst,  __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_add_relu_cast_f162s8_impl(dst, src0, src1, CAPI_DEFAULT_REPEAT, CAPI_DEFAULT_BLOCK_STRIDE, CAPI_DEFAULT_BLOCK_STRIDE, CAPI_DEFAULT_BLOCK_STRIDE, CAPI_DEFAULT_REPEAT_STRIDE,
        CAPI_DEFAULT_REPEAT_STRIDE, CAPI_DEFAULT_REPEAT_STRIDE);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_add_relu_cast_f162s8_sync_impl(__ubuf__ int8_t* dst,  __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_add_relu_cast_f162s8_impl(dst, src0, src1, count);
    pipe_barrier(pipe_t::PIPE_ALL);
}

#endif