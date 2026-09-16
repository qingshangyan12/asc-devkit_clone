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
 * \file kernel_utils_ptr_overload.h
 * \brief Shared helpers for the raw-pointer overloads of the Basic API.
 *
 * Kept in its own header on purpose: both kernel_operator_mm_intf.h and
 * kernel_operator_fixpipe_intf.h need these traits, and making one interface header include the other
 * would move the point at which that header is first pulled into a translation unit.  The CUBE
 * interface headers are included more than once per translation unit with different __NPU_ARCH__
 * settings, so that reordering is observable: an interface header that is first reached too early
 * has its include guard set before __NPU_ARCH__ is available, and the implementation header it
 * includes at its end is then skipped for the whole translation unit.
 */
#ifndef ASCENDC_MODULE_OPERATOR_UTILS_PTR_OVERLOAD_H
#define ASCENDC_MODULE_OPERATOR_UTILS_PTR_OVERLOAD_H

#include "kernel_tensor.h"
#include "../kernel_tensor_base.h"

namespace AscendC {
namespace CubePtrOverload {
// True when at least one of the given operands is a raw pointer.  Address-space qualifiers such as
// __ca__ qualify the pointee and do not hide the pointer-ness, so the standard trait applies as-is;
// LocalTensor / GlobalTensor, by contrast, are class types and are never seen as pointers.
template <typename... Ts> struct AnyPtr;
template <typename T> struct AnyPtr<T> : public Std::is_pointer<T> {};
template <typename T, typename... Rest>
struct AnyPtr<T, Rest...>
    : public Std::integral_constant<bool, Std::is_pointer<T>::value || AnyPtr<Rest...>::value> {};
/*
 * The LocalTensor view a raw UB address denotes.
 *
 * ReadSpmBuffer's backend only ever reads a tensor's address, so a pointer call can reuse the
 * Tensor path verbatim once that address is re-wrapped.  The round trip is exact by construction:
 * on device LocalTensor::GetPhyAddr() returns address_.bufferAddr, which is what a __ubuf__ pointer
 * already encodes, and under ASCENDC_CPU_DEBUG it returns address_.absAddr, which is the host
 * pointer the caller passed.  The position is a template parameter because a raw address does not
 * say which on-chip buffer it belongs to -- each overload that uses this fixes it.
 */
template <TPosition position, typename T>
__aicore__ inline LocalTensor<T> LocalTensorFromPtr(__ubuf__ T* ptr)
{
    TBuffAddr bufferAddr;
    bufferAddr.dataLen = 0;
    bufferAddr.bufferHandle = nullptr;
    bufferAddr.logicPos = static_cast<uint8_t>(position);
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
    bufferAddr.bufferAddr = 0;
    bufferAddr.absAddr = reinterpret_cast<uint8_t*>(ptr);
#else
    bufferAddr.bufferAddr = static_cast<uint32_t>(reinterpret_cast<uint64_t>(ptr));
#endif
    LocalTensor<T> tensor;
    tensor.SetAddr(bufferAddr);
    return tensor;
}
}  // namespace CubePtrOverload
}  // namespace AscendC

#endif  // ASCENDC_MODULE_OPERATOR_UTILS_PTR_OVERLOAD_H
