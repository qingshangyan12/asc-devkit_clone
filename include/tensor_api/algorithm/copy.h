/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement version 2.0 (the "License").
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
* \file copy.h
* \brief
*/
#ifndef INCLUDE_TENSOR_API_ALGORITHM_COPY_H
#define INCLUDE_TENSOR_API_ALGORITHM_COPY_H

#include "impl/tensor_api/algorithm/copy_impl.h"

namespace asc {
namespace te {

/**
 * @brief Perform a copy operation with a preconstructed copy_atom.
 * @param atom_copy : copy atom object that determines the copy behavior.
 * @param dst : Destination tensor.
 * @param src : Source tensor.
 */
template <typename atom_type, typename dst_tensor_type, typename src_tensor_type>
__aicore__ inline void copy(const copy_atom<atom_type>& atom_copy, const dst_tensor_type& dst, const src_tensor_type& src);

/**
 * @brief Perform a copy operation with a preconstructed copy_atom and a quantization parameter.
 * @param atom_copy : copy atom object that determines the copy behavior.
 * @param dst : Destination tensor.
 * @param src : Source tensor.
 * @param quant : Quantization parameter, which can be a scalar or a tensor_type API tensor.
 */
template <typename atom_type, typename dst_tensor_type, typename src_tensor_type, typename quant_param_type,
    Std::enable_if_t<is_copy_quant_param_v<quant_param_type>, int> Enable>
__aicore__ inline void copy(const copy_atom<atom_type>& atom_copy, const dst_tensor_type& dst, const src_tensor_type& src,
    const quant_param_type& quant);

/**
 * @brief Perform a copy operation on the region selected by the copy operation.
 * @param coord_dst : Start coordinate in the destination tensor, or zero_coord.
 * @param coord_src : Start coordinate in the source tensor, or zero_coord.
 * @param shape : requested_type region shape.
 */
template <typename atom_type, typename dst_tensor_type, typename src_tensor_type, typename coord_dst_type, typename coord_src_type,
    typename shape_type, Std::enable_if_t<is_copy_region_param_v<coord_dst_type, coord_src_type, shape_type>, int> Enable>
__aicore__ inline void copy(const copy_atom<atom_type>& atom_copy, const dst_tensor_type& dst, const src_tensor_type& src,
    const coord_dst_type& coord_dst, const coord_src_type& coord_src, const shape_type& shape);

/**
 * @brief Perform a quantized copy operation on the region selected by the copy operation.
 */
template <typename atom_type, typename dst_tensor_type, typename src_tensor_type, typename quant_param_type, typename coord_dst_type,
    typename coord_src_type, typename shape_type,
    Std::enable_if_t<is_copy_quant_param_v<quant_param_type> && is_copy_region_param_v<coord_dst_type, coord_src_type, shape_type>, int> Enable>
__aicore__ inline void copy(const copy_atom<atom_type>& atom_copy, const dst_tensor_type& dst, const src_tensor_type& src,
    const quant_param_type& quant, const coord_dst_type& coord_dst, const coord_src_type& coord_src, const shape_type& shape);

/**
 * @brief Construct a copy_atom from the copy operation object.
 * @param copy_operation : copy operation object.
 */
template <typename CopyOperationType>
__aicore__ inline constexpr auto make_copy(const CopyOperationType& copy_operation);

/**
 * @brief Construct a copy_atom from the copy operation object and trait object.
 * @param copy_operation : copy operation object.
 * @param copy_trait : copy trait object.
 */
template <typename CopyOperationType, typename CopyTraitType>
__aicore__ inline constexpr auto make_copy(const CopyOperationType& copy_operation, const CopyTraitType& copy_trait);

}
}

#endif // INCLUDE_TENSOR_API_ALGORITHM_COPY_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif
