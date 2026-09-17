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
 * \file kernel_utils_pointer.h
 * \brief Shared Tensor/raw-pointer operand traits used by the pointer-extended VECTOR Basic APIs.
 *        A single API accepts either a LocalTensor operand or an equivalent UB raw pointer
 *        (`__ubuf__ PrimType*`) or a raw UB device address (the value returned by LocalTensor::GetPhyAddr()).
 */

#ifndef ASCENDC_MODULE_UTILS_POINTER_H
#define ASCENDC_MODULE_UTILS_POINTER_H

#include <cstdint>
#include "../../../include/utils/std/type_traits.h"
#include "kernel_utils_macros.h"

namespace AscendC {
template <typename T>
class LocalTensor;
template <typename T>
class GlobalTensor;

namespace Internal {

// Element type of a single operand, resolved from the operand itself.
// A LocalTensor<T> operand yields PrimT<T>; a pointer operand yields the pointee PrimType.
template <typename Operand, typename = void>
struct OperandTraits {
    static constexpr bool IS_OPERAND = false;
    static constexpr bool IS_TENSOR = false;
    static constexpr bool IS_POINTER = false;
    static constexpr bool IS_ADDRESS = false;
    static constexpr bool HAS_PRIM = false;
};

template <typename T>
struct OperandTraits<LocalTensor<T>> {
    using PrimType = PrimT<T>;
    static constexpr bool IS_OPERAND = true;
    static constexpr bool IS_TENSOR = true;
    static constexpr bool IS_POINTER = false;
    static constexpr bool IS_ADDRESS = false;
    static constexpr bool HAS_PRIM = true;
};

template <typename T>
struct OperandTraits<GlobalTensor<T>> {
    using PrimType = PrimT<T>;
    static constexpr bool IS_OPERAND = true;
    static constexpr bool IS_TENSOR = true;
    static constexpr bool IS_POINTER = false;
    static constexpr bool IS_ADDRESS = false;
    static constexpr bool HAS_PRIM = true;
};

template <typename T>
struct OperandTraits<T*> {
    using PrimType = PrimT<Std::remove_cv_t<T>>;
    static constexpr bool IS_OPERAND = !Std::is_void<T>::value;
    static constexpr bool IS_TENSOR = false;
    static constexpr bool IS_POINTER = !Std::is_void<T>::value;
    static constexpr bool IS_ADDRESS = false;
    static constexpr bool HAS_PRIM = !Std::is_void<T>::value;
};

// A raw device address, i.e. the uint64_t value of LocalTensor::GetPhyAddr() on the device toolchain.
// Only uint64_t is treated as an address so ordinary integral scalars are never mistaken for pointers.
template <typename T>
struct OperandTraits<T, typename Std::enable_if<Std::is_same<T, uint64_t>::value, void>::type> {
    static constexpr bool IS_OPERAND = true;
    static constexpr bool IS_TENSOR = false;
    static constexpr bool IS_POINTER = false;
    static constexpr bool IS_ADDRESS = true;
    static constexpr bool HAS_PRIM = false;
};

// A pure element-type tag. Used as a last-resort type anchor so that APIs whose element type is only
// spelled once (for example ReduceRepeat<reduceType, T>) can resolve the type of the other operand too.
template <typename T>
struct PrimTag {
    using PrimType = T;
};

template <typename T>
struct OperandTraits<PrimTag<T>, void> {
    using PrimType = T;
    static constexpr bool IS_OPERAND = true;
    static constexpr bool IS_TENSOR = false;
    static constexpr bool IS_POINTER = false;
    static constexpr bool IS_ADDRESS = false;
    static constexpr bool HAS_PRIM = true;
};

template <typename Operand>
using CleanOperand = Std::remove_cvref_t<Operand>;

template <typename Operand>
constexpr bool IsTensorOperand = OperandTraits<CleanOperand<Operand>>::IS_TENSOR;

template <typename Operand>
constexpr bool IsPointerOperand = OperandTraits<CleanOperand<Operand>>::IS_POINTER;

template <typename Operand>
constexpr bool IsAddressOperand = OperandTraits<CleanOperand<Operand>>::IS_ADDRESS;

// True for an operand that is not a LocalTensor/GlobalTensor, i.e. a raw pointer or a raw device address.
// This predicate drives the pointer-extended overloads, so a whole-Tensor call keeps resolving to the
// original Tensor-only overload and no ambiguity is introduced.
template <typename Operand>
constexpr bool IsRawOperand = !IsTensorOperand<Operand> && (OperandTraits<CleanOperand<Operand>>::IS_POINTER ||
                                                            OperandTraits<CleanOperand<Operand>>::IS_ADDRESS);

template <typename... Operands>
constexpr bool IsRawOperandInvolved = (IsRawOperand<Operands> || ...);

// Element type of an operand that carries type information (Tensor or typed pointer); void otherwise.
template <typename Operand, bool = OperandTraits<CleanOperand<Operand>>::HAS_PRIM>
struct OperandPrimTypeImpl {
    using type = typename OperandTraits<CleanOperand<Operand>>::PrimType;
};

template <typename Operand>
struct OperandPrimTypeImpl<Operand, false> {
    using type = void;
};

template <typename Operand>
using OperandPrimT = typename OperandPrimTypeImpl<Operand>::type;

// First operand, scanning left to right, that carries a primitive element type.
template <typename... Operands>
struct FirstOperandPrimType {
    using type = void;
};

template <typename First, typename... Rest>
struct FirstOperandPrimType<First, Rest...> {
    using type = typename Std::conditional_t<
        !Std::is_void<OperandPrimT<First>>::value, OperandPrimTypeImpl<First>, FirstOperandPrimType<Rest...>>::type;
};

// Element type used by a pointer-extended overload:
//  - an explicitly provided element type T always wins (needed for the raw-address form);
//  - otherwise the element type is taken from the first operand that carries one.
template <typename T, bool = Std::is_void<T>::value, typename... Operands>
struct VecPrimTypeImpl {
    using type = PrimT<T>;
};

template <typename T, typename... Operands>
struct VecPrimTypeImpl<T, true, Operands...> {
    using type = typename FirstOperandPrimType<Operands...>::type;
};

template <typename T, typename... Operands>
using VecPrimType = typename VecPrimTypeImpl<T, Std::is_void<T>::value, Operands...>::type;

// The element type of a pointer-extended overload is resolvable when an explicit type T is given or at
// least one operand carries a primitive type. Whole-raw-address calls therefore require an explicit T.
template <typename T, typename... Operands>
constexpr bool CanResolveVecPrim = !Std::is_void<T>::value || (OperandTraits<CleanOperand<Operands>>::HAS_PRIM || ...);

// Element type of a scalar-fill style API (Duplicate and friends): an explicit T wins, then the typed
// destination operand, then the scalar argument itself, whose type is the element type by construction.
template <typename T, typename Dst, typename Scalar, bool = Std::is_void<T>::value>
struct FillPrimTypeImpl {
    using type = PrimT<T>;
};

template <typename T, typename Dst, typename Scalar>
struct FillPrimTypeImpl<T, Dst, Scalar, true> {
    using fromDst = OperandPrimT<Dst>;
    using fromScalar = OperandPrimT<Scalar>;
    using scalarFallback = Std::conditional_t<!Std::is_void<fromScalar>::value, fromScalar, Scalar>;
    using type = Std::conditional_t<!Std::is_void<fromDst>::value, fromDst, scalarFallback>;
};

template <typename T, typename Dst, typename Scalar>
using FillPrimType = typename FillPrimTypeImpl<T, Dst, Scalar>::type;

} // namespace Internal

// Converts a Tensor operand, a typed UB raw pointer or a raw UB device address into a `__ubuf__ PrimType*`.
template <typename PrimType, typename T>
__aicore__ inline __ubuf__ PrimType* GetUbufPtr(const LocalTensor<T>& value)
{
    return reinterpret_cast<__ubuf__ PrimType*>(value.GetPhyAddr());
}

template <
    typename PrimType, typename Pointer,
    typename Std::enable_if<Internal::IsPointerOperand<Pointer>, bool>::type = true>
__aicore__ inline __ubuf__ PrimType* GetUbufPtr(const Pointer& value)
{
    return reinterpret_cast<__ubuf__ PrimType*>(value);
}

template <
    typename PrimType, typename Address,
    typename Std::enable_if<Internal::IsAddressOperand<Address>, bool>::type = true>
__aicore__ inline __ubuf__ PrimType* GetUbufPtr(const Address& value)
{
    return reinterpret_cast<__ubuf__ PrimType*>(value);
}

// Rebuilds a LocalTensor over the same UB buffer as a Tensor/pointer/address operand. Only used by the
// pointer overloads whose original implementation needs a LocalTensor handle but not its size metadata.
template <typename PrimType, typename Operand>
__aicore__ inline LocalTensor<PrimType> MakeVecTensor(const Operand& value)
{
    return LocalTensor<PrimType>(
        TPosition::VECCALC, static_cast<uint32_t>(reinterpret_cast<uintptr_t>(GetUbufPtr<PrimType>(value))), 1);
}

} // namespace AscendC

#endif // ASCENDC_MODULE_UTILS_POINTER_H
