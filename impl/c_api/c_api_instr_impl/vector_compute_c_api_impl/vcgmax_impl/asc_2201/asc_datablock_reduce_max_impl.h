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
 * \file data_block_reduce_max_impl.h
 * \brief
 */
#ifndef IMPL_INSTR_VECTOR_COMPUTE_VCGMAX_IMPL_ASC_2201_ASC_DATA_BLOCK_REDUCE_MAX_IMPL_H
#define IMPL_INSTR_VECTOR_COMPUTE_VCGMAX_IMPL_ASC_2201_ASC_DATA_BLOCK_REDUCE_MAX_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

__aicore__ inline void asc_datablock_reduce_max_impl(__ubuf__ half* dst, __ubuf__ half* src, const asc_block_reduce_config& config)
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIV_TYPE))
    vcgmax(dst, src, static_cast<uint8_t>(config.repeat), static_cast<uint16_t>(config.dst_repeat_stride),
           static_cast<uint16_t>(config.src_block_stride), static_cast<uint16_t>(config.src_repeat_stride));
#endif
}

__aicore__ inline void asc_datablock_reduce_max_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_datablock_reduce_max_impl(dst, src, CAPI_BLOCK_DEFAULT_REDUCE_CFG);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_datablock_reduce_max_sync_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_datablock_reduce_max_impl(dst, src, count);
    asc_sync_impl();
}

__aicore__ inline void asc_datablock_reduce_max_impl(__ubuf__ float* dst, __ubuf__ float* src, const asc_block_reduce_config& config)
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIV_TYPE))
    vcgmax(dst, src, static_cast<uint8_t>(config.repeat), static_cast<uint16_t>(config.dst_repeat_stride),
           static_cast<uint16_t>(config.src_block_stride), static_cast<uint16_t>(config.src_repeat_stride));
#endif
}

__aicore__ inline void asc_datablock_reduce_max_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_datablock_reduce_max_impl(dst, src, CAPI_BLOCK_DEFAULT_REDUCE_CFG);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_datablock_reduce_max_sync_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_datablock_reduce_max_impl(dst, src, count);
    asc_sync_impl();
}

#endif