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
 * \file asc_le_scalar_impl.h
 * \brief
 */
#ifndef IMPL_INSTR_VECTOR_COMPUTE_VCMPVS_LE_ASC_2201_ASC_LE_SCALAR_IMPL_H
#define IMPL_INSTR_VECTOR_COMPUTE_VCMPVS_LE_ASC_2201_ASC_LE_SCALAR_IMPL_H

namespace CApiInternal {

template <typename SRC_T>
__aicore__ inline void asc_le_scalar_impl(__ubuf__ uint8_t* dst, __ubuf__ SRC_T* src, SRC_T value, const asc_unary_config& config)
{
    vcmpvs_le(reinterpret_cast<__ubuf__ uint8_t*>(dst), src, value, static_cast<uint8_t>(config.repeat), 
          static_cast<uint16_t>(config.dst_block_stride), static_cast<uint16_t>(config.src_block_stride),
          static_cast<uint16_t>(config.dst_repeat_stride), static_cast<uint16_t>(config.src_repeat_stride));
}

template <typename SRC_T>
__aicore__ inline void asc_le_scalar_sync_impl(__ubuf__ uint8_t* dst, __ubuf__ SRC_T* src, SRC_T value, const asc_unary_config& config)
{
    asc_le_scalar_impl(dst, src, value, config);
    pipe_barrier(pipe_t::PIPE_ALL);
}

}

#endif