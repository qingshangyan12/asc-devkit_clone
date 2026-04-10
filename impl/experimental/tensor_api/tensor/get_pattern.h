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
    "impl/tensor_api/tensor/get_pattern.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file get_pattern.h
* \brief
*/
#ifndef IMPL_TENSOR_API_TENSOR_GET_PATTERN_H
#define IMPL_TENSOR_API_TENSOR_GET_PATTERN_H

#include "impl/experimental/tensor_api/tensor/layout_definition.h"
#include "impl/experimental/tensor_api/tensor/local_tensor_impl.h"

namespace AscendC {
namespace Te {

template <typename T>
struct dependent_false : Std::false_type {};

template <typename Info, typename = void>
struct LayoutInfoTrait {
    static_assert(dependent_false<Info>::value,
        "Layout::info must be Std::tuple<LayoutPattern, TraitType>.");
};

template <typename LayoutPattern, typename TraitType>
struct LayoutInfoTrait<Std::tuple<LayoutPattern, TraitType>> {
    using pattern = LayoutPattern;
    using trait = TraitType;
};

template <typename T, typename = void>
struct PatternTrait;

template <typename T>
struct PatternTrait<T, Std::enable_if_t<is_layout_v<T>>> {
    using Info = typename Std::remove_cvref_t<T>::info;
    using type = typename LayoutInfoTrait<Info>::pattern;
};

template <typename T>
struct PatternTrait<T, Std::enable_if_t<IsTileTensorV<T>>> {
    using type = typename PatternTrait<decltype(Std::declval<const T&>().Layout())>::type;
};

template <typename T>
using GetLayoutPattern = typename PatternTrait<T>::type;

template <typename T, typename = void>
struct TraitTrait;

template <typename T>
struct TraitTrait<T, Std::enable_if_t<is_layout_v<T>>> {
    using Info = typename Std::remove_cvref_t<T>::info;
    using type = typename LayoutInfoTrait<Info>::trait;
};

template <typename T>
struct TraitTrait<T, Std::enable_if_t<IsTileTensorV<T>>> {
    using type = typename TraitTrait<decltype(Std::declval<const T&>().Layout())>::type;
};

template <typename T>
using GetLayoutTrait = typename TraitTrait<T>::type;

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_TENSOR_GET_PATTERN_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
