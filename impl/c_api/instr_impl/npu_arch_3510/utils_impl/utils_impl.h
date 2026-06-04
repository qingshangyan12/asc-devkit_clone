/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_UTILS_IMPL_UTILS_C_API_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_UTILS_IMPL_UTILS_C_API_IMPL_H

#include "instr_impl/npu_arch_2201/utils_impl/utils_impl.h"

union asc_gm2l1_loop_size_config {
    uint64_t config;
    struct {
        uint64_t loop1_size : 21;
        uint64_t loop2_size : 42;
    };
};

union asc_gm2l1_loop_stride_config {
    uint64_t config;
    struct {
        uint64_t src_stride : 40;
        uint64_t dst_stride : 24;
    };
};

union asc_gm2ub_loop_size_config {
    uint64_t config;
    struct {
        uint64_t loop1_size : 21;
        uint64_t loop2_size : 42;
    };
};

union asc_gm2ub_loop_stride_config {
    uint64_t config;
    struct {
        uint64_t src_stride : 40;
        uint64_t dst_stride : 24;
    };
};

enum class asc_position {
    EVEN = 0,
    ODD
};

constexpr std::integral_constant<asc_position, asc_position::EVEN> ASC_POSITION_EVEN;
constexpr std::integral_constant<asc_position, asc_position::ODD> ASC_POSITION_ODD;

enum class asc_disperse_quarter {
    FIRST = 0,
    SECOND,
    THIRD,
    FOURTH
};

constexpr std::integral_constant<asc_disperse_quarter, asc_disperse_quarter::FIRST> ASC_DISPERSE_FIRST_QUARTER;
constexpr std::integral_constant<asc_disperse_quarter, asc_disperse_quarter::SECOND> ASC_DISPERSE_SECOND_QUARTER;
constexpr std::integral_constant<asc_disperse_quarter, asc_disperse_quarter::THIRD> ASC_DISPERSE_THIRD_QUARTER;
constexpr std::integral_constant<asc_disperse_quarter, asc_disperse_quarter::FOURTH> ASC_DISPERSE_FOURTH_QUARTER;

#endif
