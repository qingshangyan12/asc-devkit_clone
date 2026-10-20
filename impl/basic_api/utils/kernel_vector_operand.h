/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 */
#ifndef ASCENDC_KERNEL_VECTOR_OPERAND_H
#define ASCENDC_KERNEL_VECTOR_OPERAND_H

namespace AscendC {
namespace VectorOperand {
struct DefaultType {};
template <typename T> struct Identity { using Type = T; };

template <typename T> struct Traits {
    using Element = void;
    static constexpr bool isPointer = false;
    static constexpr bool isOperand = false;
};

template <typename T> struct Traits<LocalTensor<T>> {
    using Element = T;
    static constexpr bool isPointer = false;
    static constexpr bool isOperand = true;
};

template <typename T> struct Traits<__ubuf__ T*> {
    using Element = T;
    static constexpr bool isPointer = true;
    static constexpr bool isOperand = true;
};

template <typename T, unsigned long long N> struct Traits<__ubuf__ T[N]> {
    using Element = T;
    static constexpr bool isPointer = true;
    static constexpr bool isOperand = true;
};

template <typename Explicit, typename Operand> struct Resolve {
    using Type = Explicit;
};

template <typename Operand> struct Resolve<DefaultType, Operand> {
    using Type = typename Traits<Operand>::Element;
};

template <typename Explicit, typename Operand>
using ElementT = typename Resolve<Explicit, Operand>::Type;

template <typename... Operands> __aicore__ constexpr bool HasPointer()
{
    return (Traits<Operands>::isPointer || ...);
}

template <typename... Operands> __aicore__ constexpr bool AllOperands()
{
    return (Traits<Operands>::isOperand && ...);
}

template <typename Operand>
using Primitive = PrimT<typename Traits<Operand>::Element>;

template <typename A, typename B> __aicore__ constexpr bool SameElement()
{
    return Std::is_same<Primitive<A>, Primitive<B>>::value;
}

template <typename Operand, typename Count>
__aicore__ inline bool FitsCount(Operand operand, Count count)
{
    if constexpr (Traits<Operand>::isPointer) {
        return true; // Capacity is a caller precondition for a raw pointer.
    } else {
        return count <= operand.GetSize();
    }
}

template <typename Expected, typename Operand> __aicore__ constexpr bool Matches()
{
    if constexpr (!Traits<Operand>::isOperand) {
        return false;
    } else {
        return Std::is_same<PrimT<Expected>, PrimT<typename Traits<Operand>::Element>>::value;
    }
}

template <typename Operand> __aicore__ constexpr bool IsWritable()
{
    return Traits<Operand>::isOperand && !Std::is_const<typename Traits<Operand>::Element>::value;
}
} // namespace VectorOperand

template <typename T>
__aicore__ inline __ubuf__ T* GetUnderlyingPtr(__ubuf__ T* value)
{
    return value;
}

template <typename T>
__aicore__ inline __ubuf__ PrimT<T>* GetUnderlyingPtr(const LocalTensor<T>& value)
{
    return (__ubuf__ PrimT<T>*)value.GetPhyAddr();
}
} // namespace AscendC
#endif // ASCENDC_KERNEL_VECTOR_OPERAND_H
