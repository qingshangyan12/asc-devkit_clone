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
 * \file sub_impl.h
 * \brief
 */
#ifndef IMPL_CAPI_C_API_INSTR_IMPL_VECTOR_COMPUTE_C_API_IMPL_LEAKYRELU_SCALAR_IMPL_NPU_ARCH_2201_ASC_LEAKYRELU_SCALAR_IMPL_H
#define IMPL_CAPI_C_API_INSTR_IMPL_VECTOR_COMPUTE_C_API_IMPL_LEAKYRELU_SCALAR_IMPL_NPU_ARCH_2201_ASC_LEAKYRELU_SCALAR_IMPL_H

namespace CApiInternal {

template <typename T>
__aicore__ inline void asc_leakyrelu_impl(__ubuf__ T* dst, __ubuf__ T* src0, T src1, const asc_unary_config& config)
{
    vlrelu(dst, src0, src1, static_cast<uint8_t>(config.repeat), static_cast<uint16_t>(config.dst_block_stride),
          static_cast<uint16_t>(config.src_block_stride), static_cast<uint16_t>(config.dst_repeat_stride),
          static_cast<uint16_t>(config.src_repeat_stride));
}

template <typename T>
__aicore__ inline void asc_leakyrelu_impl(__ubuf__ T* dst, __ubuf__ T* src0, T src1, uint32_t count)
{
    set_mask_count();
    set_vector_mask(static_cast<uint64_t>(0), static_cast<uint64_t>(count));
    asc_leakyrelu_impl<T>(dst, src0, src1, CAPI_DEFAULT_UNARY_CFG);
    set_mask_norm();
}

template <typename T>
__aicore__ inline void asc_leakyrelu_sync_impl(__ubuf__ T* dst, __ubuf__ T* src0, T src1, uint32_t count)
{
    asc_leakyrelu_impl<T>(dst, src0, src1, count);
    pipe_barrier(pipe_t::PIPE_ALL);
}

}

#endif