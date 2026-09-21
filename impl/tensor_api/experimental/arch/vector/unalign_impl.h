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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_UNALIGN_IMPL_H
#endif

#ifndef IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_UNALIGN_IMPL_H
#define IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_UNALIGN_IMPL_H

#include "impl/tensor_api/experimental/arch/utils/reg_utils.h"
#include "impl/tensor_api/experimental/arch/vector/complex_ops_impl.h"
#include "tensor_api/tensor/tensor.h"

namespace asc {
namespace te {
namespace experimental {

template <typename Tensor>
__simd_callee__ inline void unalign_load::prepare(const Tensor& src_base)
{
    using engine_type = typename Tensor::engine_type;
    static_assert(
        Std::is_same_v<get_mem_location<engine_type>, location::ub>,
        "unalign_load::prepare only supports tensors located in UB");
    asc_loadunalign_pre(ureg_, src_base.engine().begin().get());
}

template <typename Tensor>
__simd_callee__ inline auto unalign_load::load(const Tensor& src_tile)
{
    using engine_type = typename Tensor::engine_type;
    using data_type = typename Tensor::data_type;
    static_assert(
        Std::is_same_v<get_mem_location<engine_type>, location::ub>,
        "unalign_load::load only supports tensors located in UB");
    reg_tensor<data_type> reg;
    asc_loadunalign(reg.reg, ureg_, src_tile.engine().begin().get());
    return reg;
}

template <typename Tensor>
__simd_callee__ inline auto unalign_load::load_complex(const Tensor& src_tile)
{
    using engine_type = typename Tensor::engine_type;
    using data_type = typename Tensor::data_type;
    static_assert(
        Std::is_same_v<get_mem_location<engine_type>, location::ub>,
        "unalign_load::load_complex only supports tensors located in UB");
    static_assert(
        Std::is_same_v<data_type, half> || Std::is_same_v<data_type, float>,
        "unalign_load::load_complex requires half or float tensor element type");

    // Two unalign beats of interleaved elements, then software deinterleave (basic_api pattern).
    reg_tensor<data_type> tmp0;
    reg_tensor<data_type> tmp1;
    auto* ptr = src_tile.engine().begin().get();
    asc_loadunalign(tmp0.reg, ureg_, ptr);
    constexpr uint32_t vf_bytes = 256;
    constexpr uint32_t elem_per_reg = vf_bytes / sizeof(data_type);
    asc_loadunalign(tmp1.reg, ureg_, ptr + elem_per_reg);

    using Tag = typename complex_map<data_type>::type;
    typename reg_tensor<Tag>::reg_type real;
    typename reg_tensor<Tag>::reg_type imag;
    asc_deintlv(real, imag, tmp0.reg, tmp1.reg);
    vector_bool mask;
    if constexpr (Std::is_same_v<data_type, half>) {
        mask = asc_create_mask_b16(PAT_ALL);
    } else {
        mask = asc_create_mask_b32(PAT_ALL);
    }
    return detail::make_complex<Tag>(real, imag, mask);
}

template <typename Tensor, typename DataType>
__simd_callee__ inline void unalign_store::store(Tensor& dst_tile, const reg_tensor<DataType>& reg)
{
    using engine_type = typename Tensor::engine_type;
    using data_type = typename Tensor::data_type;
    static_assert(
        Std::is_same_v<get_mem_location<engine_type>, location::ub>,
        "unalign_store::store only supports tensors located in UB");
    static_assert(
        Std::is_same_v<data_type, DataType>, "unalign_store::store tensor and register element types must match");
    const uint32_t vl = static_cast<uint32_t>(dst_tile.size());
    asc_storeunalign(dst_tile.engine().begin().get(), ureg_, reg.reg, vl);
    count_ += vl;
}

template <typename Tensor, typename Elem>
__simd_callee__ inline void unalign_store::store_complex(Tensor& dst_tile, const complex<Elem>& reg)
{
    using engine_type = typename Tensor::engine_type;
    using data_type = typename Tensor::data_type;
    static_assert(
        Std::is_same_v<get_mem_location<engine_type>, location::ub>,
        "unalign_store::store_complex only supports tensors located in UB");
    static_assert(Std::is_same_v<data_type, Elem>, "unalign_store::store_complex element type mismatch");

    typename reg_tensor<Elem>::reg_type v0;
    typename reg_tensor<Elem>::reg_type v1;
    asc_intlv(v0, v1, reg.real_, reg.imag_);
    auto* ptr = dst_tile.engine().begin().get();
    constexpr uint32_t vf_bytes = 256;
    constexpr uint32_t elem_per_reg = vf_bytes / sizeof(Elem);
    const uint32_t vl = static_cast<uint32_t>(dst_tile.size());
    // Store two interleaved half-registers; accumulate complex element count.
    asc_storeunalign(ptr, ureg_, v0, elem_per_reg);
    asc_storeunalign(ptr + elem_per_reg, ureg_, v1, elem_per_reg);
    count_ += vl;
}

template <typename Tensor>
__simd_callee__ inline void unalign_store::flush(Tensor& dst_base)
{
    using engine_type = typename Tensor::engine_type;
    static_assert(
        Std::is_same_v<get_mem_location<engine_type>, location::ub>,
        "unalign_store::flush only supports tensors located in UB");
    asc_storeunalign_post(dst_base.engine().begin().get(), ureg_, static_cast<int32_t>(count_));
}

} // namespace experimental
} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_UNALIGN_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_UNALIGN_IMPL_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_UNALIGN_IMPL_H
#endif
