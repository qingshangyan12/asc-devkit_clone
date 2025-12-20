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
 * \file asc_int322int64_impl.h
 * \brief
 */
#ifndef IMPL_INSTR_VECTOR_COMPUTE_VCONV_VCONV_S322S64_NPU_ARCH_2201_ASC_INT322INT64_IMPL_H
#define IMPL_INSTR_VECTOR_COMPUTE_VCONV_VCONV_S322S64_NPU_ARCH_2201_ASC_INT322INT64_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

__aicore__ inline void asc_int322int64_impl(__ubuf__ int64_t* dst, __ubuf__ int32_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_s322s64(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_int322int64_impl(__ubuf__ int64_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_int322int64_impl(dst, src, CAPI_DEFAULT_REPEAT, CAPI_DEFAULT_BLOCK_STRIDE, CAPI_DEFAULT_BLOCK_STRIDE,
        CAPI_DEFAULT_REPEAT_STRIDE, CAPI_DEFAULT_REPEAT_STRIDE / 2);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_int322int64_sync_impl(__ubuf__ int64_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322int64_impl(dst, src, count);
    asc_sync_post_process();
}

#endif