/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_MEMORY_BASE_IMPL_SPR_INTF_IMPL_H
#define IMPL_C_API_MEMORY_BASE_IMPL_SPR_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/memory_base_impl/spr_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_SPR_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/memory_base_impl/utils_intf_impl.h"

__aicore__ inline int64_t asc_get_ctrl_impl() { return get_ctrl(); }

__aicore__ inline void asc_set_ctrl_impl(uint64_t config) { set_ctrl(config); }

__aicore__ inline int64_t asc_get_ctrl() { return asc_get_ctrl_impl(); }

__aicore__ inline void asc_set_ctrl(uint64_t config) { asc_set_ctrl_impl(config); }

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_SPR_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_SPR_INTF_IMPL_H
#endif

#endif // IMPL_C_API_MEMORY_BASE_IMPL_SPR_INTF_IMPL_H
