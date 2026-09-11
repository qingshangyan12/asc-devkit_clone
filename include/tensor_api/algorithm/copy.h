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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif

/*!
 * \file copy.h
 * \brief
 */
#ifndef INCLUDE_TENSOR_API_ALGORITHM_COPY_H
#define INCLUDE_TENSOR_API_ALGORITHM_COPY_H

#include "tensor_api/tensor/layout_interface.h"
#include "tensor_api/tensor/pointer.h"
#include "tensor_api/atom/cube/copy_atom.h"
#include "impl/tensor_api/algorithm/copy_impl.h"

namespace asc {
namespace te {

/**
 * @brief Perform a copy operation with a preconstructed copy_atom.
 * @param atom : Copy atom object that determines the copy behavior.
 * @param dst : Destination tensor.
 * @param src : Source tensor.
 */
template <typename Atom, typename DstTensor, typename SrcTensor>
/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.vector_compute_load
 * @override_id copy__RCcopyatomg1x5_pAtom_RCpDstTensor_RCpSrcTensor
 * @archs 3510
 * @brief Tensor API通过`copy`接口统一执行不同通路数据搬运。该接口用于将Global Memory中的数据搬运到L1 Buffer。`copy`接口根据源张量和目的张量的存储位置、数据类型和布局选择具体搬运实现。搬运块数、搬运长度、源/目的侧步长以及格式转换相关信息由Tensor布局推导，用户不需要在`copy`调用中额外传入搬运参数。
 * @param [in] atom 搬运原子对象，可由`make_copy`接口构造。
 * @param [out] dst 目的张量，存储位置为`location::l1`。
 * @param [in] src 源张量，存储位置为`location::gm`。
 */
__aicore__ inline void copy(const copy_atom<Atom>& atom, const DstTensor& dst, const SrcTensor& src);

/**
 * @brief Perform a copy operation with a preconstructed copy_atom and a quantization parameter.
 * @param atom : Copy atom object that determines the copy behavior.
 * @param dst : Destination tensor.
 * @param src : Source tensor.
 * @param quant : Quantization parameter, which can be a scalar or a Tensor API tensor.
 */
template <
    typename Atom, typename DstTensor, typename SrcTensor, typename Quant,
    Std::enable_if_t<is_valid_quant_v<Quant>, int> Enable>
/**
 * @ingroup simd_api.tensor_api.matrix_compute.cube_compute_store
 * @override_id copy__RCcopyatomg1x5_pAtom_RCpDstTensor_RCpSrcTensor_RCpQuant
 * @archs 3510
 * @brief Tensor API通过`copy`接口统一执行不同通路数据搬运。该接口用于将L0C Buffer中的矩阵计算结果搬运到Global Memory。L0C Buffer中的数据通常为`mmad`的输出，数据格式为`NZ`。搬运到Global Memory时，接口会根据目的张量布局自动选择`NZ`到`ND`、`DN`、`NZ`、`NC1HWC0`、`NHWC`或`NCHW`的随路格式转换。其中`NC1HWC0`、`NHWC`和`NCHW`用于卷积输出特征图。
 * @param [in] atom 搬运原子对象，可由`make_copy`接口构造。
 * @param [out] dst 目的张量，存储位置为`location::gm`。支持`ND`、`DN`、`NZ`、`NC1HWC0`、`NHWC`和`NCHW`数据格式。
 * @param [in] src 源张量，存储位置为`location::l0c`，数据格式为`NZ`，通常为`mmad`的计算结果。
 * @param [in] quant 可选量化参数。传入`uint64_t`时表示scalar量化参数，传入张量时表示tensor量化参数，张量位于L1 Buffer，元素类型为`uint64_t`。
 */
__aicore__ inline void copy(
    const copy_atom<Atom>& atom, const DstTensor& dst, const SrcTensor& src, const Quant& quant);

/**
 * @brief Perform a copy operation using the default atom inferred from the tensor memory locations.
 * @param dst : Destination tensor.
 * @param src : Source tensor.
 */
template <
    typename DstTensor, typename SrcTensor,
    Std::enable_if_t<is_attr_tensor_v<DstTensor> && is_attr_tensor_v<SrcTensor>, int> Enable>
/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.vector_compute_load
 * @override_id copy__RCpDstTensor_RCpSrcTensor
 * @archs 3510
 * @brief Tensor API通过`copy`接口统一执行不同通路数据搬运。该接口用于将Global Memory中的数据搬运到L1 Buffer。`copy`接口根据源张量和目的张量的存储位置、数据类型和布局选择具体搬运实现。搬运块数、搬运长度、源/目的侧步长以及格式转换相关信息由Tensor布局推导，用户不需要在`copy`调用中额外传入搬运参数。
 * @param [out] dst 目的张量，存储位置为`location::l1`。
 * @param [in] src 源张量，存储位置为`location::gm`。
 */
__aicore__ inline void copy(const DstTensor& dst, const SrcTensor& src);

/**
 * @brief Perform a quantized copy using the default atom inferred from the tensor memory locations.
 * @param dst : Destination tensor.
 * @param src : Source tensor.
 * @param quant : Quantization parameter, which can be a scalar or a Tensor API tensor.
 */
template <
    typename DstTensor, typename SrcTensor, typename QuantParam,
    Std::enable_if_t<is_attr_tensor_v<DstTensor> && is_attr_tensor_v<SrcTensor> && is_valid_quant_v<QuantParam>, int>
        Enable>
__aicore__ inline void copy(const DstTensor& dst, const SrcTensor& src, const QuantParam& quant);

/**
 * @brief Perform a copy operation using source/destination coordinates and a requested shape.
 * @param atom : Copy atom object that determines the copy behavior.
 * @param dst : Destination tensor.
 * @param src : Source tensor.
 * @param dst_coord : Start coordinate in the destination tensor, or zero_coord.
 * @param src_coord : Start coordinate in the source tensor, or zero_coord.
 * @param copy_shape : Shape of the region to copy.
 */
template <
    typename Atom, typename DstTensor, typename SrcTensor, typename DstCoord, typename SrcCoord, typename CopyShape,
    Std::enable_if_t<is_valid_coord_v<DstCoord> && is_valid_coord_v<SrcCoord> && is_valid_shape_v<CopyShape>, int>
        Enable>
/**
 * @ingroup simd_api.tensor_api.matrix_compute.cube_compute_load
 * @override_id copy__RCcopyatomg1x5_pAtom_RCpDstTensor_RCpSrcTensor_RCpDstCoord_RCpSrcCoord_RCpCopyShape
 * @archs 3510
 * @brief Tensor API通过`copy`接口统一执行不同通路数据搬运。该接口用于将Global Memory中的数据搬运到L1 Buffer。`copy`接口根据源张量和目的张量的存储位置、数据类型和布局选择具体搬运实现。搬运块数、搬运长度、源/目的侧步长以及格式转换相关信息由Tensor布局推导，用户不需要在`copy`调用中额外传入搬运参数。
 * @param [in] atom 搬运原子对象，可由`make_copy`接口构造。
 * @param [out] dst 目的张量，存储位置为`location::l1`。
 * @param [in] src 源张量，存储位置为`location::gm`。
 * @param [in] dst_coord 搬运区域在目的张量中的起始坐标，也可传入`zero_coord`。
 * @param [in] src_coord 搬运区域在源张量中的起始坐标，也可传入`zero_coord`。
 * @param [in] copy_shape 搬运区域的形状，用于指定搬运区域的大小。
 */
__aicore__ inline void copy(
    const copy_atom<Atom>& atom, const DstTensor& dst, const SrcTensor& src, const DstCoord& dst_coord,
    const SrcCoord& src_coord, const CopyShape& copy_shape);

/**
 * @brief Perform a quantized copy operation using source/destination coordinates and a requested shape.
 * @param atom : Copy atom object that determines the copy behavior.
 * @param dst : Destination tensor.
 * @param src : Source tensor.
 * @param quant : Quantization parameter, which can be a scalar or a Tensor API tensor.
 * @param dst_coord : Start coordinate in the destination tensor, or zero_coord.
 * @param src_coord : Start coordinate in the source tensor, or zero_coord.
 * @param copy_shape : Shape of the region to copy.
 */
template <
    typename Atom, typename DstTensor, typename SrcTensor, typename Quant, typename DstCoord, typename SrcCoord,
    typename CopyShape,
    Std::enable_if_t<
        is_valid_quant_v<Quant> && is_valid_coord_v<DstCoord> && is_valid_coord_v<SrcCoord> &&
            is_valid_shape_v<CopyShape>,
        int>
        Enable>
/**
 * @ingroup simd_api.tensor_api.matrix_compute.cube_compute_store
 * @override_id copy__RCcopyatomg1x5_pAtom_RCpDstTensor_RCpSrcTensor_RCpQuant_RCpDstCoord_RCpSrcCoord_RCpCopyShape
 * @archs 3510
 * @brief Tensor API通过`copy`接口统一执行不同通路数据搬运。该接口用于将L0C Buffer中的矩阵计算结果搬运到Global Memory。L0C Buffer中的数据通常为`mmad`的输出，数据格式为`NZ`。搬运到Global Memory时，接口会根据目的张量布局自动选择`NZ`到`ND`、`DN`、`NZ`、`NC1HWC0`、`NHWC`或`NCHW`的随路格式转换。其中`NC1HWC0`、`NHWC`和`NCHW`用于卷积输出特征图。
 * @param [in] atom 搬运原子对象，可由`make_copy`接口构造。
 * @param [out] dst 目的张量，存储位置为`location::gm`。支持`ND`、`DN`、`NZ`、`NC1HWC0`、`NHWC`和`NCHW`数据格式。
 * @param [in] src 源张量，存储位置为`location::l0c`，数据格式为`NZ`，通常为`mmad`的计算结果。
 * @param [in] quant 可选量化参数。传入`uint64_t`时表示scalar量化参数，传入张量时表示tensor量化参数，张量位于L1 Buffer，元素类型为`uint64_t`。
 * @param [in] dst_coord 搬运区域在目的张量中的起始坐标，也可传入`zero_coord`。
 * @param [in] src_coord 搬运区域在源张量中的起始坐标，也可传入`zero_coord`。
 * @param [in] copy_shape 搬运区域的形状，用于指定搬运区域的大小。
 */
__aicore__ inline void copy(
    const copy_atom<Atom>& atom, const DstTensor& dst, const SrcTensor& src, const Quant& quant,
    const DstCoord& dst_coord, const SrcCoord& src_coord, const CopyShape& copy_shape);

/**
 * @brief Perform a region copy using the default atom inferred from the tensor memory locations.
 * @param dst : Destination tensor.
 * @param src : Source tensor.
 * @param dst_coord : Start coordinate in the destination tensor, or zero_coord.
 * @param src_coord : Start coordinate in the source tensor, or zero_coord.
 * @param copy_shape : Shape of the region to copy.
 */
template <
    typename DstTensor, typename SrcTensor, typename DstCoord, typename SrcCoord, typename CopyShape,
    Std::enable_if_t<
        is_attr_tensor_v<DstTensor> && is_attr_tensor_v<SrcTensor> && is_valid_coord_v<DstCoord> &&
            is_valid_coord_v<SrcCoord> && is_valid_shape_v<CopyShape>,
        int>
        Enable>
/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.vector_compute_load
 * @override_id copy__RCpDstTensor_RCpSrcTensor_RCpDstCoord_RCpSrcCoord_RCpCopyShape
 * @archs 3510
 * @brief Tensor API通过`copy`接口统一执行不同通路数据搬运。该接口用于将Global Memory中的数据搬运到L1 Buffer。`copy`接口根据源张量和目的张量的存储位置、数据类型和布局选择具体搬运实现。搬运块数、搬运长度、源/目的侧步长以及格式转换相关信息由Tensor布局推导，用户不需要在`copy`调用中额外传入搬运参数。
 * @param [out] dst 目的张量，存储位置为`location::l1`。
 * @param [in] src 源张量，存储位置为`location::gm`。
 * @param [in] dst_coord 搬运区域在目的张量中的起始坐标，也可传入`zero_coord`。
 * @param [in] src_coord 搬运区域在源张量中的起始坐标，也可传入`zero_coord`。
 * @param [in] copy_shape 搬运区域的形状，用于指定搬运区域的大小。
 */
__aicore__ inline void copy(
    const DstTensor& dst, const SrcTensor& src, const DstCoord& dst_coord, const SrcCoord& src_coord,
    const CopyShape& copy_shape);

/**
 * @brief Perform a quantized region copy using the default atom inferred from the tensor memory locations.
 * @param dst : Destination tensor.
 * @param src : Source tensor.
 * @param quant : Quantization parameter, which can be a scalar or a Tensor API tensor.
 * @param dst_coord : Start coordinate in the destination tensor, or zero_coord.
 * @param src_coord : Start coordinate in the source tensor, or zero_coord.
 * @param copy_shape : Shape of the region to copy.
 */
template <
    typename DstTensor, typename SrcTensor, typename QuantParam, typename DstCoord, typename SrcCoord,
    typename CopyShape,
    Std::enable_if_t<
        is_attr_tensor_v<DstTensor> && is_attr_tensor_v<SrcTensor> && is_valid_quant_v<QuantParam> &&
            is_valid_coord_v<DstCoord> && is_valid_coord_v<SrcCoord> && is_valid_shape_v<CopyShape>,
        int>
        Enable>
__aicore__ inline void copy(
    const DstTensor& dst, const SrcTensor& src, const QuantParam& quant, const DstCoord& dst_coord,
    const SrcCoord& src_coord, const CopyShape& copy_shape);

/**
 * @brief Construct a copy_atom from the copy operation object.
 * @param operation : Copy operation object.
 */
template <typename CopyOperation>
/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.vector_compute_load
 * @override_id make_copy__RCpCopyOperation
 * @archs 3510
 * @brief Tensor API通过`copy`接口统一执行不同通路数据搬运。该接口用于将Global Memory中的数据搬运到L1 Buffer。`copy`接口根据源张量和目的张量的存储位置、数据类型和布局选择具体搬运实现。搬运块数、搬运长度、源/目的侧步长以及格式转换相关信息由Tensor布局推导，用户不需要在`copy`调用中额外传入搬运参数。
 * @param [in] operation 搬运操作对象。Global Memory到L1 Buffer搬运取`copy_gm_to_l1{}`。
 */
__aicore__ inline constexpr auto make_copy(const CopyOperation& operation);

/**
 * @brief Construct a copy_atom from the copy operation object and trait object.
 * @param operation : Copy operation object.
 * @param trait : Copy trait object.
 */
template <typename CopyOperation, typename CopyTrait>
/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.vector_compute_load
 * @override_id make_copy__RCpCopyOperation_RCpCopyTrait
 * @archs 3510
 * @brief Tensor API通过`copy`接口统一执行不同通路数据搬运。该接口用于将Global Memory中的数据搬运到L1 Buffer。`copy`接口根据源张量和目的张量的存储位置、数据类型和布局选择具体搬运实现。搬运块数、搬运长度、源/目的侧步长以及格式转换相关信息由Tensor布局推导，用户不需要在`copy`调用中额外传入搬运参数。
 * @param [in] operation 搬运操作对象。Global Memory到L1 Buffer搬运取`copy_gm_to_l1{}`。
 * @param [in] trait 搬运trait对象。Global Memory到L1 Buffer默认取`gm_to_l1_trait_default{}`。
 */
__aicore__ inline constexpr auto make_copy(const CopyOperation& operation, const CopyTrait& trait);

} // namespace te
} // namespace asc

#endif // INCLUDE_TENSOR_API_ALGORITHM_COPY_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif
