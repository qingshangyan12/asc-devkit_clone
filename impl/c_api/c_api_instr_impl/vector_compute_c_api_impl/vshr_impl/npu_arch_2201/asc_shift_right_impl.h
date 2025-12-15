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
 * \file asc_shift_right_impl.h
 * \brief
 */
#ifndef IMPL_INSTR_VECTOR_COMPUTE_VSHR_ASC_2201_SHIFT_RIGHT_IMPL_H
#define IMPL_INSTR_VECTOR_COMPUTE_VSHR_ASC_2201_SHIFT_RIGHT_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

namespace CApiInternal {

template <typename T>
__aicore__ inline void asc_shift_right_impl(__ubuf__ T* dst, __ubuf__ T* src, T value, const asc_unary_config& config,
    bool round)
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIV_TYPE))
    if (round) {
        if constexpr (std::is_same<T, int16_t>::value || std::is_same<T, int32_t>::value) {
            vshr(dst, src, static_cast<int32_t>(value), static_cast<uint8_t>(config.repeat), static_cast<uint16_t>(
                config.dst_block_stride), static_cast<uint16_t>(config.src_block_stride), static_cast<uint16_t>(
                config.dst_repeat_stride), static_cast<uint16_t>(config.src_repeat_stride), true);
        } else {
            vshr(dst, src, static_cast<uint32_t>(value), static_cast<uint8_t>(config.repeat), static_cast<uint16_t>(
                config.dst_block_stride), static_cast<uint16_t>(config.src_block_stride), static_cast<uint16_t>(
                config.dst_repeat_stride), static_cast<uint16_t>(config.src_repeat_stride), true);
        }
    } else {
        if constexpr (std::is_same<T, int16_t>::value || std::is_same<T, int32_t>::value) {
            vshr(dst, src, static_cast<int32_t>(value), static_cast<uint8_t>(config.repeat), static_cast<uint16_t>(
                config.dst_block_stride), static_cast<uint16_t>(config.src_block_stride), static_cast<uint16_t>(
                config.dst_repeat_stride), static_cast<uint16_t>(config.src_repeat_stride), false);
        } else {
            vshr(dst, src, static_cast<uint32_t>(value), static_cast<uint8_t>(config.repeat), static_cast<uint16_t>(
                config.dst_block_stride), static_cast<uint16_t>(config.src_block_stride), static_cast<uint16_t>(
                config.dst_repeat_stride), static_cast<uint16_t>(config.src_repeat_stride), false);
        }
    }
#endif
}

template <typename T>
__aicore__ inline void asc_shift_right_impl(__ubuf__ T* dst, __ubuf__ T* src, T value, uint32_t count, bool round)
{
    set_mask_count();
    set_vector_mask(static_cast<uint64_t>(0), static_cast<uint64_t>(count));
    asc_shift_right_impl<T>(dst, src, value, CAPI_DEFAULT_UNARY_CFG, round);
    set_mask_norm();
}

template <typename T>
__aicore__ inline void asc_shift_right_sync_impl(__ubuf__ T* dst, __ubuf__ T* src, T value, uint32_t count, bool round)
{
    asc_shift_right_impl<T>(dst, src, value, count, round);
    pipe_barrier(pipe_t::PIPE_ALL);
}

}

#endif