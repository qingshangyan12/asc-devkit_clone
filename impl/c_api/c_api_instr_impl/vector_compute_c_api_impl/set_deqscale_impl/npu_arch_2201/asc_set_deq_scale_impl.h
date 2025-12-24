/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef IMPL_C_API_INSTR_VECTOR_COMPUTE_SET_DEQSCALE_NPU_ARCH_2201_ASC_SET_DEQSCALE_IMPL_H
#define IMPL_C_API_INSTR_VECTOR_COMPUTE_SET_DEQSCALE_NPU_ARCH_2201_ASC_SET_DEQSCALE_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

__aicore__ inline void asc_set_deq_scale_impl(const asc_deq_scale_config config)
{
    if ASC_IS_AIV {
        set_deqscale(config.val);
    }
}

__aicore__ inline void asc_set_deq_scale_impl(__ubuf__ uint64_t* config)
{
    if ASC_IS_AIV {
        constexpr uint64_t deq_addr = 5;
        set_deqscale((uint64_t)config >> deq_addr);
    }
}

#endif