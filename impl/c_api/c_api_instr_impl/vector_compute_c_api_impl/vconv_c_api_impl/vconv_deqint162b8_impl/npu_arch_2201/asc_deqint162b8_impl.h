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
 * \file asc_deqint162b8_impl.h
 * \brief
 */
#ifndef IMPL_INSTR_VECTOR_COMPUTE_VCONV_DEQ_VCONV_DEQS162B8_ASC_2201_ASC_DEQINT162B8_IMPL_H
#define IMPL_INSTR_VECTOR_COMPUTE_VCONV_DEQ_VCONV_DEQS162B8_ASC_2201_ASC_DEQINT162B8_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

namespace CApiInternal {

template<typename T, typename U>
__aicore__ inline void asc_deqint162b8_h_impl(__ubuf__ T* dst, __ubuf__ U* src, const asc_unary_config& config)
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIV_TYPE))
    vconv_deqs162b8h(dst, src, static_cast<uint8_t>(config.repeat), static_cast<uint16_t>(config.dst_block_stride),
                   static_cast<uint16_t>(config.src_block_stride), static_cast<uint8_t>(config.dst_repeat_stride),
                   static_cast<uint8_t>(config.src_repeat_stride));
#endif
}

template<typename T, typename U>
__aicore__ inline void asc_deqint162b8_h_impl(__ubuf__ T* dst, __ubuf__ U* src, uint32_t count)
{
    set_mask_count();
    set_vector_mask(static_cast<uint64_t>(0), static_cast<uint64_t>(count));
    asc_deqint162b8_h_impl<T, U>(dst, src, CAPI_DEFAULT_UNARY_CFG);
    set_mask_norm();
}

template<typename T, typename U>
__aicore__ inline void asc_deqint162b8_h_sync_impl(__ubuf__ T* dst, __ubuf__ U* src, uint32_t count)
{
    asc_deqint162b8_h_impl<T, U>(dst, src, count);
    pipe_barrier(pipe_t::PIPE_ALL);
}

template<typename T, typename U>
__aicore__ inline void asc_deqint162b8_l_impl(__ubuf__ T* dst, __ubuf__ U* src, const asc_unary_config& config)
{
    vconv_deqs162b8l(dst, src, static_cast<uint8_t>(config.repeat), static_cast<uint16_t>(config.dst_block_stride),
                   static_cast<uint16_t>(config.src_block_stride), static_cast<uint8_t>(config.dst_repeat_stride),
                   static_cast<uint8_t>(config.src_repeat_stride));
}

template<typename T, typename U>
__aicore__ inline void asc_deqint162b8_l_impl(__ubuf__ T* dst, __ubuf__ U* src, uint32_t count)
{
    set_mask_count();
    set_vector_mask(static_cast<uint64_t>(0), static_cast<uint64_t>(count));
    asc_deqint162b8_l_impl<T, U>(dst, src, CAPI_DEFAULT_UNARY_CFG);
    set_mask_norm();
}

template<typename T, typename U>
__aicore__ inline void asc_deqint162b8_l_sync_impl(__ubuf__ T* dst, __ubuf__ U* src, uint32_t count)
{
    asc_deqint162b8_l_impl<T, U>(dst, src, count);
    pipe_barrier(pipe_t::PIPE_ALL);
}

}

#endif