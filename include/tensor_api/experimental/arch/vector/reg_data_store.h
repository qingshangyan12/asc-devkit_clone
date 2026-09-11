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
/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.reg_data_store
 * @override_id store__RpTensor_RCpCoord_RCregtensorg1x12_pRegDataType
 * @archs 3510
 * @brief `store`用于将[reg_tensor](../reg_tensor/reg_tensor.md)中的数据搬出到Unified Buffer（UB）Tensor。接口通过`store_sideband_mode`配置正常搬出、首元素搬出、交织搬出或压缩搬出等数据排列方式。
 * @param [out] tensor 目的Tensor，必须位于UB。
 * @param [in] coord 搬出起始元素在`tensor`逻辑布局中的坐标。
 * @param [in] src 单源操作数重载的源寄存器。`direct`、`pack`和`pack_quarter`模式使用`src.mask`筛选有效元素；`store_1st`模式忽略`src.mask`。
 */
__simd_callee__ inline void store(Tensor& tensor, const Coord& coord, const reg_tensor<RegDataType>& src);

/**
 * @brief Stores register data at the beginning of a UB tensor.
 * @param tensor Destination tensor located in UB.
 * @param src Source register tensor.
 * @note The tensor and register element types must match.
 */
template <store_sideband_mode sideband_mode = store_sideband_mode::direct, typename Tensor, typename RegDataType>
/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.reg_data_store
 * @override_id store__RpTensor_RCregtensorg1x12_pRegDataType
 * @archs 3510
 * @brief `store`用于将[reg_tensor](../reg_tensor/reg_tensor.md)中的数据搬出到Unified Buffer（UB）Tensor。接口通过`store_sideband_mode`配置正常搬出、首元素搬出、交织搬出或压缩搬出等数据排列方式。
 * @param [out] tensor 目的Tensor，必须位于UB。
 * @param [in] src 单源操作数重载的源寄存器。`direct`、`pack`和`pack_quarter`模式使用`src.mask`筛选有效元素；`store_1st`模式忽略`src.mask`。
 */
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
/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.reg_data_store
 * @override_id store__RpTensor_RCpCoord_RCregtensorg1x12_pRegDataType_RCregtensorg1x12_pRegDataType
 * @archs 3510
 * @brief `store`用于将[reg_tensor](../reg_tensor/reg_tensor.md)中的数据搬出到Unified Buffer（UB）Tensor。接口通过`store_sideband_mode`配置正常搬出、首元素搬出、交织搬出或压缩搬出等数据排列方式。
 * @param [out] tensor 目的Tensor，必须位于UB。
 * @param [in] coord 搬出起始元素在`tensor`逻辑布局中的坐标。
 * @param [in] src0 intlv`模式的第一个源寄存器。该模式忽略`src0.mask`。
 * @param [in] src1 intlv`模式的第二个源寄存器。该模式忽略`src1.mask`。
 */
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
/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.reg_data_store
 * @override_id store__RpTensor_RCregtensorg1x12_pRegDataType_RCregtensorg1x12_pRegDataType
 * @archs 3510
 * @brief `store`用于将[reg_tensor](../reg_tensor/reg_tensor.md)中的数据搬出到Unified Buffer（UB）Tensor。接口通过`store_sideband_mode`配置正常搬出、首元素搬出、交织搬出或压缩搬出等数据排列方式。
 * @param [out] tensor 目的Tensor，必须位于UB。
 * @param [in] src0 intlv`模式的第一个源寄存器。该模式忽略`src0.mask`。
 * @param [in] src1 intlv`模式的第二个源寄存器。该模式忽略`src1.mask`。
 */
__simd_callee__ inline void store(
    Tensor& tensor, const reg_tensor<RegDataType>& src0, const reg_tensor<RegDataType>& src1);

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
