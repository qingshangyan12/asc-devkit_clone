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
 * \brief Pointer overloads for Fixpipe / SetFixPipeConfig.
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/kernel_operator_fixpipe_ptr_impl.h is an internal header file and must not be used directly.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_PTR_IMPL_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_FIXPIPE_PTR_IMPL_H
#define ASCENDC_MODULE_OPERATOR_FIXPIPE_PTR_IMPL_H

#include "../../include/basic_api/kernel_struct_fixpipe.h"

namespace AscendC {

/* **************************************************************************************************
 * SetFixPipeConfig (pointer)
 * ************************************************************************************************* */
template <typename T, bool setRelu = false>
__aicore__ inline void SetFixPipeConfig(__cc__ T* preData, bool isUnitFlag = false)
{
    if ASCEND_IS_AIC {
        uint64_t config = 0;
        if constexpr (setRelu) {
            config = config | ((uint64_t)preData >> 6);
        } else {
            config = config | (((uint64_t)preData >> 7) << 8);
        }
        config = config | (static_cast<uint64_t>(isUnitFlag) << 63);
        set_fpc(config);
    }
}

template <typename T>
__aicore__ inline void SetFixPipeConfig(__cc__ T* reluPre, __cc__ T* quantPre, bool isUnitFlag = false)
{
    if ASCEND_IS_AIC {
        uint64_t config = 0;
        config = config | ((uint64_t)reluPre >> 6);
        config = config | (((uint64_t)quantPre >> 7) << 8);
        config = config | (static_cast<uint64_t>(isUnitFlag) << 63);
        set_fpc(config);
    }
}

/* **************************************************************************************************
 * Fixpipe (pointer) — V220 params
 * ************************************************************************************************* */
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3002 || __NPU_ARCH__ == 3003 || \
                              __NPU_ARCH__ == 3102 || __NPU_ARCH__ == 3113 || __NPU_ARCH__ == 3510 || \
                              __NPU_ARCH__ == 5102)
// Note: FixpipeParamsM300 / M310 are typedef aliases of FixpipeParamsV220.
template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(__cbuf__ T* dst, __cc__ U* src, const FixpipeParamsV220& intriParams)
{
    FixpipeL0C2L1Impl<T, U, config>(dst, src, intriParams);
}

template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(__ubuf__ T* dst, __cc__ U* src, const FixpipeParamsV220& intriParams)
{
    FixpipeL0C2UBImpl<T, U, config>(dst, src, intriParams);
}

template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    __cbuf__ T* dst, __cc__ U* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeL0C2L1Impl<T, U, config>(dst, src, cbufWorkspace, intriParams);
}

template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    __ubuf__ T* dst, __cc__ U* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeL0C2UBImpl<T, U, config>(dst, src, cbufWorkspace, intriParams);
}

template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(__gm__ T* dst, __cc__ U* src, const FixpipeParamsV220& intriParams)
{
    FixpipeL0C2GMImpl<T, U, config>(dst, src, intriParams);
}

template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    __gm__ T* dst, __cc__ U* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeL0C2GMImpl<T, U, config>(dst, src, cbufWorkspace, intriParams);
}
#endif

/* **************************************************************************************************
 * Fixpipe (pointer) — Arch3510 params
 * ************************************************************************************************* */
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(__cbuf__ T* dst, __cc__ U* src, const FixpipeParamsArch3510<config.format>& intriParams)
{
    FixpipeL0C2L1Impl<T, U, config>(dst, src, intriParams);
}

template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(__ubuf__ T* dst, __cc__ U* src, const FixpipeParamsArch3510<config.format>& intriParams)
{
    FixpipeL0C2UBImpl<T, U, config>(dst, src, intriParams);
}

template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    __cbuf__ T* dst, __cc__ U* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsArch3510<config.format>& intriParams)
{
    FixpipeL0C2L1Impl<T, U, config>(dst, src, cbufWorkspace, intriParams);
}

template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    __ubuf__ T* dst, __cc__ U* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsArch3510<config.format>& intriParams)
{
    FixpipeL0C2UBImpl<T, U, config>(dst, src, cbufWorkspace, intriParams);
}

template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(__gm__ T* dst, __cc__ U* src, const FixpipeParamsArch3510<config.format>& intriParams)
{
    FixpipeL0C2GMImpl<T, U, config>(dst, src, intriParams);
}

template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    __gm__ T* dst, __cc__ U* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsArch3510<config.format>& intriParams)
{
    FixpipeL0C2GMImpl<T, U, config>(dst, src, cbufWorkspace, intriParams);
}
#endif

} // namespace AscendC

#endif // ASCENDC_MODULE_OPERATOR_FIXPIPE_PTR_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_PTR_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_PTR_IMPL_H__
#endif
