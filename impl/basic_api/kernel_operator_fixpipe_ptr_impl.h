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
 * \file kernel_operator_fixpipe_ptr_impl.h
 * \brief Pointer-path entry points for the CUBE Fixpipe family.
 *
 *        These overloads are strictly additive: they take bare hardware pointers instead of
 *        GlobalTensor / LocalTensor and forward to the same FixpipeL0C2*Impl implementation the Tensor
 *        path uses. The Tensor overloads are left untouched, so the Tensor path is unchanged.
 *
 *        The address space of the destination operand selects the hardware target in exactly the way
 *        LocalTensor::GetPosition() does on the Tensor path:
 *            __cbuf__ -> L1 Buffer (C1)   __ubuf__ -> Unified Buffer   __gm__ -> Global Memory
 *        and the source operand of every Fixpipe form is an L0C (__cc__) pointer.
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/kernel_operator_fixpipe_ptr_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_fixpipe_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_PTR_IMPL_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_FIXPIPE_PTR_IMPL_H
#define ASCENDC_MODULE_OPERATOR_FIXPIPE_PTR_IMPL_H

#include "utils/kernel_utils_underlying_ptr.h"
#include "../../include/basic_api/kernel_struct_fixpipe.h"

namespace AscendC {

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113 || \
                              __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102)
/* **************************************************************************************************
 * Fixpipe (FixpipeParamsV220), pointer path
 * ************************************************************************************************* */
/* L0C (__cc__) -> L1 (__cbuf__) */
template <typename DstT, typename SrcT, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(__cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeL0C2L1Impl<DstT, SrcT, config>(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), intriParams);
}

/* L0C (__cc__) -> UB (__ubuf__) */
template <typename DstT, typename SrcT, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(__ubuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeL0C2UBImpl<DstT, SrcT, config>(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), intriParams);
}

/* L0C (__cc__) -> GM (__gm__) */
template <typename DstT, typename SrcT, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(__gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeL0C2GMImpl<DstT, SrcT, config>(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), intriParams);
}

/* L0C (__cc__) -> L1 (__cbuf__), deq tensor quant */
template <typename DstT, typename SrcT, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    __cbuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeL0C2L1Impl<DstT, SrcT, config>(
        GetUnderlyingPtr(dst), GetUnderlyingPtr(src), GetUnderlyingPtr(cbufWorkspace), intriParams);
}

/* L0C (__cc__) -> UB (__ubuf__), deq tensor quant */
template <typename DstT, typename SrcT, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    __ubuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeL0C2UBImpl<DstT, SrcT, config>(
        GetUnderlyingPtr(dst), GetUnderlyingPtr(src), GetUnderlyingPtr(cbufWorkspace), intriParams);
}

/* L0C (__cc__) -> GM (__gm__), deq tensor quant */
template <typename DstT, typename SrcT, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    __gm__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeL0C2GMImpl<DstT, SrcT, config>(
        GetUnderlyingPtr(dst), GetUnderlyingPtr(src), GetUnderlyingPtr(cbufWorkspace), intriParams);
}
#endif

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
/* **************************************************************************************************
 * Fixpipe (FixpipeParamsArch3510), pointer path
 * ************************************************************************************************* */
/* L0C (__cc__) -> L1 (__cbuf__) */
template <typename DstT, typename SrcT, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    __cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsArch3510<config.format>& intriParams)
{
    FixpipeL0C2L1Impl<DstT, SrcT, config>(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), intriParams);
}

/* L0C (__cc__) -> UB (__ubuf__) */
template <typename DstT, typename SrcT, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    __ubuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsArch3510<config.format>& intriParams)
{
    FixpipeL0C2UBImpl<DstT, SrcT, config>(GetUnderlyingPtr(dst), GetUnderlyingPtr(src), intriParams);
}

/* L0C (__cc__) -> GM (__gm__) */
template <typename DstT, typename SrcT, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    __gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsArch3510<config.format>& intriParams)
{
    FixpipeL0C2GMImpl<DstT, SrcT, config>(
        GetUnderlyingPtr(dst), GetUnderlyingPtr(src), intriParams, ExtractCacheMode(GetUnderlyingPtr(dst)));
}

/* L0C (__cc__) -> L1 (__cbuf__), deq tensor quant */
template <typename DstT, typename SrcT, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    __cbuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsArch3510<config.format>& intriParams)
{
    FixpipeL0C2L1Impl<DstT, SrcT, config>(
        GetUnderlyingPtr(dst), GetUnderlyingPtr(src), GetUnderlyingPtr(cbufWorkspace), intriParams);
}

/* L0C (__cc__) -> UB (__ubuf__), deq tensor quant */
template <typename DstT, typename SrcT, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    __ubuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsArch3510<config.format>& intriParams)
{
    FixpipeL0C2UBImpl<DstT, SrcT, config>(
        GetUnderlyingPtr(dst), GetUnderlyingPtr(src), GetUnderlyingPtr(cbufWorkspace), intriParams);
}

/* L0C (__cc__) -> GM (__gm__), deq tensor quant */
template <typename DstT, typename SrcT, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    __gm__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsArch3510<config.format>& intriParams)
{
    FixpipeL0C2GMImpl<DstT, SrcT, config>(
        GetUnderlyingPtr(dst), GetUnderlyingPtr(src), GetUnderlyingPtr(cbufWorkspace), intriParams,
        ExtractCacheMode(GetUnderlyingPtr(dst)));
}
#endif

/* **************************************************************************************************
 * SetFixPipeConfig, pointer path: programs the FPC register from bare hardware pointers
 * ************************************************************************************************* */
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102 || \
                              __NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || \
                              __NPU_ARCH__ == 5163)
/* reluPre / quantPre pair form */
template <typename T>
__aicore__ inline void SetFixPipeConfig(__cc__ T* reluPre, __cc__ T* quantPre, bool isUnitFlag = false)
{
    SetFixPipeFpcCal(
        static_cast<uint64_t>(reinterpret_cast<uint64_t>(GetUnderlyingPtr(reluPre))),
        static_cast<uint64_t>(reinterpret_cast<uint64_t>(GetUnderlyingPtr(quantPre))), isUnitFlag);
}

/* single pre-data form; setRelu selects the ReluPre field instead of the QuantPre field of the FPC */
template <typename T, bool setRelu = false>
__aicore__ inline void SetFixPipeConfig(__cc__ T* preData, bool isUnitFlag = false)
{
    SetFixPipeFpcCal<setRelu>(
        static_cast<uint64_t>(reinterpret_cast<uint64_t>(GetUnderlyingPtr(preData))), isUnitFlag);
}
#endif

} // namespace AscendC

#endif // ASCENDC_MODULE_OPERATOR_FIXPIPE_PTR_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_PTR_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_PTR_IMPL_H__
#endif