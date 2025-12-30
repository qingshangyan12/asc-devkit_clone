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
 * \file asc_copy_ubuf_to_gm_align_impl.h
 * \brief
 */
#ifndef IMPL_INSTR_VECTOR_DMAMOVE_COPY_UBUF_TO_GM_ALIGN_NPU_ARCH_2201_ASC_COPY_UBUF_TO_GM_ALIGN_IMPL_H
#define IMPL_INSTR_VECTOR_DMAMOVE_COPY_UBUF_TO_GM_ALIGN_NPU_ARCH_2201_ASC_COPY_UBUF_TO_GM_ALIGN_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

constexpr uint8_t ASC_ALIGIN_DEFAULT_SID_PADDING_GAP = 0;
constexpr uint16_t ASC_ALIGIN_DEFAULT_NBURST = 1;
constexpr uint32_t ASC_ALIGIN_DEFAULT_LEN_BURST = 0;

__aicore__ inline void asc_copy_ub2gm_align_impl(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint8_t sid, uint16_t n_burst, uint32_t len_burst,
            uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    copy_ubuf_to_gm_align_b8((__ubuf__ void*)dst, (__gm__ void*)src, sid, n_burst, len_burst, left_padding_num, right_padding_num, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__ubuf__  uint8_t* dst, __gm__ uint8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, ASC_ALIGIN_DEFAULT_SID_PADDING_GAP, ASC_ALIGIN_DEFAULT_NBURST, size / ASC_ONE_DATABLOCK_SIZE, ASC_ALIGIN_DEFAULT_SID_PADDING_GAP,
                ASC_ALIGIN_DEFAULT_SID_PADDING_GAP, ASC_ALIGIN_DEFAULT_SID_PADDING_GAP, ASC_ALIGIN_DEFAULT_SID_PADDING_GAP);
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint8_t sid, uint16_t n_burst, uint32_t len_burst,
            uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    copy_ubuf_to_gm_align_b16((__ubuf__ void*)dst, (__gm__ void*)src, sid, n_burst, len_burst, left_padding_num, right_padding_num, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, ASC_ALIGIN_DEFAULT_SID_PADDING_GAP, ASC_ALIGIN_DEFAULT_NBURST, size / ASC_ONE_DATABLOCK_SIZE, ASC_ALIGIN_DEFAULT_SID_PADDING_GAP,
                ASC_ALIGIN_DEFAULT_SID_PADDING_GAP, ASC_ALIGIN_DEFAULT_SID_PADDING_GAP, ASC_ALIGIN_DEFAULT_SID_PADDING_GAP);
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint8_t sid, uint16_t n_burst, uint32_t len_burst,
            uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
{
    copy_ubuf_to_gm_align_b32((__ubuf__ void*)dst, (__gm__ void*)src, sid, n_burst, len_burst, left_padding_num, right_padding_num, src_gap, dst_gap);
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, ASC_ALIGIN_DEFAULT_SID_PADDING_GAP, ASC_ALIGIN_DEFAULT_NBURST, size / ASC_ONE_DATABLOCK_SIZE, ASC_ALIGIN_DEFAULT_SID_PADDING_GAP,
                ASC_ALIGIN_DEFAULT_SID_PADDING_GAP, ASC_ALIGIN_DEFAULT_SID_PADDING_GAP, ASC_ALIGIN_DEFAULT_SID_PADDING_GAP);
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

#endif