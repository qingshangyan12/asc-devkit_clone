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
    "reg_data_store_impl.h is internal; include <tensor_api/experimental/arch/vector/reg_data_store.h> instead."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_DATA_STORE_IMPL
#endif

#ifndef IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_DATA_STORE_IMPL_H
#define IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_DATA_STORE_IMPL_H

#if !defined(INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_DATA_STORE_H)
#include "tensor_api/experimental/arch/vector/reg_data_store.h"
#endif
#include "tensor_api/tensor/tensor.h"

namespace asc {
namespace te {
namespace experimental {
namespace detail {

template <size_t Bytes>
struct store_datatype;

template <>
struct store_datatype<1> {
    using type = uint8_t;
};

template <>
struct store_datatype<2> {
    using type = uint16_t;
};

template <>
struct store_datatype<4> {
    using type = uint32_t;
};

template <>
struct store_datatype<8> {
    using type = uint64_t;
};

template <typename DataType>
inline constexpr size_t store_element_bytes =
    is_b4_type<Std::remove_cvref_t<DataType>> ? 1 : sizeof(Std::remove_cvref_t<DataType>);

template <typename DataType, size_t PhysicalWidthRatio = 1>
using store_datatype_t =
    typename store_datatype<store_element_bytes<DataType> * PhysicalWidthRatio>::type;

template <store_sideband_mode sideband_mode>
struct store_to_ptr_impl;

template <>
struct store_to_ptr_impl<store_sideband_mode::direct> {
    template <typename Pointer, typename RegDataType>
    __simd_callee__ static inline void run(Pointer dst, const reg_tensor<RegDataType>& src)
    {
        using scalar_type = store_datatype_t<RegDataType>;
        using reg_type = typename type_get<scalar_type>::type;
        auto* rep_dst = reinterpret_cast<__ubuf__ scalar_type*>(dst);
        const auto& rep_src = reinterpret_cast<const reg_type&>(src.reg);
        asc_storealign(rep_dst, rep_src, src.mask);
    }
};

template <>
struct store_to_ptr_impl<store_sideband_mode::store_1st> {
    template <typename Pointer, typename RegDataType>
    __simd_callee__ static inline void run(Pointer dst, const reg_tensor<RegDataType>& src)
    {
        using scalar_type = store_datatype_t<RegDataType>;
        using reg_type = typename type_get<scalar_type>::type;
        auto* rep_dst = reinterpret_cast<__ubuf__ scalar_type*>(dst);
        const auto& rep_src = reinterpret_cast<const reg_type&>(src.reg);
        asc_storealign_1st(rep_dst, rep_src);
    }
};

template <>
struct store_to_ptr_impl<store_sideband_mode::pack> {
    template <typename Pointer, typename RegDataType>
    __simd_callee__ static inline void run(Pointer dst, const reg_tensor<RegDataType>& src)
    {
        // Select matching pointer and register C API types from the pre-pack lane width.
        using scalar_type = store_datatype_t<RegDataType, 2>;
        using reg_type = typename type_get<scalar_type>::type;
        auto* rep_dst = reinterpret_cast<__ubuf__ scalar_type*>(dst);
        const auto& rep_src = reinterpret_cast<const reg_type&>(src.reg);
        asc_storealign_pack(rep_dst, rep_src, src.mask);
    }
};

template <>
struct store_to_ptr_impl<store_sideband_mode::pack_quarter> {
    template <typename Pointer, typename RegDataType>
    __simd_callee__ static inline void run(Pointer dst, const reg_tensor<RegDataType>& src)
    {
        using scalar_type = store_datatype_t<RegDataType, 4>;
        static_assert(sizeof(scalar_type) == sizeof(uint32_t),
            "pack_quarter requires a b32 physical lane type");
        using reg_type = typename type_get<scalar_type>::type;
        auto* rep_dst = reinterpret_cast<__ubuf__ scalar_type*>(dst);
        const auto& rep_src = reinterpret_cast<const reg_type&>(src.reg);
        asc_storealign_pack_quarter(rep_dst, rep_src, src.mask);
    }
};

template <>
struct store_to_ptr_impl<store_sideband_mode::intlv> {
    template <typename Pointer, typename RegDataType>
    __simd_callee__ static inline void run(
        Pointer dst, const reg_tensor<RegDataType>& src0, const reg_tensor<RegDataType>& src1)
    {
        using scalar_type = store_datatype_t<RegDataType>;
        using reg_type = typename type_get<scalar_type>::type;
        auto* rep_dst = reinterpret_cast<__ubuf__ scalar_type*>(dst);
        const auto& rep_src0 = reinterpret_cast<const reg_type&>(src0.reg);
        const auto& rep_src1 = reinterpret_cast<const reg_type&>(src1.reg);
        asc_storealign_intlv(rep_dst, rep_src0, rep_src1);
    }
};

template <store_sideband_mode sideband_mode, typename Pointer, typename RegDataType>
__simd_callee__ inline void store_to_ptr(Pointer dst, const reg_tensor<RegDataType>& src)
{
    store_to_ptr_impl<sideband_mode>::run(dst, src);
}

template <store_sideband_mode sideband_mode, typename Pointer, typename RegDataType>
__simd_callee__ inline void store_to_ptr(
    Pointer dst, const reg_tensor<RegDataType>& src0, const reg_tensor<RegDataType>& src1)
{
    store_to_ptr_impl<sideband_mode>::run(dst, src0, src1);
}

} // namespace detail

template <store_sideband_mode sideband_mode, typename Tensor, typename Coord, typename RegDataType>
__simd_callee__ inline void store(Tensor& tensor, const Coord& coord, const reg_tensor<RegDataType>& src)
{
    using engine_type = typename Tensor::engine_type;
    using data_type = typename Tensor::data_type;
    static_assert(Std::is_same_v<get_mem_location<engine_type>, location::ub>,
        "store only supports tensors located in UB");
    static_assert(Std::is_same_v<data_type, RegDataType>,
        "store requires matching tensor and register element types");
    TENSOR_API_DEBUG_CHECK(debug_check_coord, tensor.layout(), coord, "store");

    auto dst_engine = tensor.engine() + tensor.layout()(coord);
    detail::store_to_ptr<sideband_mode>(dst_engine.begin().get(), src);
}

template <store_sideband_mode sideband_mode, typename Tensor, typename RegDataType>
__simd_callee__ inline void store(Tensor& tensor, const reg_tensor<RegDataType>& src)
{
    using engine_type = typename Tensor::engine_type;
    using data_type = typename Tensor::data_type;
    static_assert(Std::is_same_v<get_mem_location<engine_type>, location::ub>,
        "store only supports tensors located in UB");
    static_assert(Std::is_same_v<data_type, RegDataType>,
        "store requires matching tensor and register element types");

    detail::store_to_ptr<sideband_mode>(tensor.engine().begin().get(), src);
}

template <store_sideband_mode sideband_mode, typename Tensor, typename Coord, typename RegDataType>
__simd_callee__ inline void store(Tensor& tensor, const Coord& coord, const reg_tensor<RegDataType>& src0,
    const reg_tensor<RegDataType>& src1)
{
    using engine_type = typename Tensor::engine_type;
    using data_type = typename Tensor::data_type;
    static_assert(Std::is_same_v<get_mem_location<engine_type>, location::ub>,
        "store only supports tensors located in UB");
    static_assert(Std::is_same_v<data_type, RegDataType>,
        "store requires matching tensor and register element types");
    TENSOR_API_DEBUG_CHECK(debug_check_coord, tensor.layout(), coord, "store");

    auto dst_engine = tensor.engine() + tensor.layout()(coord);
    detail::store_to_ptr<sideband_mode>(dst_engine.begin().get(), src0, src1);
}

template <store_sideband_mode sideband_mode, typename Tensor, typename RegDataType>
__simd_callee__ inline void store(
    Tensor& tensor, const reg_tensor<RegDataType>& src0, const reg_tensor<RegDataType>& src1)
{
    using engine_type = typename Tensor::engine_type;
    using data_type = typename Tensor::data_type;
    static_assert(Std::is_same_v<get_mem_location<engine_type>, location::ub>,
        "store only supports tensors located in UB");
    static_assert(Std::is_same_v<data_type, RegDataType>,
        "store requires matching tensor and register element types");

    detail::store_to_ptr<sideband_mode>(tensor.engine().begin().get(), src0, src1);
}

} // namespace experimental
} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_DATA_STORE_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_DATA_STORE_IMPL)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_DATA_STORE_IMPL
#endif
