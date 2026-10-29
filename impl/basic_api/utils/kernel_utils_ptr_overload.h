/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
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
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/utils/kernel_utils_ptr_overload.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_mm_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_UTILS_PTR_OVERLOAD_H__
#endif
#ifndef ASCENDC_MODULE_UTILS_PTR_OVERLOAD_H
#define ASCENDC_MODULE_UTILS_PTR_OVERLOAD_H

#include "kernel_tensor.h"
#include "../kernel_tensor_base.h"

namespace AscendC {
namespace CubePtrOverload {
// True when at least one of the given operands is a raw pointer.  Address-space qualifiers such as
// __ca__ qualify the pointee and do not hide the pointer-ness, so the standard trait applies as-is;
// LocalTensor / GlobalTensor, by contrast, are class types and are never seen as pointers.
template <typename... Ts>
struct AnyPtr;
template <typename T>
struct AnyPtr<T> : public Std::is_pointer<T> {};
template <typename T, typename... Rest>
struct AnyPtr<T, Rest...> : public Std::integral_constant<bool, Std::is_pointer<T>::value || AnyPtr<Rest...>::value> {};

/*
 * 仅当全部操作数都是裸指针时为真。用于「操作数所处位置本身有意义」的场合——例如
 * SetFixPipeConfig 的双操作数形，第二个实参写入的 FPC 字段与第一个不同。若该位置只用
 * AnyPtr（至少一个是裸指针）约束，那么 bool / 整数会被当作非指针操作数接受下来，
 * 随后在函数体内被转成 uint64_t 写进地址字段。
 */
template <typename... Ts>
struct AllPtr;
template <typename T>
struct AllPtr<T> : public Std::is_pointer<T> {};
template <typename T, typename... Rest>
struct AllPtr<T, Rest...> : public Std::integral_constant<bool, Std::is_pointer<T>::value && AllPtr<Rest...>::value> {};

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
/* Scalar access through a raw UB pointer.
 *
 * These two are the pointer-level equivalents of the LocalTensor accessors that carry no addressing
 * of their own -- their whole body is one dereference of the buffer address:
 *
 *   LocalTensor<T>::GetValue(index)
 *       -> *(reinterpret_cast<__ubuf__ PrimType*>(static_cast<uint64_t>(address_.bufferAddr)) + index)
 *   LocalTensor<T>::SetValue(index, value)
 *       -> the same address, assigned static_cast<PrimType>(value)
 *
 * (impl/basic_api/kernel_tensor_impl.h, the final `else` branch of each accessor.)  They reproduce
 * exactly those two statements for a primitive that is neither half-byte nor complex, which is the
 * case for every operand of the synchronisation APIs that use them (int32_t).  Note that the address
 * the accessors use is `address_.bufferAddr` reinterpreted as a pointer, which is the same quantity
 * a caller obtains from GetPhyAddr() -- so a raw pointer substituted here denotes the same address.
 *
 * NOT reproduced here, on purpose: both accessors are void for a UB operand on AIC
 * (`if ASCEND_IS_AIC { ... return PrimType(0); }` / `return;`).  A caller whose body can reach an
 * AIC path must apply that rule itself; see the comment at the IBSet / IBWait pointer overloads in
 * impl/basic_api/kernel_operator_block_sync_intf_impl.h.  It is not baked in here because the
 * determine-compute-sync pointer overloads are already inside `if ASCEND_IS_AIV`, where the rule is
 * provably unreachable.
 *
 * Not reproduced either: the `__inout_pipe__(S)` scheduling attribute on the two accessors.  It is a
 * compiler hint about which pipe performs the access and has no numeric effect; it is not attached
 * to these free functions.
 */
template <typename T>
__aicore__ inline T UbScalarRead(__ubuf__ T* addr, uint32_t index)
{
    return *(addr + index);
}

template <typename T>
__aicore__ inline void UbScalarWrite(__ubuf__ T* addr, uint32_t index, T value)
{
    *(addr + index) = value;
}
/* Tensor views over a raw pointer operand.
 *
 * These exist so that the raw-pointer overloads of the synchronisation APIs can hand the caller's
 * pointer to the *same* function the Tensor form calls, instead of re-expressing that function's
 * body.  A raw pointer is the same quantity the corresponding GetPhyAddr() returns, so building a
 * view over it adds no arithmetic and reinterprets nothing.
 *
 * Why a view rather than a re-expressed body: impl/basic_api/kernel_operator_block_sync_intf_impl.h
 * sits inside an include cycle (kernel_operator_data_copy_intf_impl.h -> kernel_operator_data_copy_
 * base_impl.h -> dav_<arch>/kernel_operator_data_copy_impl.h -> kernel_tpipe.h -> kernel_tpipe_impl.h ->
 * kernel_tquebind_impl.h -> include/basic_api/kernel_operator_block_sync_intf.h ->
 * kernel_operator_block_sync_intf_impl.h).  In a translation unit that enters the cycle from the
 * data-copy side, that last file is parsed while kernel_operator_data_copy_intf_impl.h is only a
 * third of the way through, so the pointer-level primitives declared further down in it
 * (DataCopyCheck at :713, and the arch DataCopyGM2UBImpl / DataCopyUB2GMImpl) are not yet visible.
 * Naming them there is therefore only valid in some include orders -- the single-header checker
 * (ascendc_run_all_header_checks_cpu_single) compiles exactly the orders where it is not.  A view
 * built from declarations that are already complete inside the cycle -- LocalTensor, GlobalTensor
 * and TBuffAddr, all from include/basic_api/kernel_tensor.h -- is order-independent.
 */

// A GlobalTensor view over a raw GM pointer.  SetGlobalBuffer re-seats the tensor on exactly the
// address it is given, which is the address GlobalTensor::GetPhyAddr() hands back to the caller in
// the first place, so the view denotes the same memory.
template <typename T, typename GmPtr>
__aicore__ inline GlobalTensor<T> GmTensorOver(GmPtr gmPtr)
{
    GlobalTensor<T> t;
    t.SetGlobalBuffer(reinterpret_cast<__gm__ PrimT<T>*>(gmPtr));
    return t;
}

// A LocalTensor view over a raw UB pointer, of `lenHint` elements.
//
// The validated LocalTensor(TPosition, offset, length) constructor is not usable here: on the device
// build it takes a buffer offset and a raw UB pointer obtained from GetPhyAddr() is exactly that
// offset, but under ASCENDC_CPU_DEBUG GetPhyAddr() returns an absolute host address and that
// constructor would add the platform base to it a second time.  A tensor's address is the TBuffAddr
// it holds, and each build fills the one field its own GetPhyAddr() reads -- bufferAddr on the
// device, absAddr under ASCENDC_CPU_DEBUG -- so the field is filled from the pointer directly.
//
// `lenHint` is the region length this API's own accesses need, not something the caller supplied: a
// bare pointer carries no length, unlike the LocalTensor the Tensor form is given.  It is in
// elements.  It only affects the length-dependent debug checks and the tensor's declared size; it
// takes no part in addressing.  A caller that needs a particular declared length on the workspace
// must keep using the Tensor form.
template <typename T, typename UbPtr>
__aicore__ inline LocalTensor<T> UbTensorOver(UbPtr ubPtr, uint32_t lenHint)
{
    TBuffAddr addr;
    addr.logicPos = static_cast<int8_t>(TPosition::VECCALC);
    addr.bufferAddr = static_cast<uint32_t>(reinterpret_cast<uint64_t>(ubPtr));
    addr.bufferHandle = nullptr;
    addr.dataLen = lenHint * sizeof(PrimT<T>);
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
    addr.absAddr = reinterpret_cast<uint8_t*>(ubPtr);
#endif
    LocalTensor<T> t;
    t.SetAddr(addr);
    return t;
}
} // namespace CubePtrOverload
} // namespace AscendC

#endif // ASCENDC_MODULE_UTILS_PTR_OVERLOAD_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_UTILS_PTR_OVERLOAD_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_UTILS_PTR_OVERLOAD_H__
#endif
