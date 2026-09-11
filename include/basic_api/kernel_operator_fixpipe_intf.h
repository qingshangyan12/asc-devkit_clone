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
/**
 * @ingroup simd_api.basic_api.cube_compute_isasi.cube_store_aux_config
 * @override_id SetFixPipeConfig__RCltg1x2_pT_RCltg1x2_pT_b
 * @archs 3510 2201 3002 x90 9030
 * @brief DataCopy数据搬运（[L0C Buffer->GM](../cube_compute_store/Fixpipe_L0CToGM.md)、[L0C Buffer->L1 Buffer](../cube_compute_store/Fixpipe_L0CToL1.md)）过程中进行随路量化时，通过调用该接口设置量化的参数。
 */
__aicore__ inline void SetFixPipeConfig(
    const LocalTensor<T>& reluPre, const LocalTensor<T>& quantPre, bool isUnitFlag = false);

template <typename T, bool setRelu = false>
/**
 * @ingroup simd_api.basic_api.cube_compute_isasi.cube_store_aux_config
 * @override_id SetFixPipeConfig__RCltg1x2_pT_b
 * @archs 3510 2201 3002 x90 9030
 * @brief DataCopy数据搬运（[L0C Buffer->GM](../cube_compute_store/Fixpipe_L0CToGM.md)、[L0C Buffer->L1 Buffer](../cube_compute_store/Fixpipe_L0CToL1.md)）过程中进行随路量化时，通过调用该接口设置量化的参数。
 */
__aicore__ inline void SetFixPipeConfig(const LocalTensor<T>& preData, bool isUnitFlag = false);

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
/**
 * @ingroup simd_api.basic_api.cube_compute_isasi.cube_store_aux_config
 * @override_id SetFixpipeNz2ndFlag__u16_u16_u32
 * @archs 3510
 * @brief DataCopy数据搬运（[L0C Buffer->GM](../cube_compute_store/Fixpipe_L0CToGM.md)）过程中进行随路格式转换（NZ格式转换为ND格式）时，通过调用该接口设置格式转换的相关配置。
 */
__aicore__ inline void SetFixpipeNz2ndFlag(uint16_t ndNum, uint16_t srcNdStride, uint32_t dstNdStride);
#else
/**
 * @ingroup simd_api.basic_api.cube_compute_isasi.cube_store_aux_config
 * @override_id SetFixpipeNz2ndFlag__u16_u16_u16
 * @archs 3510 2201 3002 x90 9030
 * @brief DataCopy数据搬运（[L0C Buffer->GM](../cube_compute_store/Fixpipe_L0CToGM.md)）过程中进行随路格式转换（NZ格式转换为ND格式）时，通过调用该接口设置格式转换的相关配置。
 */
__aicore__ inline void SetFixpipeNz2ndFlag(uint16_t ndNum, uint16_t srcNdStride, uint16_t dstNdStride);
#endif

/**
 * @ingroup simd_api.basic_api.cube_compute_isasi.cube_store_aux_config
 * @override_id SetFixpipePreQuantFlag__u64
 * @archs 3510 2201 3002 x90 9030
 * @brief DataCopy数据搬运（[L0C Buffer->GM](../cube_compute_store/Fixpipe_L0CToGM.md)、[L0C Buffer->L1 Buffer](../cube_compute_store/Fixpipe_L0CToL1.md)）过程中进行随路量化时，通过调用该接口设置scalar量化参数。
 */
__aicore__ inline void SetFixpipePreQuantFlag(uint64_t config);

/**
 * @ingroup simd_api.basic_api.cube_compute_isasi.cube_store_aux_config
 * @override_id SetFixPipeClipRelu__u64
 * @archs 3002 x90 9030
 * @brief DataCopy数据搬运（[L0C Buffer->GM](../cube_compute_store/Fixpipe_L0CToGM.md)）过程中进行随路量化后，通过调用该接口设置ClipReLU操作的最大值。
 */
__aicore__ inline void SetFixPipeClipRelu(uint64_t config);

template <typename T>
/**
 * @ingroup simd_api.basic_api.cube_compute_isasi.cube_store_aux_config
 * @override_id SetFixPipeAddr__RCltg1x2_pT_u16
 * @archs 3002
 * @brief DataCopy数据搬运（[L0C Buffer->GM](../cube_compute_store/Fixpipe_L0CToGM.md)）过程中进行随路量化后，通过调用该接口设置Elementwise操作时LocalTensor的地址。
 */
__aicore__ inline void SetFixPipeAddr(const LocalTensor<T>& eleWiseData, uint16_t c0ChStride);

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113 || \
                              __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102)
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
#elif !defined(__NPU_ARCH__) || (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
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
#endif
} // namespace AscendC

#if defined(__NPU_ARCH__)
#include "../../impl/basic_api/kernel_operator_fixpipe_intf_impl.h"
#endif
#endif // ASCENDC_MODULE_OPERATOR_FIXPIPE_INTERFACE_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_INTF_H__
#endif
