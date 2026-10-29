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
 * \file kernel_operator_fixpipe_intf.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_operator_fixpipe_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_INTF_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_FIXPIPE_INTERFACE_H
#define ASCENDC_MODULE_OPERATOR_FIXPIPE_INTERFACE_H

#include "../../impl/basic_api/kernel_macros.h"
#include "kernel_struct_fixpipe.h"
#include "kernel_tensor.h"
#include "../../impl/basic_api/utils/kernel_utils_macros.h"

#include "../../impl/basic_api/utils/kernel_utils_ptr_overload.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "stub_def.h"
#endif

namespace AscendC {
/* **************************************************************************************************
 * Fixpipe                                             *
 * ************************************************************************************************* */
/*
 * @ingroup Fixpipe
 * @brief After calculation, process the results
 * @param [out] dst output GlobalTensor
 * @param [in] src input LocalTensor
 * @param [in] intriParams.cburstNum number of burst
 * @param [in] intriParams.burstLen burst length
 * @param [in] intriParams.srcStride src block stride
 * @param [in] intriParams.dstStride dst block stride
 * @param [in] intriParams.biasParams contains isBias flag and bias LocalTensor
 * @param [in] intriParams.quantParams contains quant mode and quant params
 * @param [in] intriParams.reluEn indicates whether to enable the relu function
 * @param [in] intriParams.nz2ndParams contains the input params for enable the nz2nd function
 */

template <typename T>
__aicore__ inline void SetFixPipeConfig(
    const LocalTensor<T>& reluPre, const LocalTensor<T>& quantPre, bool isUnitFlag = false);

template <typename T, bool setRelu = false>
__aicore__ inline void SetFixPipeConfig(const LocalTensor<T>& preData, bool isUnitFlag = false);

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5162) || (__NPU_ARCH__ == 9201) || \
    (__NPU_ARCH__ == 9202)
__aicore__ inline void SetFixpipeNz2ndFlag(uint16_t ndNum, uint16_t srcNdStride, uint32_t dstNdStride);
#else
__aicore__ inline void SetFixpipeNz2ndFlag(uint16_t ndNum, uint16_t srcNdStride, uint16_t dstNdStride);
#endif

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 9202)
template <typename T, bool enableCompareWithMaxBuf = false, bool enableMaxOut2UB = false, bool enableAbsInput = false>
__aicore__ inline void SetFixPipeMaxTrack(LocalTensor<T> rowOutput, LocalTensor<T> groupOutput, uint32_t groupN);
#endif

__aicore__ inline void SetFixpipePreQuantFlag(uint64_t config);

__aicore__ inline void SetFixPipeClipRelu(uint64_t config);

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5162)
template <typename T1, typename T2>
__aicore__ inline void SetFixPipeClipRelu(const FixpipeClipReluParams<T1, T2>& params);

__aicore__ inline void SetFixPipeConfig(uint64_t config);

template <typename T>
__aicore__ inline void SetFixpipePreQuantFlag(const FixPipePreQuantParams<T>& params);

__aicore__ inline void SetFixpipePostQuant(const FixPipePostQuantParams& params);

template <typename T>
__aicore__ inline void SetFixpipeEltAntiq(const FixPipeEltAntiqParams<T>& params);

__aicore__ inline void SetFixpipeEltwiseAddr(uint64_t config);

__aicore__ inline void SetFixpipeEltwiseAddr(const FixpipeEltwiseAddrParams& params);
#endif

template <typename T>
__aicore__ inline void SetFixPipeAddr(const LocalTensor<T>& eleWiseData, uint16_t c0ChStride);

#if defined(__NPU_ARCH__) &&                                                                         \
    (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113 || __NPU_ARCH__ == 3510 || \
     __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 9201 || __NPU_ARCH__ == 9202)
// L0C->L1
template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsV220& intriParams);

// L0C->L1 deq tensor quant
template <
    typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR, typename S = uint64_t,
    typename Std::enable_if<Std::is_same<PrimT<S>, uint64_t>::value, bool>::type = true>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<S>& cbufWorkspace,
    const FixpipeParamsV220& intriParams);

// L0C->GM
template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    const GlobalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsV220& intriParams);

// L0C->GM deq tensor quant
template <
    typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR, typename S = uint64_t,
    typename Std::enable_if<Std::is_same<PrimT<S>, uint64_t>::value, bool>::type = true>
__aicore__ inline void Fixpipe(
    const GlobalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<S>& cbufWorkspace,
    const FixpipeParamsV220& intriParams);
#endif

#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3002
// L0C->L1/UB
template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsM300& intriParams);

// L0C->L1/UB deq tensor quant
template <
    typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR, typename S = uint64_t,
    typename Std::enable_if<Std::is_same<PrimT<S>, uint64_t>::value, bool>::type = true>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<S>& cbufWorkspace,
    const FixpipeParamsM300& intriParams);

// L0C->GM
template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    const GlobalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsM300& intriParams);

// L0C->GM deq tensor quant
template <
    typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR, typename S = uint64_t,
    typename Std::enable_if<Std::is_same<PrimT<S>, uint64_t>::value, bool>::type = true>
__aicore__ inline void Fixpipe(
    const GlobalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<S>& cbufWorkspace,
    const FixpipeParamsM300& intriParams);
#elif defined(__NPU_ARCH__) && __NPU_ARCH__ == 3102
// L0C->L1/UB
template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsM310& intriParams);

// L0C->L1/UB deq tensor quant
template <
    typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR, typename S = uint64_t,
    typename Std::enable_if<Std::is_same<PrimT<S>, uint64_t>::value, bool>::type = true>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<S>& cbufWorkspace,
    const FixpipeParamsM310& intriParams);

// L0C->GM
template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    const GlobalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsM310& intriParams);

// L0C->GM deq tensor quant
template <
    typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR, typename S = uint64_t,
    typename Std::enable_if<Std::is_same<PrimT<S>, uint64_t>::value, bool>::type = true>
__aicore__ inline void Fixpipe(
    const GlobalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<S>& cbufWorkspace,
    const FixpipeParamsM310& intriParams);
#elif !defined(__NPU_ARCH__) || (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 9201) || (__NPU_ARCH__ == 5102)
// L0C->L1
template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsArch3510<config.format>& intriParams);

// L0C->L1/UB deq tensor quant
template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<uint64_t>& cbufWorkspace,
    const FixpipeParamsArch3510<config.format>& intriParams);

// L0C->GM
template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    const GlobalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsArch3510<config.format>& intriParams);

// L0C->GM deq tensor quant
template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    const GlobalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<uint64_t>& cbufWorkspace,
    const FixpipeParamsArch3510<config.format>& intriParams);
#elif (__NPU_ARCH__ == 5162)
// L1->UB,L1
template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsV516& intriParams);

// L1->GM
template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    const GlobalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsV516& intriParams);
#elif (__NPU_ARCH__ == 9202)
// L0C->L1
template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsArch9202<config.format>& intriParams);

// L0C->L1/UB deq tensor quant
template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<uint64_t>& cbufWorkspace,
    const FixpipeParamsArch9202<config.format>& intriParams);

// L0C->GM
template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    const GlobalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsArch9202<config.format>& intriParams);

// L0C->GM deq tensor quant
template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    const GlobalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<uint64_t>& cbufWorkspace,
    const FixpipeParamsArch9202<config.format>& intriParams);
#endif
/* ---- Fixpipe / SetFixPipeConfig with raw pointer operands -------------------- */
/*
 * Fixpipe with explicit element types: the pointer operands are deduced *after* the explicit
 * <T, U, config> parameters, which is the shape the corpus uses for the L0C -> GM / L1 / UB calls.
 * The address space of `dst` selects the destination pipe.
 */
template <
    typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR, typename DstPtr, typename SrcPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type = true>
__aicore__ inline void Fixpipe(DstPtr dst, SrcPtr src, const FixpipeParamsArch3510<config.format>& intriParams);

template <
    typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR, typename DstPtr, typename SrcPtr,
    typename WorkspacePtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr, WorkspacePtr>::value, bool>::type = true>
__aicore__ inline void Fixpipe(
    DstPtr dst, SrcPtr src, WorkspacePtr cbufWorkspace, const FixpipeParamsArch3510<config.format>& intriParams);

template <
    typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR, typename DstPtr, typename SrcPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type = true>
__aicore__ inline void Fixpipe(DstPtr dst, SrcPtr src, const FixpipeParamsV220& intriParams);

template <
    typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR, typename DstPtr, typename SrcPtr,
    typename WorkspacePtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr, WorkspacePtr>::value, bool>::type = true>
__aicore__ inline void Fixpipe(
    DstPtr dst, SrcPtr src, WorkspacePtr cbufWorkspace, const FixpipeParamsV220& intriParams);

/*
 * Fixpipe without explicit element types.  The element types are re-deduced from the address-space
 * qualified pointer parameters of the CubePtrOverload::Fp helpers below; `config` is deduced from
 * the FixpipeParamsArch3510<> parameter, whose CO2Layout is carried in the config constant.
 */
template <
    typename DstPtr, typename SrcPtr, const FixpipeConfig& config = CFG_ROW_MAJOR,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type = true>
__aicore__ inline void Fixpipe(DstPtr dst, SrcPtr src, const FixpipeParamsArch3510<config.format>& intriParams);

template <
    typename DstPtr, typename SrcPtr, typename WorkspacePtr, const FixpipeConfig& config = CFG_ROW_MAJOR,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr, WorkspacePtr>::value, bool>::type = true>
__aicore__ inline void Fixpipe(
    DstPtr dst, SrcPtr src, WorkspacePtr cbufWorkspace, const FixpipeParamsArch3510<config.format>& intriParams);

template <
    typename DstPtr, typename SrcPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type = true>
__aicore__ inline void Fixpipe(DstPtr dst, SrcPtr src, const FixpipeParamsV220& intriParams);

template <
    typename DstPtr, typename SrcPtr, typename WorkspacePtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr, WorkspacePtr>::value, bool>::type = true>
__aicore__ inline void Fixpipe(
    DstPtr dst, SrcPtr src, WorkspacePtr cbufWorkspace, const FixpipeParamsV220& intriParams);

/*
 * @ingroup SetFixPipeAddr
 * @brief Set the fixpipe element-wise address from a raw pointer operand.
 * @param [in] eleWiseData element-wise data address, in L1 or UB; the address space selects the pipe
 * @param [in] c0ChStride stride of the C0 channel
 */
template <typename DstPtr, typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr>::value, bool>::type = true>
__aicore__ inline void SetFixPipeAddr(DstPtr eleWiseData, uint16_t c0ChStride);

/*
 * @ingroup SetFixPipeConfig
 * @brief Set the fixpipe pre-quant or relu address from a raw pointer operand.
 * @param [in] preData pre-quant (default) or relu address; `setRelu` selects which FPC field it is
 *                   written into, exactly as in SetFixPipeConfigImpl
 * @param [in] isUnitFlag unit-flag bit written into FPC[63]
 *
 * Unlike the LocalTensor form there is no element-type parameter: a raw pointer already names its
 * pointee, so an explicit `T` could not be deduced from the argument and would have forced callers
 * to spell it out.  Leaving only the defaulted `setRelu` ahead of the deduced pointer means
 * `SetFixPipeConfig(ptr, true)` binds *this* overload -- with the bool as `isUnitFlag` -- instead of
 * sliding into the two-operand form below, where it would have been read as a quant-tensor address.
 */
template <
    bool setRelu = false, typename DstPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr>::value, bool>::type = true>
__aicore__ inline void SetFixPipeConfig(DstPtr preData, bool isUnitFlag = false);
/*
 * @ingroup SetFixPipeConfig
 * @brief Set both the relu and the quant pre-tensor addresses from raw pointer operands.
 * @param [in] reluPre relu pre-tensor address, written into FPC[7:0]
 * @param [in] quantPre quant pre-tensor address, written into FPC[15:8]
 * @param [in] isUnitFlag unit-flag bit written into FPC[63]
 *
 * Mirrors the Tensor overload
 * SetFixPipeConfig(const LocalTensor<T>& reluPre, const LocalTensor<T>& quantPre, bool).
 *
 * Both operands must themselves be raw pointers (AllPtr, not AnyPtr): the two addresses go to
 * different FPC fields, so a non-pointer operand -- a bool, an integer -- would otherwise be
 * accepted in this slot and then stored as an address.
 */
template <
    typename ReluPtr, typename QuantPtr,
    typename Std::enable_if<CubePtrOverload::AllPtr<ReluPtr, QuantPtr>::value, bool>::type = true>
__aicore__ inline void SetFixPipeConfig(ReluPtr reluPre, QuantPtr quantPre, bool isUnitFlag = false);

} // namespace AscendC

#if defined(__NPU_ARCH__)
#include "../../impl/basic_api/kernel_operator_fixpipe_intf_impl.h"
#endif
#endif // ASCENDC_MODULE_OPERATOR_FIXPIPE_INTERFACE_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_INTF_H__
#endif
