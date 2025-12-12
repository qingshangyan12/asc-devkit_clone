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
 * \file half2float_impl.h
 * \brief
 */
#ifndef IMPL_INSTR_VECTOR_COMPUTE_VCONV_HALF2FLOAT_IMPL_ASC_2201_HALF2FLOAT_IMPL_H
#define IMPL_INSTR_VECTOR_COMPUTE_VCONV_HALF2FLOAT_IMPL_ASC_2201_HALF2FLOAT_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

namespace CApiInternal {

template<typename T, typename U>
__aicore__ inline void asc_half2float_impl(__ubuf__ T* dst, __ubuf__ U* src, const asc_unary_config& config)
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIV_TYPE))
    vconv_f162f32(dst, src, static_cast<uint8_t>(config.repeat), static_cast<uint16_t>(config.dst_block_stride),
                   static_cast<uint16_t>(config.src_block_stride), static_cast<uint16_t>(config.dst_repeat_stride),
                   static_cast<uint16_t>(config.src_repeat_stride));
#endif
}

template<typename T, typename U>
__aicore__ inline void asc_half2float_impl(__ubuf__ T* dst, __ubuf__ U* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_unary_config config{};
    config.dst_block_stride = 1;
    config.src_block_stride = 1;
    config.dst_repeat_stride = 8;
    config.src_repeat_stride = 4;
    config.repeat = 1;
    asc_half2float_impl<T, U>(dst, src, config);
    asc_set_mask_count_end();
}

template<typename T, typename U>
__aicore__ inline void asc_half2float_sync_impl(__ubuf__ T* dst, __ubuf__ U* src, uint32_t count)
{
    asc_half2float_impl<T, U>(dst, src, count);
    asc_sync_impl();
}

}

#endif