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
    "impl/tensor_api/tensor/layout_pattern.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file layout_pattern.h
* \brief
*/
#ifndef IMPL_TENSOR_API_TENSOR_LAYOUT_PATTERN_H
#define IMPL_TENSOR_API_TENSOR_LAYOUT_PATTERN_H

#include "impl/tensor_api/tensor/layout_method.h"

namespace AscendC {
namespace Te {

struct ZNLayoutPtn {};
struct ZZLayoutPtn {};
struct NNLayoutPtn {};
struct NZLayoutPtn {};
struct NDLayoutPtn {};
struct DNLayoutPtn {};
struct ScaleANDLayoutPtn {};
struct ScaleADNLayoutPtn {};
struct ScaleBNDLayoutPtn {};
struct ScaleBDNLayoutPtn {};
struct DefaultPtn {};

template <typename T = uint16_t, size_t C0 = 32 / sizeof(T)>
struct LayoutTraitDefault {
    using type = T;
    static constexpr auto C0_ELEMENT = Std::Int<C0>{};
};

template <typename T = fp8_e8m0_t, size_t C0 = 2 / sizeof(T)>
struct LayoutTraitScale {
    using type = T;
    static constexpr auto C0_ELEMENT = Std::Int<C0>{};
};

template <typename T = fp4x2_e2m1_t, size_t C0 = 64 / sizeof(T)>
struct LayoutTraitFP4 {
    using type = T;
    static constexpr auto C0_ELEMENT = Std::Int<C0>{};
};

struct MakeNzFrameLayout {
    template <typename TraitType, typename T, typename U>
    __aicore__ inline static auto Make(T row, U column) {
        constexpr auto c0Ele = TraitType::C0_ELEMENT;
        auto shape = MakeShape(MakeShape(Std::Int<FRACTAL_FIXED>{}, Std::ceil_division(row, FRACTAL_FIXED)),
                               MakeShape(c0Ele, Std::ceil_division(column, c0Ele)));
        auto stride = MakeStride(MakeStride(c0Ele, c0Ele * Std::Int<FRACTAL_FIXED>{}),
                                 MakeStride(Std::Int<1>{}, c0Ele * Std::ceil_align(row, FRACTAL_FIXED)));
        return MakePatternLayout<NZLayoutPtn, TraitType>(shape, stride);
    }
};

struct MakeNDFrameLayout {
    template <typename TraitType, typename T, typename U>
    __aicore__ inline static auto Make(T row, U column) {
        auto shape = MakeShape(MakeShape(Std::Int<1>{}, row), MakeShape(Std::Int<1>{}, column));
        auto stride = MakeStride(MakeStride(Std::Int<0>{}, column), MakeStride(Std::Int<0>{}, Std::Int<1>{}));
        return MakePatternLayout<NDLayoutPtn, TraitType>(shape, stride);
    }
};

struct MakeZnFrameLayout {
    template <typename TraitType, typename T, typename U>
    __aicore__ inline static auto Make(T row, U column) {
        constexpr auto c0Ele = TraitType::C0_ELEMENT;
        auto shape = MakeShape(MakeShape(c0Ele, Std::ceil_division(row, c0Ele)),
                               MakeShape(Std::Int<FRACTAL_FIXED>{}, Std::ceil_division(column, FRACTAL_FIXED)));
        auto stride = MakeStride(MakeStride(Std::Int<1>{}, c0Ele * Std::ceil_align(column, FRACTAL_FIXED)),
                                 MakeStride(c0Ele, c0Ele * Std::Int<FRACTAL_FIXED>{}));
        return MakePatternLayout<ZNLayoutPtn, TraitType>(shape, stride);
    }
};

struct MakeDNFrameLayout {
    template <typename TraitType, typename T, typename U>
    __aicore__ inline static auto Make(T row, U column) {
        auto shape = MakeShape(MakeShape(Std::Int<1>{}, row), MakeShape(Std::Int<1>{}, column));
        auto stride = MakeStride(MakeStride(Std::Int<0>{}, Std::Int<1>{}), MakeStride(Std::Int<0>{}, row));
        return MakePatternLayout<DNLayoutPtn, TraitType>(shape, stride);
    }
};

struct MakeZzFrameLayout {
    template <typename TraitType, typename T, typename U>
    __aicore__ inline static auto Make(T row, U column) {
        constexpr auto c0Ele = TraitType::C0_ELEMENT;
        auto shape = MakeShape(MakeShape(Std::Int<FRACTAL_FIXED>{}, Std::ceil_division(row, FRACTAL_FIXED)),
                               MakeShape(c0Ele, Std::ceil_division(column, c0Ele)));
        auto stride = MakeStride(MakeStride(c0Ele, FRACTAL_FIXED * Std::ceil_align(column, c0Ele)),
                                 MakeStride(Std::Int<1>{}, c0Ele * Std::Int<FRACTAL_FIXED>{}));
        return MakePatternLayout<ZZLayoutPtn, TraitType>(shape, stride);
    }
};

struct MakeNnFrameLayout {
    template <typename TraitType, typename T, typename U>
    __aicore__ inline static auto Make(T row, U column) {
        constexpr auto c0Ele = TraitType::C0_ELEMENT;
        static_assert(c0Ele == 2, "NnLayoutPtn only supports fp8_e8m0_t and ShapeColumn0 as 2.");
        auto shape = MakeShape(MakeShape(c0Ele, row / c0Ele), MakeShape(Std::Int<FRACTAL_FIXED>{}, Std::ceil_division(column, FRACTAL_FIXED)));
        auto stride = MakeStride(MakeStride(Std::Int<1>{}, c0Ele * Std::Int<FRACTAL_FIXED>{}),
                                 MakeStride(c0Ele, row * FRACTAL_FIXED));
        return MakePatternLayout<NNLayoutPtn, TraitType>(shape, stride);
    }
};

struct MakeScaleANDFrameLayout {
    template <typename TraitType, typename T, typename U>
    __aicore__ inline static auto Make(T row, U column) {
        constexpr auto c0Ele = TraitType::C0_ELEMENT;
        static_assert(c0Ele == 2, "ScaleANDLayoutPtn only supports fp8_e8m0_t and ShapeColumn0 as 2.");
        auto shape = MakeShape(MakeShape(Std::Int<1>{}, row), MakeShape(Std::Int<1>{}, column));
        auto stride = MakeStride(MakeStride(Std::Int<0>{}, column), MakeStride(Std::Int<0>{}, Std::Int<1>{}));
        return MakePatternLayout<ScaleANDLayoutPtn, TraitType>(shape, stride);
    }
};

struct MakeScaleADNFrameLayout {
    template <typename TraitType, typename T, typename U>
    __aicore__ inline static auto Make(T row, U column) {
        constexpr auto c0Ele = TraitType::C0_ELEMENT;
        static_assert(c0Ele == 2, "ScaleADNLayoutPtn only supports fp8_e8m0_t and ShapeColumn0 as 2.");
        auto shape = MakeShape(MakeShape(Std::Int<1>{}, row), MakeShape(c0Ele, column / c0Ele));
        auto stride = MakeStride(MakeStride(Std::Int<0>{}, c0Ele),
                                 MakeStride(Std::Int<1>{}, c0Ele * row));
        return MakePatternLayout<ScaleADNLayoutPtn, TraitType>(shape, stride);
    }
};

struct MakeScaleBNDFrameLayout {
    template <typename TraitType, typename T, typename U>
    __aicore__ inline static auto Make(T row, U column) {
        constexpr auto c0Ele = TraitType::C0_ELEMENT;
        static_assert(c0Ele == 2, "ScaleBNDLayoutPtn only supports fp8_e8m0_t and ShapeColumn0 as 2.");
        auto shape = MakeShape(MakeShape(c0Ele, row / c0Ele), MakeShape(Std::Int<1>{}, column));
        auto stride = MakeStride(MakeStride(Std::Int<1>{}, c0Ele * column),
                                 MakeStride(Std::Int<0>{}, c0Ele));
        return MakePatternLayout<ScaleBNDLayoutPtn, TraitType>(shape, stride);
    }
};

struct MakeScaleBDNFrameLayout {
    template <typename TraitType, typename T, typename U>
    __aicore__ inline static auto Make(T row, U column) {
        auto shape = MakeShape(MakeShape(Std::Int<1>{}, row), MakeShape(Std::Int<1>{}, column));
        auto stride = MakeStride(MakeStride(Std::Int<0>{}, Std::Int<1>{}), MakeStride(Std::Int<0>{}, row));
        return MakePatternLayout<ScaleBDNLayoutPtn, TraitType>(shape, stride);
    }
};


} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_TENSOR_LAYOUT_PATTERN_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
