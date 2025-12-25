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
 * \file utils_c_api_impl.h
 * \brief
 */
#ifndef IMPL_C_API_C_API_INSTR_IMPL_UTILS_C_API_IMPL_UTILS_C_API_IMPL_H
#define IMPL_C_API_C_API_INSTR_IMPL_UTILS_C_API_IMPL_UTILS_C_API_IMPL_H

#include <cstdint>
#include <type_traits>
#include "c_api_utils_intf.h"
#include "utils_c_api_type.h"

constexpr uint32_t C_API_AIC_TYPE = 1;
constexpr uint32_t C_API_AIV_TYPE = 2;
constexpr uint32_t C_API_MIX_TYPE = 3;

#if defined(ASCENDC_CPU_DEBUG)
    extern uint32_t g_c_api_core_type;
    #define ASC_IS_AIV (g_c_api_core_type == C_API_AIV_TYPE)
    #define ASC_IS_AIC (g_c_api_core_type == C_API_AIC_TYPE)
    #define ASC_IS_NOT_AIV (g_c_api_core_type != C_API_AIV_TYPE)
    #define ASC_IS_NOT_AIC (g_c_api_core_type != C_API_AIC_TYPE)
#else
    #if (defined(__DAV_CUBE__) && defined(__NPU_ARCH__) && __NPU_ARCH__ == 2201)
        constexpr uint32_t g_c_api_core_type = C_API_AIC_TYPE;
    #elif (defined(__DAV_VEC__) && defined(__NPU_ARCH__) && __NPU_ARCH__ == 2201)
        constexpr uint32_t g_c_api_core_type = C_API_AIV_TYPE;
    #else
        constexpr uint32_t g_c_api_core_type = C_API_MIX_TYPE;
    #endif
    #define ASC_IS_AIV constexpr(g_c_api_core_type == C_API_AIV_TYPE)
    #define ASC_IS_AIC constexpr(g_c_api_core_type == C_API_AIC_TYPE)
    #define ASC_IS_NOT_AIV constexpr(g_c_api_core_type != C_API_AIV_TYPE)
    #define ASC_IS_NOT_AIC constexpr(g_c_api_core_type != C_API_AIC_TYPE)
#endif

union asc_sparse_addr_config {
    uint64_t config;
    struct {
        uint64_t src : 32;
        uint64_t index : 32;
    };
};

__aicore__ inline void asc_set_mask_count_begin(uint32_t count)
{
    if ASC_IS_AIV {
        set_mask_count();
        set_vector_mask(static_cast<uint64_t>(0), static_cast<uint64_t>(count));
    }
}

__aicore__ inline void asc_set_mask_count_end()
{
    if ASC_IS_AIV {
        set_mask_norm();
    }
}

__aicore__ inline void asc_sync_post_process()
{
    pipe_barrier(pipe_t::PIPE_ALL);
}

union asc_capi_arch_version {
    uint64_t full;
    struct {
        uint64_t low_bit : 32;
        uint64_t core_ver : 12;
        uint64_t reserved : 20;
    };
};

#endif // IMPL_C_API_C_API_INSTR_IMPL_UTILS_C_API_IMPL_UTILS_C_API_IMPL_H
