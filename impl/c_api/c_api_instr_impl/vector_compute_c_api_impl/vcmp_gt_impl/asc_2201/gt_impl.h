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
 * \file gt_impl.h
 * \brief
 */
#ifndef IMPL_INSTR_VECTOR_COMPUTE_VCMP_GT_ASC_2201_GT_IMPL_H
#define IMPL_INSTR_VECTOR_COMPUTE_VCMP_GT_ASC_2201_GT_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

namespace CApiInternal {

template <typename T>
__aicore__ inline void gt_impl(__ubuf__ T* src0, __ubuf__ T* src1, const asc_binary_config& config)
{
    vcmp_gt(src0, src1, static_cast<uint8_t>(config.repeat), static_cast<uint8_t>(config.dst_block_stride),
         static_cast<uint8_t>(config.src0_block_stride), static_cast<uint8_t>(config.src1_block_stride),
         static_cast<uint8_t>(config.dst_repeat_stride), static_cast<uint8_t>(config.src0_repeat_stride),
         static_cast<uint8_t>(config.src1_repeat_stride));
}

template <typename T>
__aicore__ inline void gt_sync_impl(__ubuf__ T* src0, __ubuf__ T* src1, const asc_binary_config& config)
{
    gt_impl<T>(src0, src1, config);
    pipe_barrier(pipe_t::PIPE_ALL);
}

template <typename T>
__aicore__ inline void gt_impl(__ubuf__ uint8_t* dst, __ubuf__ T* src0, __ubuf__ T* src1, const asc_binary_config& config)
{
    vcmpv_gt(dst, src0, src1, static_cast<uint8_t>(config.repeat), static_cast<uint8_t>(config.dst_block_stride),
         static_cast<uint8_t>(config.src0_block_stride), static_cast<uint8_t>(config.src1_block_stride),
         static_cast<uint8_t>(config.dst_repeat_stride), static_cast<uint8_t>(config.src0_repeat_stride),
         static_cast<uint8_t>(config.src1_repeat_stride));
}

template <typename T>
__aicore__ inline void gt_sync_impl(__ubuf__ uint8_t* dst, __ubuf__ T* src0, __ubuf__ T* src1, const asc_binary_config& config)
{
    gt_impl<T>(dst, src0, src1, config);
    pipe_barrier(pipe_t::PIPE_ALL);
}

}

#endif