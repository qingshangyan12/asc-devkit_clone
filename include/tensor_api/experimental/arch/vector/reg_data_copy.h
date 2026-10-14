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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_DATA_COPY_H
#endif

/*!
 * \file reg_data_copy.h
 * \brief Register data copy interfaces.
 */
#ifndef INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_DATA_COPY_H
#define INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_DATA_COPY_H

#include "tensor_api/experimental/arch/vector/mask_reg_compute.h"
#include "tensor_api/experimental/arch/vector/reg_tensor.h"
#include "tensor_api/experimental/utils/reg_enum.h"

namespace asc {
namespace te {
namespace experimental {
/**
 * @brief Extracts bits from a data block of a register tensor and expands them into a register mask.
 * @tparam Offset Index of the data block from which bits are extracted.
 * @tparam T Source element type. Supported types are uint16_t and uint32_t.
 * @param src Source register tensor.
 * @return A register mask containing the extracted and expanded bits.
 */
template <int16_t Offset, typename T>
__simd_callee__ inline reg_tensor<bool> move_mask(const reg_tensor<T>& src);

/**
 * @brief Copies register data under the specified mask.
 * @tparam T Register element type.
 * @param dst Destination register tensor. Its mask is updated from mask.
 * @param src Source register tensor.
 * @param mask Active lanes to copy. Defaults to all lanes for T.
 */
template <typename T>
__simd_callee__ inline void move(
    reg_tensor<T>& dst, const reg_tensor<T>& src, const reg_tensor<bool>& mask = all_mask<T>());
} // namespace experimental
} // namespace te
} // namespace asc

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/tensor_api/experimental/arch/vector/reg_data_copy_impl.h"
#endif

#endif // INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_DATA_COPY_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_DATA_COPY_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_DATA_COPY_H
#endif
