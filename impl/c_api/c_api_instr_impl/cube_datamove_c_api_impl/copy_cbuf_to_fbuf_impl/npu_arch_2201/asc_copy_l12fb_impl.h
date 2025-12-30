/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef IMPL_C_API_C_API_INSTR_IMPL_CUBE_DATAMOVE_C_API_IMPL_COPY_CBUF_TO_FBUF_IMPL_NPU_ARCH_2201_ASC_COPY_L12FB_IMPL_H
#define IMPL_C_API_C_API_INSTR_IMPL_CUBE_DATAMOVE_C_API_IMPL_COPY_CBUF_TO_FBUF_IMPL_NPU_ARCH_2201_ASC_COPY_L12FB_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

__aicore__ inline void asc_copy_l12fb_impl(__fbuf__ void* dst, __cbuf__ void* src, uint16_t burst_num, uint16_t burst_len,
                                      uint16_t src_gap_size, uint16_t dst_gap_size)
{
    if ASC_IS_AIC {
        copy_cbuf_to_fbuf(dst, src, burst_num, burst_len, src_gap_size, dst_gap_size);
    }
}

__aicore__ inline void asc_copy_l12fb_impl(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size)
{
    asc_copy_l12fb_impl(dst, src, 1, size / ASC_ONE_DATABLOCK_SIZE, 0, 0);
}

__aicore__ inline void asc_copy_l12fb_sync_impl(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size)
{
    asc_copy_l12fb_impl(dst, src, size);
    asc_sync_post_process();
}

#endif