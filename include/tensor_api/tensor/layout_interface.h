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
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif

/*!
 * \file layout_interface.h
 * \brief
 */
#ifndef INCLUDE_TENSOR_API_TENSOR_LAYOUT_INTERFACE_H
#define INCLUDE_TENSOR_API_TENSOR_LAYOUT_INTERFACE_H

#include "tensor_api/tensor/layout_type.h"
#include "impl/tensor_api/tensor/tuple_impl.h"
#include "impl/tensor_api/tensor/layout_definition.h"

namespace asc {
namespace te {

template <typename Layout>
using get_layout_pattern = typename get_pattern<Std::remove_cvref_t<Layout>>::type;

template <typename Coord, typename Shape, typename Stride>
/**
 * @ingroup simd_api.tensor_api.layout
 * @override_id crd2idx__RCpCoord_RCpShape_RCpStride
 * @archs 3510
 * @brief crd2idx根据coord、shape和stride计算坐标对应的一维线性索引。输入为嵌套Tuple时，接口会按照相同的层次结构递归计算各维度的偏移并求和。
 * @param [in] coord 待转换的坐标。
 * @param [in] shape 描述各维度大小的shape。
 * @param [in] stride 描述各维度步长的stride。
 */
__aicore__ inline constexpr auto crd2idx(const Coord& coord, const Shape& shape, const Stride& stride);

template <size_t I, size_t... Is, typename Tuple, typename Enable>
/**
 * @ingroup simd_api.tensor_api.layout
 * @override_id get__OpTuple
 * @archs 3510
 * @brief get提供两类能力：。
 */
__aicore__ inline constexpr auto get(Tuple&& t);

// layout construction
template <typename FirstShape, typename... Shapes>
/**
 * @ingroup simd_api.tensor_api.layout
 * @override_id make_shape__RCpFirstShape_QRCpShapes
 * @archs 3510
 * @brief 构造shape对象，用于描述数据的逻辑形状。支持传入多个维度值，也支持传入嵌套的shape子结构以构造层次化shape。
 * @param [in] first_shape 第一个维度取值或子结构。
 */
__aicore__ inline constexpr shape<FirstShape, Shapes...> make_shape(
    const FirstShape& first_shape, const Shapes&... shapes);

template <typename FirstStride, typename... Strides>
/**
 * @ingroup simd_api.tensor_api.layout
 * @override_id make_stride__RCpFirstStride_QRCpStrides
 * @archs 3510
 * @brief 构造stride对象，用于描述各维度在内存中的步长。支持传入多个步长值，也支持传入嵌套的stride子结构以构造层次化stride。
 * @param [in] first_stride 第一个维度的步长或子结构。
 */
__aicore__ inline constexpr stride<FirstStride, Strides...> make_stride(
    const FirstStride& first_stride, const Strides&... strides);

template <typename FirstCoord, typename... Coords>
/**
 * @ingroup simd_api.tensor_api.layout
 * @override_id make_coord__RCpFirstCoord_QRCpCoords
 * @archs 3510
 * @brief 构造coord对象，用于定义张量的坐标。
 * @param [in] first_coord 第一个维度的坐标。
 */
__aicore__ inline constexpr coord<FirstCoord, Coords...> make_coord(
    const FirstCoord& first_coord, const Coords&... coords);

template <typename Shape, typename Stride>
/**
 * @ingroup simd_api.tensor_api.layout
 * @override_id make_layout__RCpShape_RCpStride
 * @archs 3510
 * @brief make_layout用于根据shape和stride构造layout对象，描述数据张量在逻辑维度上的形状以及各维对应的步长信息。
 */
__aicore__ inline constexpr auto make_layout(const Shape& shape, const Stride& stride);

template <typename Shape>
/**
 * @ingroup simd_api.tensor_api.layout
 * @override_id make_layout__RCpShape
 * @archs 3510
 * @brief make_layout用于根据shape和stride构造layout对象，描述数据张量在逻辑维度上的形状以及各维对应的步长信息。
 */
__aicore__ inline constexpr auto make_layout(const Shape& shape);

template <size_t... Is, typename Layout, typename Enable = Std::enable_if_t<is_layout_v<Layout>>>
/**
 * @ingroup simd_api.tensor_api.layout
 * @override_id get_shape__RCpLayout
 * @archs 3510
 * @brief `get_shape`用于从layout中提取shape部分：。
 * @param [in] layout layout对象。
 */
__aicore__ inline constexpr auto get_shape(const Layout& layout);

template <size_t... Is, typename Layout, typename Enable = Std::enable_if_t<is_layout_v<Layout>>>
/**
 * @ingroup simd_api.tensor_api.layout
 * @override_id get_stride__RCpLayout
 * @archs 3510
 * @brief get_stride用于从layout中提取stride部分：。
 * @param [in] layout layout对象。
 */
__aicore__ inline constexpr auto get_stride(const Layout& layout);

template <size_t... Is, typename Layout, typename Enable = Std::enable_if_t<is_layout_v<Layout>>>
/**
 * @ingroup simd_api.tensor_api.layout
 * @override_id coshape__RCpLayout
 * @archs 3510
 * @brief coshape表示layout陪域（codomain）区间的标量大小，即逻辑坐标映射到一维索引后，所有可能的索引值构成的区间大小。
 */
__aicore__ inline constexpr auto coshape(const Layout& layout);

template <typename LayoutPattern, typename Trait, typename Shape, typename Stride>
/**
 * @ingroup simd_api.tensor_api.layout
 * @override_id make_pattern_layout__RCpShape_RCpStride
 * @archs 3510
 * @brief make_pattern_layout用于根据shape、stride、LayoutPattern和Trait构造带布局模式信息的layout对象。
 */
__aicore__ inline constexpr auto make_pattern_layout(const Shape& shape, const Stride& stride);

template <size_t... Is, typename Layout, typename Enable = Std::enable_if_t<is_layout_v<Layout>>>
/**
 * @ingroup simd_api.tensor_api.layout
 * @override_id cosize__RCpLayout
 * @archs 3510
 * @brief cosize表示layout陪域（codomain）的跨度大小，即逻辑坐标经layout映射为一维索引后，所有可能索引值所处区间的长度。cosize和coshape在扁平与嵌套layout场景下等价。
 */
__aicore__ inline constexpr auto cosize(const Layout& layout);

template <size_t... Is, typename Layout, typename Enable = Std::enable_if_t<is_layout_v<Layout>>>
/**
 * @ingroup simd_api.tensor_api.layout
 * @override_id rank__RCpLayout
 * @archs 3510
 * @brief 用于返回layout的秩。
 */
__aicore__ inline constexpr auto rank(const Layout& layout);

template <size_t... Is, typename Layout, typename Enable = Std::enable_if_t<is_layout_v<Layout>>>
/**
 * @ingroup simd_api.tensor_api.layout
 * @override_id select__RCpLayout
 * @archs 3510
 * @brief 选择layout的shape和stride指定维度组成新的layout对象并返回。
 */
__aicore__ inline constexpr auto select(const Layout& layout);

template <size_t... Is, typename Layout, typename Enable = Std::enable_if_t<is_layout_v<Layout>>>
/**
 * @ingroup simd_api.tensor_api.layout
 * @override_id get__RCpLayout
 * @archs 3510
 * @brief get提供两类能力：。
 */
__aicore__ inline constexpr auto get(const Layout& layout);

template <size_t... Is, typename Layout, typename Enable = Std::enable_if_t<is_layout_v<Layout>>>
/**
 * @ingroup simd_api.tensor_api.layout
 * @override_id size__RCpLayout
 * @archs 3510
 * @brief 用于返回layout的有效元素个数。
 */
__aicore__ inline constexpr auto size(const Layout& layout);

template <size_t... Is, typename Layout, typename Enable = Std::enable_if_t<is_layout_v<Layout>>>
/**
 * @ingroup simd_api.tensor_api.layout
 * @override_id capacity__RCpLayout
 * @archs 3510
 * @brief capacity用于计算layout所需的实际内存容量。
 */
__aicore__ inline constexpr auto capacity(const Layout& layout);

template <typename Tensor, typename Coord, typename Info>
/**
 * @ingroup simd_api.tensor_api.tensor
 * @override_id slice__OpTensor_RCpCoord_RCpInfo
 * @archs 3510
 * @brief slice用于从张量指定坐标处切出一个子张量。根据输入的起始坐标coord和切片描述信息info，计算新的起始存储位置，并构造对应的子张量，子张量保持与原张量相同的存储位置类型。
 */
__aicore__ inline constexpr decltype(auto) slice(Tensor&& tensor, const Coord& coord, const Info& info);

template <typename LayoutPattern, typename Trait = Std::ignore_t, typename... Args>
__aicore__ inline constexpr decltype(auto) make_frame_layout(const Args&... args);

template <typename LayoutPattern, size_t C0Element, typename... Args>
__aicore__ inline constexpr decltype(auto) make_frame_layout(const Args&... args);

template <typename LayoutPattern, typename Trait = layout_trait_default<>>
struct frame_layout_format {
    template <typename... Args>
    __aicore__ inline constexpr decltype(auto) operator()(const Args&... args);
};

} // namespace te
} // namespace asc

#include "impl/tensor_api/tensor/layout_impl.h"

#endif // INCLUDE_TENSOR_API_TENSOR_LAYOUT_INTERFACE_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif
