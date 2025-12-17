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
 * \file lt_scalar_impl.h
 * \brief
 */
#ifndef IMPL_CAPI_C_API_INSTR_IMPL_VECTOR_COMPUTE_C_API_IMPL_LT_SCALAR_IMPL_NPU_ARCH_2201_ASC_LT_SCALAR_IMPL_H
#define IMPL_CAPI_C_API_INSTR_IMPL_VECTOR_COMPUTE_C_API_IMPL_LT_SCALAR_IMPL_NPU_ARCH_2201_ASC_LT_SCALAR_IMPL_H

namespace CApiInternal {

template <typename T>
__aicore__ inline void asc_lt_scalar_impl(__ubuf__ uint8_t* dst, __ubuf__ T* src0, T value, const asc_unary_config& config)
{
    vcmpvs_lt(dst, src0, value, static_cast<uint8_t>(config.repeat), static_cast<uint16_t>(config.dst_block_stride),
            static_cast<uint16_t>(config.src_block_stride), static_cast<uint16_t>(config.dst_repeat_stride),
            static_cast<uint16_t>(config.src_repeat_stride));
}

}

#endif