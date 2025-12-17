/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef IMPL_C_API_C_API_INSTR_IMPL_CACHE_CTRL_C_API_IMPL_CACHE_CTRL_C_API_IMPL_H
#define IMPL_C_API_C_API_INSTR_IMPL_CACHE_CTRL_C_API_IMPL_CACHE_CTRL_C_API_IMPL_H

#include "get_icache_preload_status_impl/npu_arch_2201/get_icache_preload_status_impl.h"
#include "dc_preload_impl/asc_2201/asc_data_cache_preload_impl.h"
#include "dcci_impl/npu_arch_2201/asc_dcci_impl.h"

__aicore__ inline void asc_dcci(__gm__ void* dst, uint64_t entire)
{
    CApiInternal::asc_dcci_impl(dst, entire);
}

__aicore__ inline void asc_dcci(__gm__ void* dst, uint64_t entire, uint64_t type)
{
    CApiInternal::asc_dcci_impl(dst, entire, type);
}

__aicore__ inline void asc_data_cache_preload(uint64_t* address, int16_t offset)
{
    CApiInternal::asc_data_cache_preload_impl<int16_t>(address, offset);
}

__aicore__ inline void asc_data_cache_preload(uint64_t* address, int64_t offset)
{
    CApiInternal::asc_data_cache_preload_impl<int64_t>(address, offset);
}

__aicore__ inline int64_t asc_get_icache_preload_status()
{
    return CApiInternal::asc_get_icache_preload_status_impl();
}

#endif