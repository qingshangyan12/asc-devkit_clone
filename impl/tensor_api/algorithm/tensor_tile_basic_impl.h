/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/*!
 * \file tensor_tile_basic_impl.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_TENSOR_TILE_BASIC_IMPL_H
#define IMPL_TENSOR_API_TENSOR_TILE_BASIC_IMPL_H
namespace AscendC {
template <typename... Ts>
__aicore__ inline constexpr Shape<Ts...> MakeShape(const Ts&... t)
{
    return TileInternal::MakeShapeImpl<Ts...>(t...);
}

template <typename... Ts>
__aicore__ inline constexpr Stride<Ts...> MakeStride(const Ts&... t)
{
    return TileInternal::MakeStrideImpl<Ts...>(t...);
}

template <typename T, typename U>
__aicore__ inline constexpr auto MakeLayout(const T& shape, const U& stride)
{
    return TileInternal::MakeLayoutImpl(shape, stride);
}

template <typename Iterator>
__aicore__ inline constexpr auto MakeGMmemPtr(Iterator iter) {
    return TileInternal::MakeMemPtr<Hardware::GM, Iterator>(iter);
}

template <typename Iterator>
__aicore__ inline constexpr auto MakeUBmemPtr(Iterator iter) {
    return TileInternal::MakeMemPtr<Hardware::UB, Iterator>(iter);
}

template <typename Iterator>
__aicore__ inline constexpr auto MakeL1memPtr(Iterator iter) {
    return TileInternal::MakeMemPtr<Hardware::L1, Iterator>(iter);
}

template <typename Iterator>
__aicore__ inline constexpr auto MakeL0AmemPtr(Iterator iter) {
    return TileInternal::MakeMemPtr<Hardware::L0A, Iterator>(iter);
}

template <typename Iterator>
__aicore__ inline constexpr auto MakeL0BmemPtr(Iterator iter) {
    return TileInternal::MakeMemPtr<Hardware::L0B, Iterator>(iter);
}

template <typename Iterator>
__aicore__ inline constexpr auto MakeL0CmemPtr(Iterator iter) {
    return TileInternal::MakeMemPtr<Hardware::L0C, Iterator>(iter);
}

template <typename Iterator>
__aicore__ inline constexpr auto MakeBiasmemPtr(Iterator iter) {
    return TileInternal::MakeMemPtr<Hardware::BIAS, Iterator>(iter);
}

template <typename Iterator>
__aicore__ inline constexpr auto MakeFixbufmemPtr(Iterator iter) {
    return TileInternal::MakeMemPtr<Hardware::FIXBUF, Iterator>(iter);
}

template <typename Iterator, typename... Args>
__aicore__ inline constexpr auto MakeTensor(const Iterator& iter, const Args&... args)
{
    return TileInternal::MakeTensorImpl<Iterator, Args...>(iter, args...);
}

template <typename T>
__aicore__ inline decltype(auto) MakeNZLayout(size_t row, size_t column) {
    return TileInternal::MakeLayoutForNZ<T>(row, column);
}

template <>
__aicore__ inline decltype(auto) MakeNZLayout<Std::ignore_t>(size_t row, size_t column) {
    return TileInternal::MakeLayoutForNZ<uint16_t>(row, column);
}

template <typename T>
__aicore__ inline decltype(auto) MakeRowMajorLayout(size_t row, size_t column) {
    return TileInternal::MakeLayoutForND<T>(row, column);
}

template <typename T>
__aicore__ inline decltype(auto) MakeColumnMajorLayout(size_t row, size_t column) {
    return TileInternal::MakeLayoutForDN<T>(row, column);
}

template <typename T>
__aicore__ inline decltype(auto) MakeZNLayout(size_t row, size_t column) {
    return TileInternal::MakeLayoutForZN<T>(row, column);
}

template <typename T>
__aicore__ inline decltype(auto) MakeZZLayout(size_t row, size_t column) {
    return TileInternal::MakeLayoutForZZ<T>(row, column);
}
template <typename T, size_t row, size_t column, typename Enable = void>
struct NZLayoutFormat;

template <typename T, size_t row, size_t column>
struct NZLayoutFormat<T, row, column, typename Std::enable_if<!Std::is_same_v<T, Std::ignore_t>>::type> {
    using type = Layout<TileInternal::NZShapeFormat<T, row, column>, TileInternal::NZStrideFormat<T, row, column>>;
};

template <typename T, size_t row, size_t column>
struct NZLayoutFormat<T, row, column, typename Std::enable_if<Std::is_same_v<T, Std::ignore_t>>::type> {
    using type = Layout<TileInternal::NZShapeFormat<uint16_t, row, column>, TileInternal::NZStrideFormat<uint16_t, row, column>>;
};

template <typename T, size_t row, size_t column>
using NDLayoutFormat = Layout<TileInternal::NDShapeFormat<T, row, column>, TileInternal::NDStrideFormat<T, row, column>>;

template <typename T, size_t row, size_t column>
using DNLayoutFormat = Layout<TileInternal::DNShapeFormat<T, row, column>, TileInternal::DNStrideFormat<T, row, column>>;

template <typename T, size_t row, size_t column>
using ZNLayoutFormat = Layout<TileInternal::ZNShapeFormat<T, row, column>, TileInternal::ZNStrideFormat<T, row, column>>;

template <typename T, size_t row, size_t column>
using ZZLayoutFormat = Layout<TileInternal::ZZShapeFormat<T, row, column>, TileInternal::ZZStrideFormat<T, row, column>>;

} // namespace AscendC

#endif // IMPL_TENSOR_API_TENSOR_TILE_BASIC_IMPL_H
