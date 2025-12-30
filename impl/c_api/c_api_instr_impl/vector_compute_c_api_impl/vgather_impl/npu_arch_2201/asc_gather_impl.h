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
 * \file asc_gather_impl.h
 * \brief
 */
#ifndef IMPL_C_API_C_API_INSTR_IMPL_VECTOR_COMPUTE_C_API_IMPL_VGATHER_IMPL_NPU_ARCH_2201_ASC_GATHER_IMPL_H
#define IMPL_C_API_C_API_INSTR_IMPL_VECTOR_COMPUTE_C_API_IMPL_VGATHER_IMPL_NPU_ARCH_2201_ASC_GATHER_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

// asc_gather uint16_t
__aicore__ inline void asc_gather_impl(__ubuf__ uint16_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr,
    uint16_t dst_repeat_stride, uint8_t repeat)
{
    if ASC_IS_AIV {
        vgather(dst, src, offset_addr, dst_repeat_stride, repeat);
    }
}

__aicore__ inline void asc_gather_impl(__ubuf__ uint16_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr,
    uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_gather_impl(dst, src, offset_addr, ASC_DEFAULT_REPEAT_STRIDE, ASC_DEFAULT_REPEAT);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_gather_sync_impl(__ubuf__ uint16_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr,
    uint32_t count)
{
    asc_gather_impl(dst, src, offset_addr, count);
    asc_sync_post_process();
}

// asc_gather uint32_t
__aicore__ inline void asc_gather_impl(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr,
    uint16_t dst_repeat_stride, uint8_t repeat)
{
    if ASC_IS_AIV {
        vgather(dst, src, offset_addr, dst_repeat_stride, repeat);
    }
}

__aicore__ inline void asc_gather_impl(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr,
    uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_gather_impl(dst, src, offset_addr, ASC_DEFAULT_REPEAT_STRIDE, ASC_DEFAULT_REPEAT);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_gather_sync_impl(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr,
    uint32_t count)
{
    asc_gather_impl(dst, src, offset_addr, count);
    asc_sync_post_process();
}

#endif