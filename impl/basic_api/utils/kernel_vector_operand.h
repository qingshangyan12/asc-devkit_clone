/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ASCENDC_KERNEL_VECTOR_OPERAND_H
#define ASCENDC_KERNEL_VECTOR_OPERAND_H

namespace AscendC {
#if ASCENDC_CPU_DEBUG
inline uint8_t* GetBaseAddrCpu(int8_t logicPos);
#endif
namespace VectorOperand {
// Select the existing Config/non-Config overload family in the implementation layer.
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5162) || (__NPU_ARCH__ == 9201) || \
    (__NPU_ARCH__ == 9202)
constexpr bool HAS_CONFIG = true;
#else
constexpr bool HAS_CONFIG = false;
#endif

struct DefaultType {};
template <typename T>
struct Identity {
    using Type = T;
};

template <typename T>
struct Traits {
    using Element = void;
    static constexpr bool isPointer = false;
    static constexpr bool isOperand = false;
};

template <typename T>
struct Traits<LocalTensor<T>> {
    using Element = T;
    static constexpr bool isPointer = false;
    static constexpr bool isOperand = true;
};

template <typename T>
struct Traits<__ubuf__ T*> {
    using Element = T;
    static constexpr bool isPointer = true;
    static constexpr bool isOperand = true;
};

template <typename T, unsigned long long N>
struct Traits<__ubuf__ T[N]> {
    using Element = T;
    static constexpr bool isPointer = true;
    static constexpr bool isOperand = true;
};

template <typename Explicit, typename Operand>
struct Resolve {
    using Type = Explicit;
};

template <typename Operand>
struct Resolve<DefaultType, Operand> {
    using Type = typename Traits<Operand>::Element;
};

template <typename Explicit, typename Operand>
using ElementT = typename Resolve<Explicit, Operand>::Type;

template <typename... Operands>
__aicore__ constexpr bool HasPointer()
{
    return (Traits<Operands>::isPointer || ...);
}

template <typename... Operands>
__aicore__ constexpr bool AllOperands()
{
    return (Traits<Operands>::isOperand && ...);
}

template <typename Operand>
using Primitive = PrimT<typename Traits<Operand>::Element>;

template <typename A, typename B>
__aicore__ constexpr bool SameElement()
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

template <typename Expected, typename Operand>
__aicore__ constexpr bool Matches()
{
    if constexpr (!Traits<Operand>::isOperand) {
        return false;
    } else {
        return Std::is_same<PrimT<Expected>, PrimT<typename Traits<Operand>::Element>>::value;
    }
}

template <typename Operand>
__aicore__ constexpr bool IsWritable()
{
    return Traits<Operand>::isOperand && !Std::is_const<typename Traits<Operand>::Element>::value;
}

#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG) || defined(__MSTX_DFX_REPORT__)
// Diagnostic paths share the existing Tensor checks and MSTX reporting. Raw pointers
// carry no allocation length: only the remaining physical UB range is known here.
// Mixed LocalTensor operands retain their own position, allocation bounds and offsets.
template <typename T, typename U>
__aicore__ inline LocalTensor<T> ToDebugTensor(const LocalTensor<U>& value)
{
    if constexpr (Std::is_same<T, U>::value) {
        return value;
    } else {
        return value.template ReinterpretCast<T>();
    }
}

template <typename T, typename U>
__aicore__ inline LocalTensor<T> ToDebugTensor(__ubuf__ U* value)
{
    TBuffAddr address{};
    address.logicPos = static_cast<uint8_t>(TPosition::VECCALC);
    uint64_t offset = reinterpret_cast<uint64_t>(value);
#if ASCENDC_CPU_DEBUG
    const uint64_t base = reinterpret_cast<uint64_t>(GetBaseAddrCpu(static_cast<int8_t>(TPosition::VECCALC)));
    ASCENDC_DEBUG_ASSERT(
        (offset >= base && offset - base < TOTAL_UB_SIZE),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "VECTOR pointer is outside the simulated UB address range."));
    offset -= base;
    address.absAddr = reinterpret_cast<uint8_t*>(const_cast<Std::remove_const_t<U>*>(value));
#endif
    address.bufferAddr = static_cast<uint32_t>(offset);
    address.dataLen = offset < TOTAL_UB_SIZE ? TOTAL_UB_SIZE - static_cast<uint32_t>(offset) : 0;
    LocalTensor<T> tensor;
    tensor.SetAddr(address);
    return tensor;
}

template <typename Operand>
__aicore__ inline auto ToDebugOperand(const Operand& value)
{
    if constexpr (Traits<Operand>::isOperand) {
        return ToDebugTensor<Std::remove_const_t<Primitive<Operand>>>(value);
    } else {
        return value;
    }
}
#endif

template <typename Operand>
__aicore__ inline auto Offset(const Operand& value, uint32_t offset)
{
    if constexpr (Traits<Operand>::isPointer) {
        return value + offset;
    } else {
        return value[offset];
    }
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
