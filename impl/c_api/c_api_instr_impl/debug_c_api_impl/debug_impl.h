/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
 
#ifndef IMPL_C_API_C_API_INSTR_IMPL_DEBUG_C_API_IMPL_DEBUG_IMPL_H
#define IMPL_C_API_C_API_INSTR_IMPL_DEBUG_C_API_IMPL_DEBUG_IMPL_H
 
#include "asc_2201/dump_routing.h"
template <typename T>
__aicore__ inline void asc_dump_gm(__gm__ T* input, uint32_t desc, uint32_t dump_size)
{
    using dump = dump_routing_gm<CURRENT_ARCH_VERSION>::type;
    dump{}.template run<T>(input, desc, dump_size);
}

template <typename T>
__aicore__ inline void asc_dump_ubuf(__ubuf__ T* input, uint32_t desc, uint32_t dump_size)
{
    using dump = dump_routing_ubuf<CURRENT_ARCH_VERSION>::type;
    dump{}.template run<T>(input, desc, dump_size);
}

#endif