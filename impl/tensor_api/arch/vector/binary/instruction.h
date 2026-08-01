/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning                                                                                                               \
    "impl/tensor_api/arch/vector/binary/instruction.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file instruction.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_BINARY_INSTRUCTION_H
#define IMPL_TENSOR_API_ARCH_VECTOR_BINARY_INSTRUCTION_H

#include "impl/tensor_api/tensor/pointer_pattern.h"
#include "impl/tensor_api/tensor/tensor_impl.h"
#include "impl/tensor_api/arch/utils/arch_utils.h"

namespace asc {
namespace te {
namespace inst {

class add {
public:
    template <typename T>
    __simd_callee__ inline static void run(T& dst, T src0, T src1, vector_bool mask)
    {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_add(dst, src0, src1, mask);
        }
    }
};

class madd {
public:
    template <typename T>
    __simd_callee__ inline static void run(T& dst, T src0, T src1, vector_bool mask)
    {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_madd(dst, src0, src1, mask);
        }
    }
};

class sub {
public:
    template <typename T>
    __simd_callee__ inline static void run(T& dst, T src0, T src1, vector_bool mask)
    {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_sub(dst, src0, src1, mask);
        }
    }
};

class bitwise_and {
public:
    template <typename T>
    __simd_callee__ inline static void run(T& dst, T src0, T src1, vector_bool mask)
    {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_and(dst, src0, src1, mask);
        }
    }
};

class select {
public:
    template <typename T>
    __simd_callee__ inline static void run(T& dst, T src0, T src1, vector_bool mask)
    {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_select(dst, src0, src1, mask);
        }
    }
};

class abs_sub {
public:
    template <typename T>
    __simd_callee__ inline static void run(T& dst, T src0, T src1, vector_bool mask)
    {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_abs_sub(dst, src0, src1, mask);
        }
    }
};

class min {
public:
    template <typename T>
    __simd_callee__ inline static void run(T& dst, T src0, T src1, vector_bool mask)
    {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_min(dst, src0, src1, mask);
        }
    }
};

class max {
public:
    template <typename T>
    __simd_callee__ inline static void run(T& dst, T src0, T src1, vector_bool mask)
    {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_max(dst, src0, src1, mask);
        }
    }
};

class bitwise_or {
public:
    template <typename T>
    __simd_callee__ inline static void run(T& dst, T src0, T src1, vector_bool mask)
    {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_or(dst, src0, src1, mask);
        }
    }
};

class mul {
public:
    template <typename T>
    __simd_callee__ inline static void run(T& dst, T src0, T src1, vector_bool mask)
    {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_mul(dst, src0, src1, mask);
        }
    }
};

class exp_sub_even {
public:
    template <typename T>
    __simd_callee__ inline static void run(T& dst, T src0, T src1, vector_bool mask)
    {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_exp_sub(dst, src0, src1, mask);
        }
    }
};

class exp_sub_odd {
public:
    template <typename T>
    __simd_callee__ inline static void run(T& dst, T src0, T src1, vector_bool mask)
    {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_exp_sub_v2(dst, src0, src1, mask);
        }
    }
};

class shift_left {
public:
    template <typename T>
    __simd_callee__ inline static void run(T& dst, T src0, T src1, vector_bool mask)
    {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_shiftleft(dst, src0, src1, mask);
        }
    }
};

class shift_right {
public:
    template <typename T>
    __simd_callee__ inline static void run(T& dst, T src0, T src1, vector_bool mask)
    {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_shiftright(dst, src0, src1, mask);
        }
    }
};

class div {
public:
    template <typename T>
    __simd_callee__ inline static void run(T& dst, T src0, T src1, vector_bool mask)
    {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_div(dst, src0, src1, mask);
        }
    }
};

class bitwise_xor {
public:
    template <typename T>
    __simd_callee__ inline static void run(T& dst, T src0, T src1, vector_bool mask)
    {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_xor(dst, src0, src1, mask);
        }
    }
};

class prelu {
public:
    template <typename T>
    __simd_callee__ inline static void run(T& dst, T src0, T src1, vector_bool mask)
    {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_prelu(dst, src0, src1, mask);
        }
    }
};

using Add = add;
using Madd = madd;
using Sub = sub;
using And = bitwise_and;
using Select = select;
using AbsSub = abs_sub;
using Min = min;
using Max = max;
using Or = bitwise_or;
using Mul = mul;
using ExpSubEven = exp_sub_even;
using ExpSubOdd = exp_sub_odd;
using ShiftLeft = shift_left;
using ShiftRight = shift_right;
using Div = div;
using Xor = bitwise_xor;
using Prelu = prelu;

} // namespace inst
} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_VECTOR_BINARY_INSTRUCTION_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
