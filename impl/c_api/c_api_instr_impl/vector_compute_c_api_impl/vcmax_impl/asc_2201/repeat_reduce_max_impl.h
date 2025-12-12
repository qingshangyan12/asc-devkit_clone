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
 * \file repeat_reduce_max_impl.h
 * \brief
 */
#ifndef IMPL_INSTR_VECTOR_COMPUTE_VCMAX_IMPL_ASC_2201_REPEAT_REDUCE_MAX_IMPL_H
#define IMPL_INSTR_VECTOR_COMPUTE_VCMAX_IMPL_ASC_2201_REPEAT_REDUCE_MAX_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

namespace CApiInternal {

template <typename T>
__aicore__ inline void asc_repeat_reduce_max_impl(__ubuf__ T* dst, __ubuf__ T* src, const asc_repeat_reduce_config& config, order_t order)
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIV_TYPE))
    if (order_t::ONLY_INDEX == order) {
        vcmax(dst, src, static_cast<uint8_t>(config.repeat), static_cast<uint16_t>(config.dst_repeat_stride),
           static_cast<uint16_t>(config.src_block_stride), static_cast<uint16_t>(config.src_repeat_stride), Order_t::ONLY_INDEX);
    } else if (order_t::ONLY_VALUE == order) {
        vcmax(dst, src, static_cast<uint8_t>(config.repeat), static_cast<uint16_t>(config.dst_repeat_stride),
           static_cast<uint16_t>(config.src_block_stride), static_cast<uint16_t>(config.src_repeat_stride), Order_t::ONLY_VALUE);
    } else if (order_t::VALUE_INDEX == order) {
        vcmax(dst, src, static_cast<uint8_t>(config.repeat), static_cast<uint16_t>(config.dst_repeat_stride),
           static_cast<uint16_t>(config.src_block_stride), static_cast<uint16_t>(config.src_repeat_stride), Order_t::VALUE_INDEX);
    } else {
        vcmax(dst, src, static_cast<uint8_t>(config.repeat), static_cast<uint16_t>(config.dst_repeat_stride),
           static_cast<uint16_t>(config.src_block_stride), static_cast<uint16_t>(config.src_repeat_stride), Order_t::INDEX_VALUE);
    }
#endif
}

template <typename T>
__aicore__ inline void asc_repeat_reduce_max_impl(__ubuf__ T* dst, __ubuf__ T* src, uint32_t count, order_t order)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_max_impl<T>(dst, src, CAPI_REPEAT_DEFAULT_REDUCE_CFG, order);
    asc_set_mask_count_end();
}

template <typename T>
__aicore__ inline void asc_repeat_reduce_max_sync_impl(__ubuf__ T* dst, __ubuf__ T* src, uint32_t count, order_t order)
{
    asc_repeat_reduce_max_impl<T>(dst, src, count, order);
    asc_sync_impl();
}

}

#endif