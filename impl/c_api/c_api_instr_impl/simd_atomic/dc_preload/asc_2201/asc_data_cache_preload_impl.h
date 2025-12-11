/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
 
#ifndef IMPL_C_API_INSTR_SIMD_ATOMIC_DC_PRELOAD_ASC_2201_ASC_DATA_CACHE_PRELOAD_IMPL_H
#define IMPL_C_API_INSTR_SIMD_ATOMIC_DC_PRELOAD_ASC_2201_ASC_DATA_CACHE_PRELOAD_IMPL_H
 
namespace CApiInternal {

template<typename T>
__aicore__ inline void asc_data_cache_preload_impl(uint64_t* address, T offset)
{
    dc_preload(address, offset);
}

}

#endif