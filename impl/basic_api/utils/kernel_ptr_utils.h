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
 * \file kernel_ptr_utils.h
 * \brief Shared pointer/tensor operand traits used by the pointer-extended DMA Basic APIs.
 *        A single set of APIs accepts either a LocalTensor / GlobalTensor operand or the equivalent
 *        hardware address-space pointer (__gm__ / __ubuf__ / __cbuf__ / __cc__ / ...).
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
        "impl/basic_api/utils/kernel_ptr_utils.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_data_copy_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_PTR_UTILS_H__
#endif

#ifndef ASCENDC_MODULE_KERNEL_PTR_UTILS_H
#define ASCENDC_MODULE_KERNEL_PTR_UTILS_H

#include <cstdint>

#include "../../../include/basic_api/kernel_tensor.h"
#include "../../../include/utils/std/type_traits.h"
#include "kernel_utils_macros.h"

namespace AscendC {

/*!
 * \brief Physical address space of a DMA operand, resolved at compile time.
 *        Tensor operands carry no compile-time position information (TPosition is runtime metadata), so they
 *        are reported as kUnknown and keep their original runtime dispatch.
 */
enum class DmaAddrSpace : uint8_t {
    kUnknown = 0,
    kGm,
    kUb,
    kL1,
    kL0A,
    kL0B,
    kL0C,
    kFb,
};

/*!
 * \brief Address space of an address-space qualified pointer. An unqualified pointer (private/local address)
 *        stays kUnknown and therefore never matches a pointer overload.
 */
template <typename T>
struct DmaAddrSpaceOf {
    static constexpr DmaAddrSpace value = DmaAddrSpace::kUnknown;
};

#if !defined(ASCENDC_CPU_DEBUG) || ASCENDC_CPU_DEBUG != 1
// The CPU twin-debug framework defines __gm__ / __ubuf__ / __cbuf__ / __cc__ as empty, which would fold every
// specialization below into the same `T*` and produce a redefinition error. Address-space qualified pointers
// only exist in device compilation, so the mapping is compiled for the NPU path exclusively. In CPU twin-debug
// every pointer operand degrades to kUnknown, which makes the pointer overloads non-viable instead of
// redefining them -- CPU twin-debug keeps calling the original Tensor overloads.
// The specializations match the bare address-space qualified type (no pointer), because
// DmaAddrSpaceQuery strips the pointer before the lookup. An unqualified type falls through to the
// kUnknown primary template (private / local address).
template <typename T>
struct DmaAddrSpaceOf<__gm__ T> {
    static constexpr DmaAddrSpace value = DmaAddrSpace::kGm;
};

template <typename T>
struct DmaAddrSpaceOf<__ubuf__ T> {
    static constexpr DmaAddrSpace value = DmaAddrSpace::kUb;
};

template <typename T>
struct DmaAddrSpaceOf<__cbuf__ T> {
    static constexpr DmaAddrSpace value = DmaAddrSpace::kL1;
};

template <typename T>
struct DmaAddrSpaceOf<__ca__ T> {
    static constexpr DmaAddrSpace value = DmaAddrSpace::kL0A;
};

template <typename T>
struct DmaAddrSpaceOf<__cb__ T> {
    static constexpr DmaAddrSpace value = DmaAddrSpace::kL0B;
};

template <typename T>
struct DmaAddrSpaceOf<__cc__ T> {
    static constexpr DmaAddrSpace value = DmaAddrSpace::kL0C;
};

template <typename T>
struct DmaAddrSpaceOf<__fbuf__ T> {
    static constexpr DmaAddrSpace value = DmaAddrSpace::kFb;
};
#endif // !ASCENDC_CPU_DEBUG

/*!
 * \brief Compile-time query of the address space of an operand type.
 */
template <typename T, bool = Std::is_pointer_v<Std::remove_cv_t<T>>>
struct DmaAddrSpaceQuery {
    // Pointer operand: strip the (possibly address-space qualified) pointer, drop the pointee cv-qualifiers and
    // look the bare pointee type up. remove_cv on an address-space qualified type only strips the pointee
    // cv-qualifiers, so the address space itself survives and stays distinguishable.
    using Pointee = Std::remove_cv_t<Std::remove_pointer_t<Std::remove_cv_t<T>>>;
    static constexpr DmaAddrSpace value = DmaAddrSpaceOf<Pointee>::value;
};

template <typename T>
struct DmaAddrSpaceQuery<T, false> {
    static constexpr DmaAddrSpace value = DmaAddrSpace::kUnknown;
};

template <typename T>
constexpr DmaAddrSpace DmaAddrSpaceOfV = DmaAddrSpaceQuery<T>::value;

template <typename T>
constexpr bool IsDmaGmPtr = DmaAddrSpaceOfV<T> == DmaAddrSpace::kGm;
template <typename T>
constexpr bool IsDmaUbPtr = DmaAddrSpaceOfV<T> == DmaAddrSpace::kUb;
template <typename T>
constexpr bool IsDmaL1Ptr = DmaAddrSpaceOfV<T> == DmaAddrSpace::kL1;
template <typename T>
constexpr bool IsDmaL0CPtr = DmaAddrSpaceOfV<T> == DmaAddrSpace::kL0C;

/*!
 * \brief `true` when the operand is a raw pointer in a hardware address space.
 */
template <typename T>
constexpr bool IsDmaPtr = DmaAddrSpaceOfV<T> != DmaAddrSpace::kUnknown;

/*!
 * \brief `true` when the operand is a LocalTensor / GlobalTensor.
 */
template <typename T>
struct IsDmaTensor : Std::false_type {};

template <typename T>
struct IsDmaTensor<LocalTensor<T>> : Std::true_type {};

template <typename T>
struct IsDmaTensor<GlobalTensor<T>> : Std::true_type {};

template <typename T>
constexpr bool IsDmaTensorV = IsDmaTensor<Std::remove_cv_t<T>>::value;

template <typename T>
struct IsDmaGlobalTensor : Std::false_type {};

template <typename T>
struct IsDmaGlobalTensor<GlobalTensor<T>> : Std::true_type {};

template <typename T>
constexpr bool IsDmaGlobalTensorV = IsDmaGlobalTensor<Std::remove_cv_t<T>>::value;

template <typename T>
struct IsDmaLocalTensor : Std::false_type {};

template <typename T>
struct IsDmaLocalTensor<LocalTensor<T>> : Std::true_type {};

template <typename T>
constexpr bool IsDmaLocalTensorV = IsDmaLocalTensor<Std::remove_cv_t<T>>::value;

/*!
 * \brief Drops the hardware address space qualifier of a pointee type. An address space qualifier survives
 *        remove_cv, so it has to be peeled off explicitly before the pointee can be used as a template
 *        argument (for instance DataCopyPadExtParams<...> must receive `float`, never `__gm__ float`).
 *        An unqualified type falls through untouched.
 */
template <typename T>
struct DmaStripAddrSpace {
    using type = T;
};

#if !defined(ASCENDC_CPU_DEBUG) || ASCENDC_CPU_DEBUG != 1
template <typename T>
struct DmaStripAddrSpace<__gm__ T> {
    using type = T;
};

template <typename T>
struct DmaStripAddrSpace<__ubuf__ T> {
    using type = T;
};

template <typename T>
struct DmaStripAddrSpace<__cbuf__ T> {
    using type = T;
};

template <typename T>
struct DmaStripAddrSpace<__ca__ T> {
    using type = T;
};

template <typename T>
struct DmaStripAddrSpace<__cb__ T> {
    using type = T;
};

template <typename T>
struct DmaStripAddrSpace<__cc__ T> {
    using type = T;
};

template <typename T>
struct DmaStripAddrSpace<__fbuf__ T> {
    using type = T;
};
#endif // !ASCENDC_CPU_DEBUG

template <typename T>
using DmaStripAddrSpaceT = typename DmaStripAddrSpace<T>::type;

/*!
 * \brief Element type of a DMA operand: the pointee type for a raw pointer, PrimType for a Tensor. Both
 *        reduce to the same primitive type the original Tensor-only signature used as its `T`.
 */
template <typename T, bool = IsDmaTensorV<T>>
struct DmaOperandElemTImpl {
    using type = DmaStripAddrSpaceT<Std::remove_cv_t<Std::remove_pointer_t<Std::remove_cv_t<T>>>>;
};

template <typename T>
struct DmaOperandElemTImpl<T, true> {
    using type = typename Std::remove_cv_t<T>::PrimType;
};

template <typename T>
struct DmaOperandElemT {
    using type = typename DmaOperandElemTImpl<T>::type;
};

template <typename T>
using DmaOperandElem = typename DmaOperandElemT<T>::type;

/*!
 * \brief Primitive element type behind the address-space casts made at the call sites. For a Tensor this is
 *        the tensor's own PrimType, which is exactly the expression the original code used; for a pointer it
 *        is the bare pointee type. Both branches yield the same type for equivalent operands.
 */
template <typename T, bool = IsDmaTensorV<T>>
struct DmaPrimTImpl {
    using type = DmaOperandElem<T>;
};

template <typename T>
struct DmaPrimTImpl<T, true> {
    using type = typename Std::remove_cv_t<T>::PrimType;
};

template <typename T>
using DmaPrimT = typename DmaPrimTImpl<T>::type;

/*!
 * \brief Operand shape predicates for the address-space generic overloads.
 *
 * Disambiguation never relies on the address space: every generic family is separated by the operand slot
 * (GlobalTensor vs LocalTensor, checked by type) and by the parameter types. The address space is only used
 * to accept or reject a pointer in a slot, which is what the following predicates express.
 *
 * In CPU twin-debug the qualifiers are empty, so the space of a pointer is unrecoverable at compile time and
 * every raw pointer is accepted in either slot; the address space check then degrades to the caller, which is
 * the documented behaviour of the pointer paths. This keeps the pointer overloads viable -- and therefore
 * executable in CPU twin-debug tests -- instead of removing them from that build.
 */
#if !defined(ASCENDC_CPU_DEBUG) || ASCENDC_CPU_DEBUG != 1
template <typename T>
constexpr bool IsDmaPtrOperand = IsDmaPtr<T>;

template <typename T>
constexpr bool IsDmaGmPtrOperand = IsDmaGmPtr<T>;

template <typename T>
constexpr bool IsDmaLocalPtrOperand = IsDmaPtr<T> && !IsDmaGmPtr<T>;
#else
template <typename T>
constexpr bool IsDmaPtrOperand = Std::is_pointer_v<Std::remove_cv_t<T>>;

template <typename T>
constexpr bool IsDmaGmPtrOperand = Std::is_pointer_v<Std::remove_cv_t<T>>;

template <typename T>
constexpr bool IsDmaLocalPtrOperand = Std::is_pointer_v<Std::remove_cv_t<T>>;
#endif // !ASCENDC_CPU_DEBUG

// GM slot: a GlobalTensor, or a GM pointer. Never a LocalTensor, so the GM and the local slot of a family can
// not both be satisfied by the same Tensor operand.
template <typename T>
constexpr bool IsDmaGmOperand = IsDmaGlobalTensorV<T> || IsDmaGmPtrOperand<T>;

// Local slot: a LocalTensor, or a non-GM hardware pointer. Never a GlobalTensor.
template <typename T>
constexpr bool IsDmaLocalOperand = IsDmaLocalTensorV<T> || IsDmaLocalPtrOperand<T>;

/*!
 * \brief `true` when at least one operand is a raw pointer. The generic overloads are only enabled by this
 *        clause, which guarantees that a Tensor/Tensor call keeps resolving to the original Tensor-only
 *        signatures: every generic candidate is SFINAE-removed for Tensor/Tensor, so no ambiguity is
 *        introduced and the original overload resolution is untouched.
 */
template <typename DstT, typename SrcT>
constexpr bool IsDmaPtrInvolved = IsDmaPtrOperand<DstT> || IsDmaPtrOperand<SrcT>;

/*!
 * \brief Pointer extraction: a raw pointer is passed through unchanged (no reinterpret, no offset fixup), a
 *        Tensor degrades to its physical address. Fully resolved at compile time, so no runtime instruction is
 *        generated for either branch.
 */
template <typename T>
__aicore__ inline constexpr auto GetUnderlyingPtr(const T& val)
{
    if constexpr (Std::is_pointer_v<Std::remove_cv_t<T>>) {
        return val;
    } else {
        return val.GetPhyAddr();
    }
}

} // namespace AscendC

#endif // ASCENDC_MODULE_KERNEL_PTR_UTILS_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_PTR_UTILS_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_PTR_UTILS_H__
#endif
