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
 * \file copy_ubuf_to_gm_impl.h
 * \brief
 */
#ifndef IMPL_INSTR_VECTOR_DATAMOVE_COPY_UBUF_TO_GM_IMPL_NPU_ARCH_2201_ASC_COPY_UBUF_TO_GM_IMPL_H
#define IMPL_INSTR_VECTOR_DATAMOVE_COPY_UBUF_TO_GM_IMPL_NPU_ARCH_2201_ASC_COPY_UBUF_TO_GM_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

__aicore__ inline void asc_copy_ub2gm_impl(__gm__ void* dst, __ubuf__ void* src, uint8_t sid, uint16_t n_burst,
    uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm(dst, src, sid, n_burst, burst_len, src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_ub2gm_impl(__gm__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_ub2gm_impl(dst, src, 0, 1, size / ASC_ONE_DATABLOCK_SIZE, 0, 0);
}

__aicore__ inline void asc_copy_ub2gm_sync_impl(__gm__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_ub2gm_impl(dst, src, size);
    asc_sync_post_process();
}

#endif