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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_DATA_LOAD_IMPL_H
#endif

#ifndef IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_DATA_LOAD_IMPL_H
#define IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_DATA_LOAD_IMPL_H

#include "impl/tensor_api/experimental/arch/utils/reg_utils.h"
#include "tensor_api/tensor/tensor.h"

namespace asc {
namespace te {
namespace experimental {
namespace detail {

template <load_sideband_mode sideband_mode>
struct load_from_ptr_impl;

template <>
struct load_from_ptr_impl<load_sideband_mode::direct> {
    template <typename DataType, typename Pointer>
    __simd_callee__ static inline reg_tensor<DataType> run(Pointer src)
    {
        static_assert(
            is_b4_type_v<DataType> || is_b8_type_v<DataType> || is_b16_type_v<DataType> || is_b32_type_v<DataType> ||
                is_b64_type_v<DataType>,
            "load direct supports b4, b8, b16, b32, and b64 element types only");
        reg_tensor<DataType> dst;
        asc_loadalign(dst.reg, src);
        return dst;
    }
};

template <>
struct load_from_ptr_impl<load_sideband_mode::downsample> {
    template <typename DataType, typename Pointer>
    __simd_callee__ static inline reg_tensor<DataType> run(Pointer src)
    {
        static_assert(
            is_b4_type_v<DataType> || is_b8_type_v<DataType> || is_b16_type_v<DataType>,
            "load downsample supports b4, b8, and b16 element types only");
        reg_tensor<DataType> dst;
        asc_loadalign_downsample(dst.reg, src);
        return dst;
    }
};

template <>
struct load_from_ptr_impl<load_sideband_mode::unpack> {
    template <typename DataType, typename Pointer>
    __simd_callee__ static inline reg_tensor<DataType> run(Pointer src)
    {
        static_assert(
            is_b4_type_v<DataType> || is_b8_type_v<DataType> || is_b16_type_v<DataType> || is_b32_type_v<DataType>,
            "load unpack supports b4, b8, b16, and b32 element types only");
        reg_tensor<DataType> dst;
        asc_loadalign_unpack(dst.reg, src);
        return dst;
    }
};

template <>
struct load_from_ptr_impl<load_sideband_mode::unpack4> {
    template <typename DataType, typename Pointer>
    __simd_callee__ static inline reg_tensor<DataType> run(Pointer src)
    {
        static_assert(
            is_b4_type_v<DataType> || is_b8_type_v<DataType>, "load unpack4 supports b4 and b8 element types only");
        reg_tensor<DataType> dst;
        asc_loadalign_unpack4(dst.reg, src);
        return dst;
    }
};

template <>
struct load_from_ptr_impl<load_sideband_mode::upsample> {
    template <typename DataType, typename Pointer>
    __simd_callee__ static inline reg_tensor<DataType> run(Pointer src)
    {
        static_assert(
            is_b4_type_v<DataType> || is_b8_type_v<DataType> || is_b16_type_v<DataType>,
            "load upsample supports b4, b8, and b16 element types only");
        reg_tensor<DataType> dst;
        asc_loadalign_upsample(dst.reg, src);
        return dst;
    }
};

template <>
struct load_from_ptr_impl<load_sideband_mode::deintlv> {
    template <typename DataType, typename Pointer>
    __simd_callee__ static inline reg_pair<DataType> run(Pointer src)
    {
        static_assert(
            is_b4_type_v<DataType> || is_b8_type_v<DataType> || is_b16_type_v<DataType> || is_b32_type_v<DataType>,
            "load deintlv supports b4, b8, b16, and b32 element types only");
        reg_pair<DataType> dst;
        asc_loadalign_deintlv(dst.first.reg, dst.second.reg, src);
        return dst;
    }
};

template <load_sideband_mode sideband_mode, typename DataType, typename Pointer>
__simd_callee__ inline decltype(auto) load_from_ptr(Pointer src)
{
    return load_from_ptr_impl<sideband_mode>::template run<DataType>(src);
}

template <broadcast_mode brc_mode>
struct load_broadcast_from_ptr_impl;

template <>
struct load_broadcast_from_ptr_impl<broadcast_mode::elem> {
    template <typename DataType, typename Pointer>
    __simd_callee__ static inline reg_tensor<DataType> run(Pointer src)
    {
        static_assert(
            is_b4_type_v<DataType> || is_b8_type_v<DataType> || is_b16_type_v<DataType> || is_b32_type_v<DataType>,
            "load_broadcast elem does not support this element type");
        reg_tensor<DataType> dst;
        asc_loadalign_brc_elem(dst.reg, src);
        return dst;
    }
};

template <>
struct load_broadcast_from_ptr_impl<broadcast_mode::datablock> {
    template <typename DataType, typename Pointer>
    __simd_callee__ static inline reg_tensor<DataType> run(Pointer src)
    {
        static_assert(
            is_b4_type_v<DataType> || is_b8_type_v<DataType> || is_b16_type_v<DataType> || is_b32_type_v<DataType>,
            "load_broadcast datablock does not support this element type");
        reg_tensor<DataType> dst;
        asc_loadalign_brc_datablock(dst.reg, src);
        return dst;
    }
};

template <>
struct load_broadcast_from_ptr_impl<broadcast_mode::elem2datablock> {
    template <typename DataType, typename Pointer>
    __simd_callee__ static inline reg_tensor<DataType> run(Pointer src)
    {
        static_assert(
            is_b16_type_v<DataType> || is_b32_type_v<DataType>,
            "load_broadcast elem2datablock does not support this element type");
        reg_tensor<DataType> dst;
        asc_loadalign_brc_elem2datablock(dst.reg, src);
        return dst;
    }
};

template <broadcast_mode brc_mode, typename DataType, typename Pointer>
__simd_callee__ inline reg_tensor<DataType> load_broadcast_from_ptr(Pointer src)
{
    return load_broadcast_from_ptr_impl<brc_mode>::template run<DataType>(src);
}

template <typename DstType, typename SrcDataType>
using gather_dst_t = Std::conditional_t<Std::is_same_v<DstType, void>, SrcDataType, DstType>;

template <typename SrcDataType, typename IndexType>
inline constexpr bool is_gather_ub_same_dst_support_type_v =
    (Std::is_same_v<IndexType, uint16_t> &&
        AscendC::Std::is_one_of_v<
            SrcDataType, int8_t, uint8_t, int16_t, uint16_t, half, bfloat16_t, fp8_e4m3fn_t, hifloat8_t,
            fp8_e5m2_t, fp8_e8m0_t>) ||
    (Std::is_same_v<IndexType, uint32_t> &&
        AscendC::Std::is_one_of_v<SrcDataType, int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, float>);

template <typename DstType, typename SrcDataType, typename IndexType>
inline constexpr bool is_gather_ub_support_type_v =
    (Std::is_same_v<DstType, SrcDataType> && is_gather_ub_same_dst_support_type_v<SrcDataType, IndexType>) ||
    (Std::is_same_v<IndexType, uint16_t> &&
        ((Std::is_same_v<SrcDataType, int8_t> && Std::is_same_v<DstType, int16_t>) ||
            (Std::is_same_v<SrcDataType, uint8_t> && Std::is_same_v<DstType, uint16_t>)));

template <typename DstType, typename SrcDataType, typename Pointer, typename IndexType>
__simd_callee__ inline reg_tensor<DstType> gather_from_ptr(Pointer src, const reg_tensor<IndexType>& index)
{
    static_assert(
        is_gather_ub_support_type_v<DstType, SrcDataType, IndexType>,
        "gather from UB tensor does not support this source, destination, and index type combination");

    reg_tensor<DstType> dst;
    if constexpr (is_gather_ub_support_type_v<DstType, SrcDataType, IndexType>) {
        asc_gather(dst.reg, src, index.reg, index.mask);
    }
    dst.mask = index.mask;
    return dst;
}

template <typename DataType, typename IndexType>
inline constexpr bool is_gather_reg_support_type_v =
    (Std::is_same_v<IndexType, uint8_t> &&
        AscendC::Std::is_one_of_v<
            DataType, int8_t, uint8_t, fp8_e4m3fn_t, hifloat8_t, fp8_e5m2_t, fp8_e8m0_t>) ||
    (Std::is_same_v<IndexType, uint16_t> &&
        AscendC::Std::is_one_of_v<DataType, int16_t, uint16_t, half, bfloat16_t>) ||
    (Std::is_same_v<IndexType, uint32_t> && AscendC::Std::is_one_of_v<DataType, int32_t, uint32_t>);

} // namespace detail

template <load_sideband_mode sideband_mode, typename Tensor, typename Coord>
__simd_callee__ inline decltype(auto) load(const Tensor& tensor, const Coord& coord)
{
    using engine_type = typename Tensor::engine_type;
    using data_type = typename Tensor::data_type;
    static_assert(
        Std::is_same_v<get_mem_location<engine_type>, location::ub>, "load only supports tensors located in UB");
    TENSOR_API_DEBUG_CHECK(debug_check_coord, tensor.layout(), coord, "load");

    auto src_engine = tensor.engine() + tensor.layout()(coord);
    return detail::load_from_ptr<sideband_mode, data_type>(src_engine.begin().get());
}

template <load_sideband_mode sideband_mode, typename Tensor>
__simd_callee__ inline decltype(auto) load(const Tensor& tensor)
{
    using engine_type = typename Tensor::engine_type;
    using data_type = typename Tensor::data_type;
    static_assert(
        Std::is_same_v<get_mem_location<engine_type>, location::ub>, "load only supports tensors located in UB");

    return detail::load_from_ptr<sideband_mode, data_type>(tensor.engine().begin().get());
}

template <broadcast_mode brc_mode, typename Tensor, typename Coord>
__simd_callee__ inline decltype(auto) load_broadcast(const Tensor& tensor, const Coord& coord)
{
    using engine_type = typename Tensor::engine_type;
    using data_type = typename Tensor::data_type;
    static_assert(
        Std::is_same_v<get_mem_location<engine_type>, location::ub>,
        "load_broadcast only supports tensors located in UB");
    TENSOR_API_DEBUG_CHECK(debug_check_coord, tensor.layout(), coord, "load_broadcast");

    auto src_engine = tensor.engine() + tensor.layout()(coord);
    return detail::load_broadcast_from_ptr<brc_mode, data_type>(src_engine.begin().get());
}

template <broadcast_mode brc_mode, typename Tensor>
__simd_callee__ inline decltype(auto) load_broadcast(const Tensor& tensor)
{
    using engine_type = typename Tensor::engine_type;
    using data_type = typename Tensor::data_type;
    static_assert(
        Std::is_same_v<get_mem_location<engine_type>, location::ub>,
        "load_broadcast only supports tensors located in UB");

    return detail::load_broadcast_from_ptr<brc_mode, data_type>(tensor.engine().begin().get());
}

template <typename DstType, typename Tensor, typename Coord, typename IndexType, typename>
__simd_callee__ inline decltype(auto) gather(
    const Tensor& tensor, const Coord& coord, const reg_tensor<IndexType>& index)
{
    using engine_type = typename Tensor::engine_type;
    using src_data_type = typename Tensor::data_type;
    using dst_data_type = detail::gather_dst_t<DstType, src_data_type>;
    static_assert(
        Std::is_same_v<get_mem_location<engine_type>, location::ub>,
        "gather only supports tensors located in UB");
    TENSOR_API_DEBUG_CHECK(debug_check_coord, tensor.layout(), coord, "gather");

    auto src_engine = tensor.engine() + tensor.layout()(coord);
    return detail::gather_from_ptr<dst_data_type, src_data_type>(src_engine.begin().get(), index);
}

template <typename DstType, typename Tensor, typename IndexType, typename>
__simd_callee__ inline decltype(auto) gather(const Tensor& tensor, const reg_tensor<IndexType>& index)
{
    using engine_type = typename Tensor::engine_type;
    using src_data_type = typename Tensor::data_type;
    using dst_data_type = detail::gather_dst_t<DstType, src_data_type>;
    static_assert(
        Std::is_same_v<get_mem_location<engine_type>, location::ub>,
        "gather only supports tensors located in UB");

    return detail::gather_from_ptr<dst_data_type, src_data_type>(tensor.engine().begin().get(), index);
}

template <typename DataType, typename IndexType>
__simd_callee__ inline reg_tensor<DataType> gather(
    const reg_tensor<DataType>& src, const reg_tensor<IndexType>& index)
{
    static_assert(
        detail::is_gather_reg_support_type_v<DataType, IndexType>,
        "gather from register does not support this source and index type combination");

    reg_tensor<DataType> dst;
    if constexpr (detail::is_gather_reg_support_type_v<DataType, IndexType>) {
        asc_gather(dst.reg, src.reg, index.reg);
    }
    dst.mask = index.mask;
    return dst;
}

} // namespace experimental
} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_DATA_LOAD_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_DATA_LOAD_IMPL_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_DATA_LOAD_IMPL_H
#endif
