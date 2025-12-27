/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOint16_tT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLint16_tDING Bint16_tT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICint16_tLAR Pint16_tRPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/* !
 * \file asc_vdeqs162b8h_impl.h
 * \brief
 */
#ifndef IMPL_INSTR_VECTOR_COMPint16_tTE_VDEQS162B8H_ASC_2201_VDEQS162B8H_IMPL_H
#define IMPL_INSTR_VECTOR_COMPint16_tTE_VDEQS162B8H_ASC_2201_VDEQS162B8H_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

//dst  int8_t uint8_t
__aicore__ inline void asc_vdeqs162b8h_impl(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_vdeqs162b8h(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_vdeqs162b8h_impl(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_vdeqs162b8h_impl(dst, src, ASC_DEFAULT_REPEAT, ASC_DEFAULT_BLOCK_STRIDE, ASC_DEFAULT_BLOCK_STRIDE,
        ASC_DEFAULT_REPEAT_STRIDE, ASC_HALF_DEFAULT_REPEAT_STRIDE);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_vdeqs162b8h_sync_impl(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8h_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_vdeqs162b8h_impl(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    if ASC_IS_AIV {
    vconv_vdeqs162b8h(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_vdeqs162b8h_impl(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_vdeqs162b8h_impl(dst, src, ASC_DEFAULT_REPEAT, ASC_DEFAULT_BLOCK_STRIDE, ASC_DEFAULT_BLOCK_STRIDE,
        ASC_DEFAULT_REPEAT_STRIDE, ASC_HALF_DEFAULT_REPEAT_STRIDE);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_vdeqs162b8h_sync_impl(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8h_impl(dst, src, count);
    asc_sync_post_process();
}

#endif