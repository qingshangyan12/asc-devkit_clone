/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_REG_ADDR_REG_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_REG_ADDR_REG_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/reg_base_impl/npu_arch_3510/reg_addr_reg_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_REG_ADDR_REG_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/reg_base_impl/utils_impl.h"
#include "impl/c_api/reg_base_impl/reg_addr_reg_intf_impl.h"

// ==========asc_create_iter_reg (deprecated, please use asc_update_addr_reg instead)=========
#define ASC_DEFINE_ITER_REG_DEPRECATED_API(bxx)                                                                      \
    [[deprecated("NOTICE: asc_create_iter_reg_" #bxx " is deprecated. "                                              \
                 "Please use asc_update_addr_reg_" #bxx                                                              \
                 " instead.")]] __simd_callee__ inline addr_reg asc_create_iter_reg_##bxx(uint32_t offset)           \
    {                                                                                                                \
        return asc_update_addr_reg_##bxx(offset);                                                                    \
    }                                                                                                                \
    [[deprecated(                                                                                                    \
        "NOTICE: asc_create_iter_reg_" #bxx " is deprecated. "                                                       \
        "Please use asc_update_addr_reg_" #bxx                                                                       \
        " instead.")]] __simd_callee__ inline addr_reg asc_create_iter_reg_##bxx(uint32_t offset0, uint32_t offset1) \
    {                                                                                                                \
        return asc_update_addr_reg_##bxx(offset0, offset1);                                                          \
    }                                                                                                                \
    [[deprecated("NOTICE: asc_create_iter_reg_" #bxx " is deprecated. "                                              \
                 "Please use asc_update_addr_reg_" #bxx " instead.")]] __simd_callee__ inline addr_reg               \
        asc_create_iter_reg_##bxx(uint32_t offset0, uint32_t offset1, uint32_t offset2)                              \
    {                                                                                                                \
        return asc_update_addr_reg_##bxx(offset0, offset1, offset2);                                                 \
    }                                                                                                                \
    [[deprecated("NOTICE: asc_create_iter_reg_" #bxx " is deprecated. "                                              \
                 "Please use asc_update_addr_reg_" #bxx " instead.")]] __simd_callee__ inline addr_reg               \
        asc_create_iter_reg_##bxx(uint32_t offset0, uint32_t offset1, uint32_t offset2, uint32_t offset3)            \
    {                                                                                                                \
        return asc_update_addr_reg_##bxx(offset0, offset1, offset2, offset3);                                        \
    }

ASC_DEFINE_ITER_REG_DEPRECATED_API(b32)
ASC_DEFINE_ITER_REG_DEPRECATED_API(b16)
ASC_DEFINE_ITER_REG_DEPRECATED_API(b8)

#undef ASC_DEFINE_ITER_REG_DEPRECATED_API

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_REG_ADDR_REG_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_REG_ADDR_REG_INTF_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_REG_ADDR_REG_INTF_IMPL_H
