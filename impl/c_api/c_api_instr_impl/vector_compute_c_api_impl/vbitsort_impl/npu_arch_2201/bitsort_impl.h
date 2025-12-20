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
 * \file asc_bitsort_impl.h
 * \brief
 */
#ifndef IMPL_INSTR_VECTOR_COMPUTE_VBITSORT_IMPL_NPU_ARCH_2201_BITSORT_IMPL_H
#define IMPL_INSTR_VECTOR_COMPUTE_VBITSORT_IMPL_NPU_ARCH_2201_BITSORT_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

__aicore__ inline void asc_bitsort_impl_half(__ubuf__ half *dst, __ubuf__ half *src0, __ubuf__ uint32_t *src1, int32_t repeat)
{
    if ASC_IS_AIV {
        vbitsort(dst, src0, src1, repeat);
    }
}

__aicore__ inline void asc_bitsort_sync_impl_half(__ubuf__ half *dst, __ubuf__ half *src0, __ubuf__ uint32_t *src1, int32_t repeat)
{
    asc_bitsort_impl_half(dst, src0, src1, repeat);
    asc_sync_post_process();
}

__aicore__ inline void asc_bitsort_impl_float(__ubuf__ float *dst, __ubuf__ float *src0, __ubuf__ uint32_t *src1, int32_t repeat)
{
    if ASC_IS_AIV {
        vbitsort(dst, src0, src1, repeat);
    }
}

__aicore__ inline void asc_bitsort_sync_impl_float(__ubuf__ float *dst, __ubuf__ float *src0, __ubuf__ uint32_t *src1, int32_t repeat)
{
    asc_bitsort_impl_float(dst, src0, src1, repeat);
    asc_sync_post_process();
}

#endif