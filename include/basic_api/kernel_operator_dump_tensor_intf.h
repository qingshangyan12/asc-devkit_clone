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
 * \file kernel_operator_dump_tensor_intf.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_operator_dump_tensor_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DUMP_TENSOR_INTF_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_DUMP_TENSOR_INTERFACE_H
#define ASCENDC_MODULE_OPERATOR_DUMP_TENSOR_INTERFACE_H

#include "../../impl/basic_api/kernel_macros.h"
#include "kernel_tensor.h"
#include "../../impl/basic_api/kernel_log.h"
#include "../../impl/basic_api/utils/kernel_utils_ptr_overload.h"
#include "utils/debug/asc_assert.h"
#include "impl/utils/debug/asc_aicore_printf_impl.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "stub_def.h"
#endif

namespace AscendC {
template <typename T>
__aicore__ inline void DumpTensor(const LocalTensor<T>& tensor, uint32_t desc, uint32_t dumpSize);
template <typename T>
__aicore__ inline void DumpTensor(const GlobalTensor<T>& tensor, uint32_t desc, uint32_t dumpSize);
template <typename T>
__aicore__ inline void DumpTensor(
    const LocalTensor<T>& tensor, uint32_t desc, uint32_t dumpSize, const ShapeInfo& shapeInfo);
template <typename T>
__aicore__ inline void DumpTensor(
    const GlobalTensor<T>& tensor, uint32_t desc, uint32_t dumpSize, const ShapeInfo& shapeInfo);
template <typename T>
__aicore__ inline void DumpAccChkPoint(
    const LocalTensor<T>& tensor, uint32_t index, uint32_t countOff, uint32_t dumpSize);
template <typename T>
__aicore__ inline void DumpAccChkPoint(
    const GlobalTensor<T>& tensor, uint32_t index, uint32_t countOff, uint32_t dumpSize);

/*
 * DumpTensor / DumpAccChkPoint with a raw GM pointer.  The GM address is re-wrapped as the
 * GlobalTensor view it denotes, so the dump FIFO and the entity selection are exactly the Tensor
 * path.
 */
template <typename T>
__aicore__ inline void DumpTensor(__gm__ T* tensor, uint32_t desc, uint32_t dumpSize);
template <typename T>
__aicore__ inline void DumpTensor(__gm__ T* tensor, uint32_t desc, uint32_t dumpSize, const ShapeInfo& shapeInfo);
template <typename T>
__aicore__ inline void DumpAccChkPoint(__gm__ T* tensor, uint32_t index, uint32_t countOff, uint32_t dumpSize);

/*
 * The on-chip roles, with the role named by an explicit TPosition template argument.
 *
 * WHY THE POSITION IS A TEMPLATE ARGUMENT RATHER THAN THE PARAMETER ADDRESS SPACE: under
 * ASCENDC_CPU_DEBUG the platform defines __ubuf__ / __cbuf__ / __cc__ / __gm__ as *empty* macros
 * (tikicpulib/include/stub_def.h), so one overload per address space would collapse into the raw GM
 * declaration above -- a redefinition -- and a call site collapses with them.  The delivered tests
 * for this family are compiled in exactly that build, so an address-space-qualified form could
 * neither be declared nor called there, and any "coverage" of it would silently bind to the GM
 * overload instead.  Naming the position keeps ONE signature that both builds can compile and call,
 * which is what makes the call coverage real.  The bit-mode LoadData pointer forms make the same
 * trade, keeping Dst and Src explicit because neither is deducible from the arguments.
 *
 * A previous revision of this header stated that the UB / L1 / CO1 roles have no pointer form "by
 * design", on the grounds that the dump path needs a position and buffer metadata that a raw
 * pointer cannot carry.  That does not hold: the impl reads only GetPosition() -- which the caller
 * now supplies -- and DumpAccChkPoint extent, which the GM form above already handles by declaring
 * an unbounded one.  The view is built with the same TBuffAddr construction that
 * CubePtrOverload::LocalTensorFromPtr and ::UbTensorOver already use for ReadSpmBuffer and the
 * synchronisation APIs -- the tool for exactly this round trip already shipped with them.
 *
 * The BIAS role of the impl (asc_dump on __biasbuf__) is not offered: __biasbuf__ is not a usable
 * CCE memory attribute in this compiler, so no caller could name it.
 */
template <
    TPosition position, typename T, typename DstPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr>::value, bool>::type = true>
__aicore__ inline void DumpTensor(DstPtr tensor, uint32_t desc, uint32_t dumpSize);
template <
    TPosition position, typename T, typename DstPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr>::value, bool>::type = true>
__aicore__ inline void DumpTensor(DstPtr tensor, uint32_t desc, uint32_t dumpSize, const ShapeInfo& shapeInfo);
template <
    TPosition position, typename T, typename DstPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr>::value, bool>::type = true>
__aicore__ inline void DumpAccChkPoint(DstPtr tensor, uint32_t index, uint32_t countOff, uint32_t dumpSize);

using __asc_aicore::printf;
using __asc_aicore::PRINTF;
} // namespace AscendC

#include "../../impl/basic_api/kernel_operator_dump_tensor_intf_impl.h"
#endif // END OF ASCENDC_MODULE_OPERATOR_DUMP_TENSOR_INTERFACE_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DUMP_TENSOR_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DUMP_TENSOR_INTF_H__
#endif
