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
 * @ingroup tensor_api.experimental.reg_load_store.reg_store
 * @override_id te_experimental_store__Tensor_R_const_Coord_R_const_reg_tensor_RegDataType_R__template_sideband_mode_Tensor_Coord_RegDataType
 * @archs 3510
 * @brief Stores register data to a UB tensor at the specified coordinate.
 * @note The tensor and register element types must match.
 * @param [out] tensor Destination tensor located in UB.
 * @param [in] coord Coordinate of the first destination element.
 * @param [in] src Source register tensor.
 */
template <
    store_sideband_mode sideband_mode = store_sideband_mode::direct, typename Tensor, typename Coord,
    typename RegDataType>
__simd_callee__ inline void store(Tensor& tensor, const Coord& coord, const reg_tensor<RegDataType>& src);

/**
 * @ingroup tensor_api.experimental.reg_load_store.reg_store
 * @override_id te_experimental_store__Tensor_R_const_reg_tensor_RegDataType_R__template_sideband_mode_Tensor_RegDataType
 * @archs 3510
 * @brief Stores register data at the beginning of a UB tensor.
 * @note The tensor and register element types must match.
 * @param [out] tensor Destination tensor located in UB.
 * @param [in] src Source register tensor.
 */
template <store_sideband_mode sideband_mode = store_sideband_mode::direct, typename Tensor, typename RegDataType>
__simd_callee__ inline void store(Tensor& tensor, const reg_tensor<RegDataType>& src);

/**
 * @ingroup tensor_api.experimental.reg_load_store.reg_store
 * @override_id te_experimental_store__Tensor_R_const_Coord_R_const_reg_tensor_RegDataType_R_const_reg_tensor_RegDataType_R__template_sideband_mode_Tensor_Coord_RegDataType
 * @archs 3510
 * @brief Interleaves two register tensors and stores the result at the specified coordinate of a UB tensor.
 * @note This overload supports only store_sideband_mode::intlv. The tensor and register element types must match.
 * @param [out] tensor Destination tensor located in UB.
 * @param [in] coord Coordinate of the first destination element.
 * @param [in] src0 First source register tensor.
 * @param [in] src1 Second source register tensor.
 */
template <
    store_sideband_mode sideband_mode = store_sideband_mode::intlv, typename Tensor, typename Coord,
    typename RegDataType>
__simd_callee__ inline void store(
    Tensor& tensor, const Coord& coord, const reg_tensor<RegDataType>& src0, const reg_tensor<RegDataType>& src1);

/**
 * @ingroup tensor_api.experimental.reg_load_store.reg_store
 * @override_id te_experimental_store__Tensor_R_const_reg_tensor_RegDataType_R_const_reg_tensor_RegDataType_R__template_sideband_mode_Tensor_RegDataType
 * @archs 3510
 * @brief Interleaves two register tensors and stores the result at the beginning of a UB tensor.
 * @note This overload supports only store_sideband_mode::intlv. The tensor and register element types must match.
 * @param [out] tensor Destination tensor located in UB.
 * @param [in] src0 First source register tensor.
 * @param [in] src1 Second source register tensor.
 */
template <store_sideband_mode sideband_mode = store_sideband_mode::intlv, typename Tensor, typename RegDataType>
__simd_callee__ inline void store(
    Tensor& tensor, const reg_tensor<RegDataType>& src0, const reg_tensor<RegDataType>& src1);

/**
 * @ingroup tensor_api.experimental.reg_load_store.reg_store
 * @override_id te_experimental_store_scatter__Tensor_R_const_reg_tensor_T_R_const_reg_tensor_Index_R__template_Tensor_T_Index
 * @archs 3510
 * @brief Scatters register data from active source lanes to a UB tensor.
 * @note The tensor and source register element types must match.
 * @param [out] tensor Destination tensor located in UB.
 * @param [in] src Source register tensor. Its active-lane mask selects the elements to store.
 * @param [in] index Register tensor containing the destination offsets.
 */
template <typename Tensor, typename T, typename Index>
__simd_callee__ inline void store_scatter(Tensor& tensor, const reg_tensor<T>& src, const reg_tensor<Index>& index);

/**
 * @ingroup tensor_api.experimental.reg_load_store.reg_store
 * @override_id te_experimental_store_scatter__Tensor_R_const_Coord_R_const_reg_tensor_T_R_const_reg_tensor_Index_R__template_Tensor_Coord_T_Index
 * @archs 3510
 * @brief Scatters register data from active source lanes to a UB tensor at the specified coordinate.
 * @note The tensor and source register element types must match.
 * @param [out] tensor Destination tensor located in UB.
 * @param [in] coord Coordinate used as the base destination position.
 * @param [in] src Source register tensor. Its active-lane mask selects the elements to store.
 * @param [in] index Register tensor containing the destination offsets relative to the base position.
 */
template <typename Tensor, typename Coord, typename T, typename Index>
__simd_callee__ inline void store_scatter(
    Tensor& tensor, const Coord& coord, const reg_tensor<T>& src, const reg_tensor<Index>& index);

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
