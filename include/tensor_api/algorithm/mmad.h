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
 * \file mmad.h
 * \brief
 */
#ifndef INCLUDE_TENSOR_API_ALGORITHM_MMAD_H
#define INCLUDE_TENSOR_API_ALGORITHM_MMAD_H

#include "tensor_api/tensor/layout_interface.h"
#include "tensor_api/tensor/pointer.h"
#include "tensor_api/atom/cube/mmad_atom.h"
#include "impl/tensor_api/algorithm/mmad_impl.h"

namespace asc {
namespace te {

/**
 * @brief Perform matrix multiplication with a preconstructed mmad_atom.
 * @param atom : Matrix multiplication atom object.
 * @param c : Destination tensor.
 * @param a : Left input tensor.
 * @param b : Right input tensor.
 */
template <typename Atom, typename CTensor, typename ATensor, typename BTensor>
/**
 * @ingroup simd_api.tensor_api.matrix_compute.mmad_compute
 * @override_id mmad__RCmmadatomg1x5_pAtom_RCpCTensor_RCpATensor_RCpBTensor
 * @archs 3510
 * @brief `mmad`接口用于完成L0A Buffer上左矩阵A和L0B Buffer上右矩阵B的矩阵乘加，结果写入L0C Buffer上结果矩阵C。默认模式为普通矩阵计算。
 * @param [in] atom 矩阵计算原子对象，可由`make_mmad`接口构造。
 * @param [out] c 结果矩阵C，存储位置为`location::l0c`，数据格式为NZ。
 * @param [in] a 左矩阵A，存储位置为`location::l0a`，数据格式为NZ。
 * @param [in] b 右矩阵B，存储位置为`location::l0b`，数据格式为ZN。
 */
__aicore__ inline void mmad(const mmad_atom<Atom>& atom, const CTensor& c, const ATensor& a, const BTensor& b);

/**
 * @brief Perform matrix multiplication with a preconstructed mmad_atom and a bias tensor.
 * @param atom : Matrix multiplication atom object.
 * @param c : Destination tensor.
 * @param a : Left input tensor.
 * @param b : Right input tensor.
 * @param bias : Bias tensor.
 */
template <
    typename Atom, typename CTensor, typename ATensor, typename BTensor, typename BiasTensor,
    Std::enable_if_t<is_attr_tensor_v<BiasTensor>, int> Enable>
/**
 * @ingroup simd_api.tensor_api.matrix_compute.mmad_compute
 * @override_id mmad__RCmmadatomg1x5_pAtom_RCpCTensor_RCpATensor_RCpBTensor_RCpBiasTensor
 * @archs 3510
 * @brief `mmad`接口用于完成L0A Buffer上左矩阵A和L0B Buffer上右矩阵B的矩阵乘加，结果写入L0C Buffer上结果矩阵C。默认模式为普通矩阵计算。
 * @param [in] atom 矩阵计算原子对象，可由`make_mmad`接口构造。
 * @param [out] c 结果矩阵C，存储位置为`location::l0c`，数据格式为NZ。
 * @param [in] a 左矩阵A，存储位置为`location::l0a`，数据格式为NZ。
 * @param [in] b 右矩阵B，存储位置为`location::l0b`，数据格式为ZN。
 * @param [in] bias bias张量，存储位置为`location::bias`，数据格式为ND。
 */
__aicore__ inline void mmad(
    const mmad_atom<Atom>& atom, const CTensor& c, const ATensor& a, const BTensor& b, const BiasTensor& bias);

/**
 * @brief Construct a mmad_atom from the matrix multiplication operation object.
 * @param operation : Matrix multiplication operation object.
 */
template <typename MmadOperation>
/**
 * @ingroup simd_api.tensor_api.matrix_compute.mmad_compute
 * @override_id make_mmad__RCpMmadOperation
 * @archs 3510
 * @brief `mmad`接口用于完成L0A Buffer上左矩阵A和L0B Buffer上右矩阵B的矩阵乘加，结果写入L0C Buffer上结果矩阵C。默认模式为普通矩阵计算。
 * @param [in] operation 矩阵计算操作对象，通过`mmad_operation{}`构造。
 */
__aicore__ inline constexpr auto make_mmad(const MmadOperation& operation);

/**
 * @brief Construct a mmad_atom from the matrix multiplication operation object and trait object.
 * @param operation : Matrix multiplication operation object.
 * @param trait : Matrix multiplication trait object.
 */
template <typename MmadOperation, typename MmadTrait>
/**
 * @ingroup simd_api.tensor_api.matrix_compute.mmad_compute
 * @override_id make_mmad__RCpMmadOperation_RCpMmadTrait
 * @archs 3510
 * @brief `mmad`接口用于完成L0A Buffer上左矩阵A和L0B Buffer上右矩阵B的矩阵乘加，结果写入L0C Buffer上结果矩阵C。默认模式为普通矩阵计算。
 * @param [in] operation 矩阵计算操作对象，通过`mmad_operation{}`构造。
 * @param [in] trait 矩阵计算Trait对象，用于指定矩阵计算静态特性。默认使用`mmad_trait_default{}`。
 */
__aicore__ inline constexpr auto make_mmad(const MmadOperation& operation, const MmadTrait& trait);

} // namespace te
} // namespace asc

#endif // INCLUDE_TENSOR_API_ALGORITHM_MMAD_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif
