/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning                                                                                                               \
    "impl/tensor_api/arch/utils/arch_utils.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file arch_utils.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_UTILS_ARCH_UTILS_H
#define IMPL_TENSOR_API_ARCH_UTILS_ARCH_UTILS_H

#include "impl/tensor_api/arch/utils/check_data_type.h"
#include "impl/tensor_api/arch/utils/check_format.h"
#include "impl/tensor_api/arch/utils/is_format.h"

namespace asc {
namespace te {

#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
#define SCALAR_QUANT_MODE QuantMode_t::DEQF16, QuantMode_t::QF322B8_PRE, QuantMode_t::REQ8,\
    QuantMode_t::QS322BF16_PRE, QuantMode_t::QF322F16_PRE, QuantMode_t::QF322BF16_PRE, QuantMode_t::QF322FP8_PRE,\
    QuantMode_t::QF322HIF8_PRE, QuantMode_t::QF322HIF8_PRE_HYBRID, QuantMode_t::QF322F32_PRE
#elif defined(__NPU_ARCH__) && __NPU_ARCH__ == 2201
#define SCALAR_QUANT_MODE QuantMode_t::DEQF16, QuantMode_t::QF322B8_PRE, QuantMode_t::REQ8
#else
#define SCALAR_QUANT_MODE QuantMode_t::NoQuant
#endif

template <QuantMode_t quant_pre>
using is_scalar_quant_mode = Std::is_one_of_value<QuantMode_t, quant_pre, SCALAR_QUANT_MODE>;

#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
#define TILE_OP_INTERNAL_TENSOR_QUANT_MODE QuantMode_t::VDEQF16, QuantMode_t::VQF322B8_PRE, QuantMode_t::VREQ8,\
    QuantMode_t::VQS322BF16_PRE, QuantMode_t::VQF322F16_PRE, QuantMode_t::VQF322BF16_PRE, QuantMode_t::VQF322FP8_PRE,\
    QuantMode_t::VQF322HIF8_PRE, QuantMode_t::VQF322HIF8_PRE_HYBRID, QuantMode_t::VQF322F32_PRE
#elif defined(__NPU_ARCH__) && __NPU_ARCH__ == 2201
#define TILE_OP_INTERNAL_TENSOR_QUANT_MODE QuantMode_t::VDEQF16, QuantMode_t::VQF322B8_PRE, QuantMode_t::VREQ8
#else
#define TILE_OP_INTERNAL_TENSOR_QUANT_MODE QuantMode_t::NoQuant
#endif

template <QuantMode_t quant_pre>
using is_vector_quant_mode = Std::is_one_of_value<QuantMode_t, quant_pre, TILE_OP_INTERNAL_TENSOR_QUANT_MODE>;

#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
#define TILE_OP_INTERNAL_DIRECT_QUANT_MODE QuantMode_t::F322F16, QuantMode_t::F322BF16
#elif defined(__NPU_ARCH__) && __NPU_ARCH__ == 2201
#define TILE_OP_INTERNAL_DIRECT_QUANT_MODE QuantMode_t::F322F16, QuantMode_t::F322BF16
#else
#define TILE_OP_INTERNAL_DIRECT_QUANT_MODE QuantMode_t::NoQuant
#endif

template <QuantMode_t quant_pre>
using is_direct_quant_mode = Std::is_one_of_value<QuantMode_t, quant_pre, TILE_OP_INTERNAL_DIRECT_QUANT_MODE>;

template <typename info1, typename info2, size_t dim, typename T>
__aicore__ inline constexpr decltype(auto) get_element(const T& layout) {
    constexpr size_t shape_or_stride = (Std::is_same_v<info1, attr_info::shape> ? 0 : 1);
    constexpr size_t row_or_column = (Std::is_same_v<info2, attr_info::row> ? 0 : 1);
    return layout.template get<shape_or_stride, row_or_column, dim>();
}

template <typename info1, typename info2, typename T>
__aicore__ inline constexpr decltype(auto) get_element(const T& layout) {
    constexpr size_t shape_or_stride = (Std::is_same_v<info1, attr_info::shape> ? 0 : 1);
    constexpr size_t row_or_column = (Std::is_same_v<info2, attr_info::row> ? 0 : 1);
    return layout.template get<shape_or_stride, row_or_column>();
}

template <typename info1, typename info2, size_t dim, typename T>
__aicore__ inline constexpr decltype(auto) get_matrix_element(const T& layout)
{
    constexpr size_t shape_or_stride = Std::is_same_v<info1, attr_info::shape> ? 0 : 1;
    constexpr size_t row_or_column = Std::is_same_v<info2, attr_info::row> ? 0 : 1;
    if constexpr (T::depth == THREE_DIM_DATA || T::depth == FIVE_DIM_DATA) {
        return layout.template get<shape_or_stride, 1, row_or_column, dim>();
    } else {
        return layout.template get<shape_or_stride, row_or_column, dim>();
    }
}

template <typename layout_type>
__aicore__ inline static constexpr uint32_t get_total_column_shape(const layout_type& layout)
{
    if constexpr (layout_type::depth == 2) {
        return get<1>(layout.shape());
    } else {
        return get<1, 0>(layout.shape()) * get<1, 1>(layout.shape());
    }
}

template <typename layout_type>
__aicore__ inline static constexpr uint32_t get_total_row_shape(const layout_type& layout)
{
    if constexpr (layout_type::depth == 2) {
        return get<0>(layout.shape());
    } else {
        return get<0, 0>(layout.shape()) * get<0, 1>(layout.shape());
    }
}

template <typename layout_type>
__aicore__ inline static constexpr decltype(auto) get_row_stride(const layout_type& layout)
{
    if constexpr (layout_type::depth == 2) {
        return get_element<attr_info::stride, attr_info::row>(layout);
    } else {
        return get_element<attr_info::stride, attr_info::row, 1>(layout);
    }
}

template <typename layout_type>
__aicore__ inline static constexpr decltype(auto) get_column_stride(const layout_type& layout)
{
    if constexpr (layout_type::depth == 2) {
        return get_element<attr_info::stride, attr_info::column>(layout);
    } else {
        return get_element<attr_info::stride, attr_info::column, 1>(layout);
    }
}

template <typename shape_type>
__aicore__ inline static constexpr uint32_t get_total_column_shape_from_shape(const shape_type& shape)
{
    constexpr auto depth = nesting_depth_v<shape_type>;
    if constexpr (depth == TWO_DIM_DATA) {
        return get<1>(shape);
    } else if constexpr (depth == THREE_DIM_DATA) {
        return get<1, 1>(shape);
    } else if constexpr (depth == FOUR_DIM_DATA) {
        return get<1, 0>(shape) * get<1, 1>(shape);
    } else {
        static_assert(depth == FIVE_DIM_DATA, "Only support 2D to 5D matrix shapes");
        return get<1, 1, 0>(shape) * get<1, 1, 1>(shape);
    }
}

template <typename shape_type>
__aicore__ inline static constexpr uint32_t get_total_row_shape_from_shape(const shape_type& shape)
{
    constexpr auto depth = nesting_depth_v<shape_type>;
    if constexpr (depth == TWO_DIM_DATA) {
        return get<0>(shape);
    } else if constexpr (depth == THREE_DIM_DATA) {
        return get<1, 0>(shape);
    } else if constexpr (depth == FOUR_DIM_DATA) {
        return get<0, 0>(shape) * get<0, 1>(shape);
    } else {
        static_assert(depth == FIVE_DIM_DATA, "Only support 2D to 5D matrix shapes");
        return get<1, 0, 0>(shape) * get<1, 0, 1>(shape);
    }
}

template <typename shape_type>
__aicore__ inline static constexpr uint32_t get_batch_shape_from_shape(const shape_type& shape)
{
    constexpr auto depth = nesting_depth_v<shape_type>;
    if constexpr (depth == THREE_DIM_DATA || depth == FIVE_DIM_DATA) {
        return get<0>(shape);
    } else {
        return 1;
    }
}
} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_UTILS_ARCH_UTILS_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
