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
 * \file asc_le_impl.h
 * \brief
 */
#ifndef IMPL_INSTR_VECTOR_COMPUTE_VCMP_LE_ASC_2201_ASC_LE_IMPL_H
#define IMPL_INSTR_VECTOR_COMPUTE_VCMP_LE_ASC_2201_ASC_LE_IMPL_H

namespace CApiInternal {

template <typename T>
__aicore__ inline void asc_le_impl(__ubuf__ T* src0, __ubuf__ T* src1, const asc_binary_config& config)
{
    vcmp_le(src0, src1, static_cast<uint8_t>(1), static_cast<uint8_t>(config.dst_block_stride),
         static_cast<uint8_t>(config.src0_block_stride), static_cast<uint8_t>(config.src1_block_stride),
         static_cast<uint8_t>(config.dst_repeat_stride), static_cast<uint8_t>(config.src0_repeat_stride),
         static_cast<uint8_t>(config.src1_repeat_stride)); 
}

template <typename T>
__aicore__ inline void asc_le_sync_impl(__ubuf__ T* src0, __ubuf__ T* src1, const asc_binary_config& config)
{
    asc_le_impl<T>(src0, src1, config);
    pipe_barrier(pipe_t::PIPE_ALL);
}

template <typename DST_T, typename SRC_T>
__aicore__ inline void asc_le_impl(__ubuf__ DST_T* dst, __ubuf__ SRC_T* src0, __ubuf__ SRC_T* src1, const asc_binary_config& config)
{
    vcmpv_le(reinterpret_cast<__ubuf__ uint8_t*>(dst), src0, src1, static_cast<uint8_t>(config.repeat),
         static_cast<uint8_t>(config.dst_block_stride), static_cast<uint8_t>(config.src0_block_stride),
         static_cast<uint8_t>(config.src1_block_stride), static_cast<uint8_t>(config.dst_repeat_stride),
         static_cast<uint8_t>(config.src0_repeat_stride), static_cast<uint8_t>(config.src1_repeat_stride)); 
}

template <typename DST_T, typename SRC_T>
__aicore__ inline void asc_le_sync_impl(__ubuf__ DST_T* dst, __ubuf__ SRC_T* src0, __ubuf__ SRC_T* src1, const asc_binary_config& config)
{
    asc_le_impl<DST_T, SRC_T>(dst, src0, src1, config);
    pipe_barrier(pipe_t::PIPE_ALL);
}

}

#endif