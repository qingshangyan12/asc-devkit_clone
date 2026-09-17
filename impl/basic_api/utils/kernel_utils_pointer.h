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
 * \brief Tensor and hardware-pointer operand helpers.
 */

#ifndef ASCENDC_MODULE_UTILS_POINTER_H
#define ASCENDC_MODULE_UTILS_POINTER_H

#include "../../../include/utils/std/type_traits.h"
#include "../common_types.h"
#include "kernel_utils_macros.h"

namespace AscendC {
template <typename T>
class LocalTensor;
template <typename T>
class GlobalTensor;

namespace Internal {
template <typename Operand>
struct OperandTraits {
    static constexpr bool IS_OPERAND = false;
    static constexpr bool IS_POINTER = false;
    static constexpr bool IS_CONST_POINTER = false;
    static constexpr bool IS_GLOBAL_TENSOR = false;
};

template <typename T>
struct OperandTraits<LocalTensor<T>> {
    using PrimType = PrimT<T>;
    static constexpr bool IS_OPERAND = true;
    static constexpr bool IS_POINTER = false;
    static constexpr bool IS_CONST_POINTER = false;
    static constexpr bool IS_GLOBAL_TENSOR = false;
};

template <typename T>
struct OperandTraits<GlobalTensor<T>> {
    using PrimType = PrimT<T>;
    static constexpr bool IS_OPERAND = true;
    static constexpr bool IS_POINTER = false;
    static constexpr bool IS_CONST_POINTER = false;
    static constexpr bool IS_GLOBAL_TENSOR = true;
};

template <typename T>
struct OperandTraits<T*> {
    using PrimType = PrimT<Std::remove_cv_t<T>>;
    static constexpr bool IS_OPERAND = !Std::is_void<T>::value;
    static constexpr bool IS_POINTER = !Std::is_void<T>::value;
    static constexpr bool IS_CONST_POINTER = Std::is_const<T>::value;
    static constexpr bool IS_GLOBAL_TENSOR = false;
};

template <typename Operand>
using OperandPrimT = typename OperandTraits<Std::remove_cvref_t<Operand>>::PrimType;

template <typename Operand>
constexpr bool IsPointerOperand = OperandTraits<Std::remove_cvref_t<Operand>>::IS_POINTER;

template <typename Operand>
constexpr bool IsConstPointerOperand = OperandTraits<Std::remove_cvref_t<Operand>>::IS_CONST_POINTER;

template <typename Operand>
struct WritableOperandTraits {
    static_assert(
        !IsConstPointerOperand<Operand>,
        "A const-qualified raw pointer cannot be used as a writable destination or workspace");
    using PrimType = OperandPrimT<Operand>;
};

template <typename Operand>
using WritableOperandPrimT = typename WritableOperandTraits<Operand>::PrimType;

template <typename Operand>
constexpr bool IsTensorOrPointerOperand = OperandTraits<Std::remove_cvref_t<Operand>>::IS_OPERAND;

template <typename Operand>
constexpr bool IsGlobalTensorOperand = OperandTraits<Std::remove_cvref_t<Operand>>::IS_GLOBAL_TENSOR;

} // namespace Internal

template <typename T>
__aicore__ inline auto GetUnderlyingPtr(const LocalTensor<T>& value)
{
    return value.GetPhyAddr();
}

template <typename T>
__aicore__ inline auto GetUnderlyingPtr(const GlobalTensor<T>& value)
{
    return value.GetPhyAddr();
}

template <typename Pointer, typename Std::enable_if<Internal::IsPointerOperand<Pointer>, bool>::type = true>
__aicore__ inline auto GetUnderlyingPtr(const Pointer& value)
{
    return value;
}

template <typename Operand>
__aicore__ inline uint8_t GetOperandCacheMode(const Operand& value)
{
    using CleanOperand = Std::remove_cvref_t<Operand>;
    if constexpr (Internal::OperandTraits<CleanOperand>::IS_GLOBAL_TENSOR) {
        // GetPhyAddr() removes the encoded cache bits on cache-aware targets,
        // so read the mode from GlobalTensor before extracting its address.
        return ExtractCacheMode(value);
    } else {
        // A raw __gm__ pointer does not carry GlobalTensor's cache metadata and
        // cannot be passed to ExtractCacheMode. Keep the backend default mode.
        static_assert(Internal::IsPointerOperand<CleanOperand>, "cache mode is only defined for GM operands");
        return 0;
    }
}

template <typename T>
__aicore__ inline TPosition GetOperandPosition(const LocalTensor<T>& value)
{
    return static_cast<TPosition>(value.GetPosition());
}

template <typename T>
__aicore__ inline constexpr TPosition GetOperandPosition(const GlobalTensor<T>&)
{
    return TPosition::GM;
}

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
// The CPU debug headers erase device address-space qualifiers. Keep one fallback
// overload so host-side header checking remains valid; device compilation uses
// the qualified overloads below and retains the exact storage position.
template <typename T>
__aicore__ inline constexpr TPosition GetOperandPosition(T*)
{
    return TPosition::VECCALC;
}
#else
template <typename T>
__aicore__ inline constexpr TPosition GetOperandPosition(__gm__ T*)
{
    return TPosition::GM;
}

template <typename T>
__aicore__ inline constexpr TPosition GetOperandPosition(__cbuf__ T*)
{
    return TPosition::A1;
}

template <typename T>
__aicore__ inline constexpr TPosition GetOperandPosition(__ca__ T*)
{
    return TPosition::A2;
}

template <typename T>
__aicore__ inline constexpr TPosition GetOperandPosition(__cb__ T*)
{
    return TPosition::B2;
}

template <typename T>
__aicore__ inline constexpr TPosition GetOperandPosition(__cc__ T*)
{
    return TPosition::CO1;
}

template <typename T>
__aicore__ inline constexpr TPosition GetOperandPosition(__ubuf__ T*)
{
    return TPosition::VECCALC;
}

template <typename T>
__aicore__ inline constexpr TPosition GetOperandPosition(__fbuf__ T*)
{
    return TPosition::C2PIPE2GM;
}

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5102)
template <typename T>
__aicore__ inline constexpr TPosition GetOperandPosition(__biasbuf__ T*)
{
    return TPosition::C2;
}
#endif
#endif

template <
    typename Operand,
    typename Std::enable_if<
        Internal::IsPointerOperand<Operand> || Internal::IsGlobalTensorOperand<Operand>, bool>::type = true>
__aicore__ inline constexpr TPosition GetStaticOperandPosition()
{
    if constexpr (Internal::IsGlobalTensorOperand<Operand>) {
        return TPosition::GM;
    } else {
        return GetOperandPosition(static_cast<Std::remove_cvref_t<Operand>>(nullptr));
    }
}
} // namespace AscendC

#endif // ASCENDC_MODULE_UTILS_POINTER_H
