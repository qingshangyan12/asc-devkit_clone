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
 * \file asc_sub_relu_cast_f162s8_impl.h
 * \brief
 */
#ifndef IMPL_CAPI_C_API_INSTR_IMPL_VECTOR_COMPUTE_C_API_IMPL_SUB_RELU_CAST_IMPL_NPU_ARCH_2201_ASC_SUB_RELU_CAST_F162S8_IMPL_H
#define IMPL_CAPI_C_API_INSTR_IMPL_VECTOR_COMPUTE_C_API_IMPL_SUB_RELU_CAST_IMPL_NPU_ARCH_2201_ASC_SUB_RELU_CAST_F162S8_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

__aicore__ inline void asc_sub_relu_castf162s8_impl(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride, bool h)
{
    if ASC_IS_AIV {
        vsubreluconv_f162s8(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride, h);
    }
}

__aicore__ inline void asc_sub_relu_castf162s8_impl(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_sub_relu_castf162s8_impl(dst, src0, src1, ASC_DEFAULT_REPEAT, ASC_DEFAULT_BLOCK_STRIDE, ASC_DEFAULT_BLOCK_STRIDE, ASC_DEFAULT_BLOCK_STRIDE, ASC_DEFAULT_REPEAT_STRIDE, ASC_DEFAULT_REPEAT_STRIDE / ASC_ONE_FOURTH_DEFAULT_REPEAT_STRIDE, ASC_DEFAULT_REPEAT_STRIDE / ASC_ONE_FOURTH_DEFAULT_REPEAT_STRIDE, false);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_sub_relu_castf162s8_sync_impl(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_sub_relu_castf162s8_impl(dst, src0, src1, count);
    asc_sync_post_process();
}

#endif