/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
 
#ifndef INCLUDE_C_API_SIMD_ATOMIC_H
#define INCLUDE_C_API_SIMD_ATOMIC_H
 
#include "c_api_instr_impl/cache_ctrl_c_api_impl/cache_ctrl_c_api_impl.h"

__aicore__ inline int64_t asc_get_icache_preload_status();

__aicore__ inline void asc_dcci(__gm__ void* dst, uint64_t entire);

__aicore__ inline void asc_dcci(__gm__ void* dst, uint64_t entire, uint64_t type);

__aicore__ inline void asc_icache_preload(const void* addr);

__aicore__ inline void asc_icache_preload(const void* addr, int64_t prefetch_len);
 
__aicore__ inline void asc_datacache_preload(__gm__ uint64_t* address, int64_t offset);

#endif