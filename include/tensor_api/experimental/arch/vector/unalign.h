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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_PUBLIC_UNALIGN_H
#endif

/*!
 * \file unalign.h
 * \brief Session wrappers for unaligned UB ↔ register load/store.
 *
 * \note Protocol:
 *       load  : prepare(src_base) → load(src_tile)×N
 *       store : prepare() → store(dst_tile, reg)×N → flush(dst_base)
 * \note Sessions hold ureg (and store count) only; they do not cache tensors.
 * \note Design: tensor_api feature design / 7_design_unalign_api.md
 */
#ifndef INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_UNALIGN_H
#define INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_UNALIGN_H

#include <cstdint>

#include "tensor_api/experimental/arch/vector/reg_tensor.h"

namespace asc {
namespace te {
namespace experimental {

/**
 * @brief Unaligned load session: prepare ≈ asc_loadunalign_pre, load ≈ asc_loadunalign.
 *
 * @note Does not cache the tensor; each beat address is supplied by the caller.
 * @note prepare base must match the first load beat; later beats must advance on the same contiguous stream.
 */
struct unalign_load {
    __simd_callee__ unalign_load() = default;

    /**
     * @brief Hardware pre only; does not cache the tensor.
     * @param src_base Unaligned stream start located in UB.
     */
    template <typename Tensor>
    __simd_callee__ inline void prepare(const Tensor& src_base);

    /**
     * @brief Loads one beat into a register tensor.
     * @param src_tile Source tile for this beat (absolute address from caller).
     * @return Register tensor populated by asc_loadunalign.
     */
    template <typename Tensor>
    __simd_callee__ inline auto load(const Tensor& src_tile);

    /**
     * @brief Unaligned complex load: two unalign beats + software deinterleave.
     * @tparam Tensor UB tensor with @c half or @c float element type (interleaved complex).
     * @return @c complex<half> or @c complex<float>.
     * @note Corresponds to basic_api continuous LoadUnAlign for complex32/64 (RegTraitNumTwo).
     */
    template <typename Tensor>
    __simd_callee__ inline auto load_complex(const Tensor& src_tile);

private:
    vector_load_unalign ureg_;
};

/**
 * @brief Unaligned store session: store ≈ asc_storeunalign, flush ≈ asc_storeunalign_post.
 *
 * @note prepare() only resets the accumulated element count (no hardware pre).
 * @note flush base must equal the first store base of the segment (do not pass the last-beat slice).
 */
struct unalign_store {
    __simd_callee__ unalign_store() = default;

    /**
     * @brief Stores one beat and accumulates the element count.
     * @param dst_tile Destination tile for this beat.
     * @param reg Source register tensor (element type must match the tensor).
     * @note vl is taken from dst_tile.size().
     */
    template <typename Tensor, typename DataType>
    __simd_callee__ inline void store(Tensor& dst_tile, const reg_tensor<DataType>& reg);

    /**
     * @brief Unaligned complex store: software interleave + two unalign stores.
     */
    template <typename Tensor, typename Elem>
    __simd_callee__ inline void store_complex(Tensor& dst_tile, const complex<Elem>& reg);

    /**
     * @brief Commits the session (hardware post) with segment start base + accumulated count.
     * @param dst_base Start address of the first store in this segment.
     */
    template <typename Tensor>
    __simd_callee__ inline void flush(Tensor& dst_base);

private:
    vector_store_unalign ureg_;
    uint32_t count_ = 0;
};

} // namespace experimental
} // namespace te
} // namespace asc

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/tensor_api/experimental/arch/vector/unalign_impl.h"
#endif

#endif // INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_UNALIGN_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_PUBLIC_UNALIGN_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_PUBLIC_UNALIGN_H
#endif
