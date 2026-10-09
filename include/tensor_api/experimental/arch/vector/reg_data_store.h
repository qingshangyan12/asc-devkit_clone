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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_DATA_STORE_H
#endif

/*!
 * \file reg_data_store.h
 * \brief Register data store interfaces.
 */
#ifndef INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_DATA_STORE_H
#define INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_DATA_STORE_H

#include "tensor_api/experimental/arch/vector/reg_tensor.h"
#include "tensor_api/experimental/utils/reg_enum.h"

namespace asc {
namespace te {
namespace experimental {

/**
 * @brief Stores register data to a UB tensor at the specified coordinate.
 * @param tensor Destination tensor located in UB.
 * @param coord Coordinate of the first destination element.
 * @param src Source register tensor.
 * @note The tensor and register element types must match.
 */
template <
    store_sideband_mode sideband_mode = store_sideband_mode::direct, typename Tensor, typename Coord,
    typename RegDataType>
__simd_callee__ inline void store(Tensor& tensor, const Coord& coord, const reg_tensor<RegDataType>& src);

/**
 * @brief Stores register data at the beginning of a UB tensor.
 * @param tensor Destination tensor located in UB.
 * @param src Source register tensor.
 * @note The tensor and register element types must match.
 */
template <store_sideband_mode sideband_mode = store_sideband_mode::direct, typename Tensor, typename RegDataType>
__simd_callee__ inline void store(Tensor& tensor, const reg_tensor<RegDataType>& src);

/**
 * @brief Interleaves two register tensors and stores the result at the specified coordinate of a UB tensor.
 * @param tensor Destination tensor located in UB.
 * @param coord Coordinate of the first destination element.
 * @param src0 First source register tensor.
 * @param src1 Second source register tensor.
 * @note This overload supports only store_sideband_mode::intlv. The tensor and register element types must match.
 */
template <
    store_sideband_mode sideband_mode = store_sideband_mode::intlv, typename Tensor, typename Coord,
    typename RegDataType>
__simd_callee__ inline void store(
    Tensor& tensor, const Coord& coord, const reg_tensor<RegDataType>& src0, const reg_tensor<RegDataType>& src1);

/**
 * @brief Interleaves two register tensors and stores the result at the beginning of a UB tensor.
 * @param tensor Destination tensor located in UB.
 * @param src0 First source register tensor.
 * @param src1 Second source register tensor.
 * @note This overload supports only store_sideband_mode::intlv. The tensor and register element types must match.
 */
template <store_sideband_mode sideband_mode = store_sideband_mode::intlv, typename Tensor, typename RegDataType>
__simd_callee__ inline void store(
    Tensor& tensor, const reg_tensor<RegDataType>& src0, const reg_tensor<RegDataType>& src1);

/**
 * @brief Maintains the hardware state for a sequence of unaligned register-to-UB stores.
 * @note Each store writes one full vector register and does not use reg_tensor::mask.
 * Destination addresses must form one contiguous stream. All tensors passed to one
 * session must have the same element type. Call flush() exactly once with the first
 * destination address after the stores are complete.
 */
struct unalign_store {
    __simd_callee__ unalign_store() = default;

    /**
     * @brief Stores one full vector register at the beginning of a UB tensor.
     * @param dst_tile Tensor whose beginning is the destination address for this store.
     * @param src Source register tensor. Its mask is not used. An int4b_t destination
     * requires reg_tensor<int4x2_t>.
     * @param count Number of elements to store.
     */
    template <typename Tensor, typename RegDataType>
    __simd_callee__ inline void store(Tensor dst_tile, const reg_tensor<RegDataType>& src, uint32_t count);

    /**
     * @brief Stores register data to a UB tensor at the specified coordinate.
     * @param dst_tile Destination tensor in the unaligned store stream.
     * @param coord Coordinate of the first destination element.
     * @param src Source register tensor. Its mask is not used. An int4b_t destination
     * requires reg_tensor<int4x2_t>.
     * @param count Number of elements to store.
     */
    template <typename Tensor, typename Coord, typename RegDataType>
    __simd_callee__ inline void store(
        Tensor dst_tile, const Coord& coord, const reg_tensor<RegDataType>& src, uint32_t count);

    /**
     * @brief Commits the pending unaligned stores.
     * @param dst_base Tensor whose beginning is the destination address of the first store.
     * @param coord Coordinate passed to the unaligned-store post operation.
     */
    template <typename Tensor>
    __simd_callee__ inline void flush(Tensor dst_base, int32_t coord);

private:
    vector_store_unalign ureg;
};

} // namespace experimental
} // namespace te
} // namespace asc

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/tensor_api/experimental/arch/vector/reg_data_store_impl.h"
#endif

#endif // INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_DATA_STORE_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_DATA_STORE_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_DATA_STORE_H
#endif
