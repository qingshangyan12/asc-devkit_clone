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
    "impl/tensor_api/arch/vector/ternary/instruction.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file instruction.h
* \brief
*/
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_TERNARY_INSTRUCTION_H
#define IMPL_TENSOR_API_ARCH_VECTOR_TERNARY_INSTRUCTION_H

#include "impl/tensor_api/tensor/pointer_pattern.h"
#include "impl/tensor_api/tensor/tensor_impl.h"
#include "impl/tensor_api/arch/utils/arch_utils.h"

namespace AscendC {
namespace Te {
namespace Inst {

class AddScalar {
public:
    static constexpr bool isCompare = false;
    template <typename T, typename S>
    __simd_callee__ inline static void Run(T& dst, T src, S value, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_add_scalar(dst, src, value, mask);
        }
    }
};

class MulScalar {
public:
    static constexpr bool isCompare = false;
    template <typename T, typename S>
    __simd_callee__ inline static void Run(T& dst, T src, S value, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_mul_scalar(dst, src, value, mask);
        }
    }
};

class MaxScalar {
public:
    static constexpr bool isCompare = false;
    template <typename T, typename S>
    __simd_callee__ inline static void Run(T& dst, T src, S value, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_max_scalar(dst, src, value, mask);
        }
    }
};

class MinScalar {
public:
    static constexpr bool isCompare = false;
    template <typename T, typename S>
    __simd_callee__ inline static void Run(T& dst, T src, S value, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_min_scalar(dst, src, value, mask);
        }
    }
};

class Axpy {
public:
    static constexpr bool isCompare = false;
    template <typename T, typename S>
    __simd_callee__ inline static void Run(T& dst, T src, S value, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_axpy(dst, src, value, mask);
        }
    }
};

class ShiftLeftScalar {
public:
    static constexpr bool isCompare = false;
    template <typename T>
    __simd_callee__ inline static void Run(T& dst, T src, int16_t value, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_shiftleft_scalar(dst, src, value, mask);
        }
    }
};

class ShiftRightScalar {
public:
    static constexpr bool isCompare = false;
    template <typename T>
    __simd_callee__ inline static void Run(T& dst, T src, int16_t value, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_shiftright_scalar(dst, src, value, mask);
        }
    }
};

class Muls {
public:
    static constexpr bool isCompare = false;
    template <typename DstReg, typename SrcReg, typename S>
    __simd_callee__ inline static void Run(DstReg& dst, SrcReg src, S value, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_muls(dst, src, value, mask);
        }
    }
};

class MulsV2 {
public:
    static constexpr bool isCompare = false;
    template <typename DstReg, typename SrcReg, typename S>
    __simd_callee__ inline static void Run(DstReg& dst, SrcReg src, S value, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_muls_v2(dst, src, value, mask);
        }
    }
};

class EqScalar {
public:
    static constexpr bool isCompare = true;
    template <typename T, typename S>
    __simd_callee__ inline static void Run(vector_bool& dst, T src, S value, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_eq_scalar(dst, src, value, mask);
        }
    }
};

class GeScalar {
public:
    static constexpr bool isCompare = true;
    template <typename T, typename S>
    __simd_callee__ inline static void Run(vector_bool& dst, T src, S value, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_ge_scalar(dst, src, value, mask);
        }
    }
};

class LeScalar {
public:
    static constexpr bool isCompare = true;
    template <typename T, typename S>
    __simd_callee__ inline static void Run(vector_bool& dst, T src, S value, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_le_scalar(dst, src, value, mask);
        }
    }
};

class LtScalar {
public:
    static constexpr bool isCompare = true;
    template <typename T, typename S>
    __simd_callee__ inline static void Run(vector_bool& dst, T src, S value, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_lt_scalar(dst, src, value, mask);
        }
    }
};

class NeScalar {
public:
    static constexpr bool isCompare = true;
    template <typename T, typename S>
    __simd_callee__ inline static void Run(vector_bool& dst, T src, S value, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_ne_scalar(dst, src, value, mask);
        }
    }
};

}}}

#endif // IMPL_TENSOR_API_ARCH_VECTOR_TERNARY_INSTRUCTION_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
