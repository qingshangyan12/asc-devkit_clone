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
 * \file asc_eq_impl.h
 * \brief
 */

#ifndef IMPL_INSTR_VECTOR_COMPUTE_VCMP_EQ_ASC_2201_ASC_EQ_IMPL_H
#define IMPL_INSTR_VECTOR_COMPUTE_VCMP_EQ_ASC_2201_ASC_EQ_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

namespace CApiInternal {

template<typename T>
__aicore__ inline void asc_eq_impl(__ubuf__ T* src0, __ubuf__ T* src1, const asc_binary_config& config)
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIV_TYPE))
    vcmp_eq(src0, src1, static_cast<uint8_t>(config.repeat), static_cast<uint8_t>(config.dst_block_stride),
         static_cast<uint8_t>(config.src0_block_stride), static_cast<uint8_t>(config.src1_block_stride),
         static_cast<uint8_t>(config.dst_repeat_stride), static_cast<uint8_t>(config.src0_repeat_stride),
         static_cast<uint8_t>(config.src1_repeat_stride));
#endif
}

template<typename T>
__aicore__ inline void asc_eq_sync_impl(__ubuf__ T* src0, __ubuf__ T* src1, const asc_binary_config& config)
{
    asc_eq_impl<T>(src0, src1, config);
    pipe_barrier(pipe_t::PIPE_ALL);
}

template<typename DstT, typename SrcT>
__aicore__ inline void asc_eq_impl(__ubuf__ DstT* dst, __ubuf__ SrcT* src0, __ubuf__ SrcT* src1, const asc_binary_config& config)
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIV_TYPE))
    vcmpv_eq(dst, src0, src1, static_cast<uint8_t>(config.repeat), static_cast<uint8_t>(config.dst_block_stride),
         static_cast<uint8_t>(config.src0_block_stride), static_cast<uint8_t>(config.src1_block_stride),
         static_cast<uint8_t>(config.dst_repeat_stride), static_cast<uint8_t>(config.src0_repeat_stride),
         static_cast<uint8_t>(config.src1_repeat_stride));
#endif
}

template<typename DstT, typename SrcT>
__aicore__ inline void asc_eq_sync_impl(__ubuf__ DstT* dst, __ubuf__ SrcT* src0, __ubuf__ SrcT* src1, const asc_binary_config& config)
{
    asc_eq_impl<DstT, SrcT>(dst, src0, src1, config);
    pipe_barrier(pipe_t::PIPE_ALL);
}

}

#endif