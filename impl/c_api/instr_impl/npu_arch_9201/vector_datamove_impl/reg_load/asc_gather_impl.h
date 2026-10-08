/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file asc_gather_impl.h
 * \brief 9201 register-load Gather implementations.
 */

#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_9201_VECTOR_DATAMOVE_IMPL_REG_LOAD_ASC_GATHER_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_9201_VECTOR_DATAMOVE_IMPL_REG_LOAD_ASC_GATHER_IMPL_H

#include "instr_impl/npu_arch_9201/utils_impl.h"

__simd_callee__ inline void asc_gather_impl(
    vector_fp8_e6m2_t& dst, __ubuf__ fp8_e6m2_t* src, vector_uint16_t index, vector_bool mask)
{
    if ASC_IS_AIV {
        vgather2(dst, src, index, mask);
    }
}

__simd_callee__ inline void asc_gather_datablock_impl(
    vector_fp8_e6m2_t& dst, __ubuf__ fp8_e6m2_t* src, vector_uint32_t index, vector_bool mask)
{
    if ASC_IS_AIV {
        vgatherb(dst, src, index, mask);
    }
}

__simd_callee__ inline void asc_gather_datablock_impl(
    vector_fp8_e6m2_t& dst, __ubuf__ fp8_e6m2_t* src, vector_uint32_t index)
{
    if ASC_IS_AIV {
        vgatherb(dst, src, index);
    }
}

#endif
