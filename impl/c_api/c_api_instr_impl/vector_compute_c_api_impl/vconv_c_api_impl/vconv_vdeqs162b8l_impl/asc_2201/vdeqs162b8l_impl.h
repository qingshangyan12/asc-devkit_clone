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
 * \file vdeqs162b8l_impl.h
 * \brief
 */
#ifndef IMPL_INSTR_VECTOR_COMPUTE_VCONV_VDEQS162B8L_ASC_2201_VDEQ-INT162B8L_IMPL_H
#define IMPL_INSTR_VECTOR_COMPUTE_VCONV_VDEQS162B8L_ASC_2201_VDEQ-INT162B8L_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

namespace CApiInternal{
template<typename T, typename V>
__aicore__ inline void vdeq_int162b8l_impl(__ubuf__ T* dst, __ubuf__ V* src, const asc_unary_config& config)
{
    vconv_vdeqs162b8l(dst, src, static_cast<uint8_t>(config.repeat), static_cast<uint16_t>(config.dst_block_stride),
                   static_cast<uint16_t>(config.src_block_stride), static_cast<uint8_t>(config.dst_repeat_stride),
                   static_cast<uint8_t>(config.src_repeat_stride));
}

template<typename T, typename V>
__aicore__ inline void vdeq_int162b8l_impl(__ubuf__ T* dst, __ubuf__ V* src, uint32_t count)
{
    set_mask_count();
    set_vector_mask(static_cast<uint64_t>(0), static_cast<uint64_t>(count));
    vdeq_int162b8l_impl<T, V>(dst, src, CAPI_DEFAULT_UNARY_CFG);
    set_mask_norm();
}

template<typename T, typename V>
__aicore__ inline void vdeq_int162b8l_sync_impl(__ubuf__ T* dst, __ubuf__ V* src, uint32_t count)
{
    vdeq_int162b8l_impl<T, V>(dst, src, count);
    pipe_barrier(pipe_t::PIPE_ALL);
}

}

#endif