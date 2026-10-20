/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file kernel_operator_vec_gather_mask_intf.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_operator_vec_gather_mask_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_GATHER_MASK_INTF_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_VEC_REDUCEV2_INTERFACE_H
#define ASCENDC_MODULE_OPERATOR_VEC_REDUCEV2_INTERFACE_H

#include "../../impl/basic_api/kernel_macros.h"
#include "kernel_tensor.h"
#include "../../impl/basic_api/utils/kernel_vector_operand.h"
#include "kernel_struct_gather.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "stub_def.h"
#endif

namespace AscendC {
#pragma begin_pipe(V)
template <typename T, typename U, GatherMaskMode mode = defaultGatherMaskMode>
__aicore__ inline void GatherMask(
    const LocalTensor<T>& dst, const LocalTensor<T>& src0, const LocalTensor<U>& src1Pattern, const bool reduceMode,
    const uint32_t mask, const GatherMaskParams& gatherMaskParams, uint64_t& rsvdCnt);

template <
    typename T = VectorOperand::DefaultType, typename U = VectorOperand::DefaultType,
    GatherMaskMode mode = defaultGatherMaskMode, typename Operand0, typename Operand1, typename Operand2,
    typename Std::enable_if<
        VectorOperand::HasPointer<Operand0, Operand1, Operand2>() &&
            VectorOperand::Matches<VectorOperand::ElementT<T, Operand0>, Operand0>() &&
            VectorOperand::IsWritable<Operand0>() &&
            VectorOperand::Matches<VectorOperand::ElementT<T, Operand0>, Operand1>() &&
            VectorOperand::Matches<VectorOperand::ElementT<U, Operand2>, Operand2>(),
        int>::type = 0>
__aicore__ inline void GatherMask(
    Operand0 dst, Operand1 src0, Operand2 src1Pattern, const bool reduceMode, const uint32_t mask,
    const GatherMaskParams& gatherMaskParams, uint64_t& rsvdCnt);

template <typename T, GatherMaskMode mode = defaultGatherMaskMode>
__ASC_USE_RESERVED_UBUF__(
    3510, "GatherMask is forbidden when compile option --cce-disable-asc-reserved-ubuf is enabled")
__aicore__ inline void GatherMask(
    const LocalTensor<T>& dst, const LocalTensor<T>& src0, const uint8_t src1Pattern, const bool reduceMode,
    const uint32_t mask, const GatherMaskParams& gatherMaskParams, uint64_t& rsvdCnt);

template <
    typename T = VectorOperand::DefaultType, GatherMaskMode mode = defaultGatherMaskMode, typename Operand0,
    typename Operand1,
    typename Std::enable_if<
        VectorOperand::HasPointer<Operand0, Operand1>() &&
            VectorOperand::Matches<VectorOperand::ElementT<T, Operand0>, Operand0>() &&
            VectorOperand::IsWritable<Operand0>() &&
            VectorOperand::Matches<VectorOperand::ElementT<T, Operand0>, Operand1>(),
        int>::type = 0>
__ASC_USE_RESERVED_UBUF__(
    3510, "GatherMask is forbidden when compile option --cce-disable-asc-reserved-ubuf is enabled")
__aicore__ inline void GatherMask(
    Operand0 dst, Operand1 src0, const uint8_t src1Pattern, const bool reduceMode, const uint32_t mask,
    const GatherMaskParams& gatherMaskParams, uint64_t& rsvdCnt);
#pragma end_pipe
} // namespace AscendC
#if defined(__NPU_ARCH__)
#include "../../impl/basic_api/kernel_operator_vec_gather_mask_intf_impl.h"
#endif
#endif // ASCENDC_MODULE_OPERATOR_VEC_REDUCEV2_INTERFACE_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_GATHER_MASK_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_GATHER_MASK_INTF_H__
#endif
