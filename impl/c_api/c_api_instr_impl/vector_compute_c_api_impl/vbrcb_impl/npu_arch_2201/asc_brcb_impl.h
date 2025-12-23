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
 * \file brcb_impl.h
 * \brief
 */
#ifndef IMPL_INSTR_VECTOR_COMPUTE_VBRCB_IMPL_ASC_2201_ASC_BRCB_IMPL_H
#define IMPL_INSTR_VECTOR_COMPUTE_VBRCB_IMPL_ASC_2201_ASC_BRCB_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

__aicore__ inline void asc_brcb_impl(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src,
    uint16_t dst_block_stride, uint16_t dst_repeat_stride, uint8_t repeat)
{
    if ASC_IS_AIV {
        vbrcb(dst, src, dst_block_stride, dst_repeat_stride, repeat);
    }
}

__aicore__ inline void asc_brcb_sync_impl(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src,
    uint16_t dst_block_stride, uint16_t dst_repeat_stride, uint8_t repeat)
{
    asc_brcb_impl(dst, src, dst_block_stride, dst_repeat_stride, repeat);
    asc_sync_post_process();
}

__aicore__ inline void asc_brcb_impl(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src,
    uint16_t dst_block_stride, uint16_t dst_repeat_stride, uint8_t repeat)
{
    if ASC_IS_AIV {
        vbrcb(dst, src, dst_block_stride, dst_repeat_stride, repeat);
    }
}

__aicore__ inline void asc_brcb_sync_impl(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src,
    uint16_t dst_block_stride, uint16_t dst_repeat_stride, uint8_t repeat)
{
    asc_brcb_impl(dst, src, dst_block_stride, dst_repeat_stride, repeat);
    asc_sync_post_process();
}

#endif