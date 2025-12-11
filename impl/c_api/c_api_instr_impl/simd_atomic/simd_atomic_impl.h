/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef IMPL_C_API_INSTR_SIMD_ATOMIC_H
#define IMPL_C_API_INSTR_SIMD_ATOMIC_H

#include "dcci/asc_2201/dcci_impl.h"
#include "set_atomic_min/asc_2201/asc_set_atomic_min_impl.h"
#include "dc_preload/asc_2201/asc_data_cache_preload_impl.h"

__aicore__ inline void asc_data_cache_clean_and_invalid(__gm__ void* dst, uint64_t entire)
{
    CApiInternal::data_cache_clean_and_invalid_impl(dst, entire);
}

__aicore__ inline void asc_set_atomic_min()
{
    CApiInternal::asc_set_atomic_min_impl();
}

__aicore__ inline void asc_data_cache_preload(uint64_t* address, int16_t offset)
{
    CApiInternal::asc_data_cache_preload_impl<int16_t>(address, offset);
}

__aicore__ inline void asc_data_cache_preload(uint64_t* address, int64_t offset)
{
    CApiInternal::asc_data_cache_preload_impl<int64_t>(address, offset);
}

#endif