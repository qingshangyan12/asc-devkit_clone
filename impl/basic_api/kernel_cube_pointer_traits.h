/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef ASCENDC_CUBE_POINTER_TRAITS_H
#define ASCENDC_CUBE_POINTER_TRAITS_H
#include "../../include/basic_api/kernel_tensor.h"

namespace AscendC {
namespace Impl {
template <typename T> struct CubeOperandTraits {
    static constexpr bool isPointer = false;
    static constexpr bool valid = false;
};
template <typename T> struct CubeOperandTraits<LocalTensor<T>> {
    using Element = PrimT<T>;
    static constexpr bool isPointer = false;
    static constexpr bool valid = true;
    static constexpr bool isGlobal = false;
};
template <typename T> struct CubeOperandTraits<GlobalTensor<T>> {
    using Element = PrimT<T>;
    static constexpr bool isPointer = false;
    static constexpr bool valid = true;
    static constexpr bool isGlobal = true;
};
#if defined(__NPU_ARCH__) && !defined(ASCENDC_CPU_DEBUG)
#define ASCENDC_CUBE_POINTER_TRAIT(QUALIFIER, SPACE, GLOBAL) \
template <typename T> struct CubeOperandTraits<QUALIFIER T*> { \
    using Element = T; \
    static constexpr bool isPointer = true; \
    static constexpr bool valid = true; \
    static constexpr bool isGlobal = GLOBAL; \
    static constexpr Hardware space = Hardware::SPACE; \
};
ASCENDC_CUBE_POINTER_TRAIT(__gm__, GM, true)
ASCENDC_CUBE_POINTER_TRAIT(__cbuf__, L1, false)
ASCENDC_CUBE_POINTER_TRAIT(__ca__, L0A, false)
ASCENDC_CUBE_POINTER_TRAIT(__cb__, L0B, false)
ASCENDC_CUBE_POINTER_TRAIT(__cc__, L0C, false)
ASCENDC_CUBE_POINTER_TRAIT(__ubuf__, UB, false)
ASCENDC_CUBE_POINTER_TRAIT(__biasbuf__, BIAS, false)
ASCENDC_CUBE_POINTER_TRAIT(__fbuf__, FIXBUF, false)
#undef ASCENDC_CUBE_POINTER_TRAIT
#endif
template <typename T> using CubeElement = typename CubeOperandTraits<T>::Element;
template <typename T> using CubeValue = typename Std::remove_const<CubeElement<T>>::type;
template <typename T, Hardware Space>
__aicore__ constexpr bool CubeMayUseSpace()
{
    if constexpr (CubeOperandTraits<T>::isPointer) {
        return CubeOperandTraits<T>::space == Space;
    }
    return true;
}
template <typename... T> struct CubeHasPointer;
template <> struct CubeHasPointer<> { static constexpr bool value = false; };
template <typename T, typename... Rest> struct CubeHasPointer<T, Rest...> {
    static constexpr bool value = CubeOperandTraits<T>::isPointer || CubeHasPointer<Rest...>::value;
};
template <typename T>
__aicore__ inline auto GetUnderlyingPtr(const T& operand)
{
    if constexpr (CubeOperandTraits<T>::isPointer) {
        if constexpr (CubeOperandTraits<T>::isGlobal) {
#if __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102
            return ExtractL2CacheGmAddr(operand);
#else
            return operand;
#endif
        } else {
            return operand;
        }
    } else {
        return operand.GetPhyAddr();
    }
}
template <typename T>
__aicore__ inline Hardware GetCubeSpace(const T& operand)
{
    if constexpr (CubeOperandTraits<T>::isPointer) {
        return CubeOperandTraits<T>::space;
    } else if constexpr (CubeOperandTraits<T>::isGlobal) {
        return Hardware::GM;
    } else {
        return GetPhyType(static_cast<TPosition>(operand.GetPosition()));
    }
}
template <Hardware Space, typename T>
__aicore__ inline void CheckCubeSpace(const T& operand)
{
    if constexpr (CubeOperandTraits<T>::isPointer) {
        static_assert(CubeOperandTraits<T>::space == Space, "Invalid CUBE operand address space");
    } else if constexpr (CubeOperandTraits<T>::isGlobal) {
        static_assert(Space == Hardware::GM, "Invalid CUBE GlobalTensor address space");
    } else {
        ASCENDC_ASSERT(GetCubeSpace(operand) == Space, { KERNEL_LOG(KERNEL_ERROR, "Invalid CUBE Tensor position"); });
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
        CheckTensorPhyPosition<Space>(operand, "operand", "CUBE operand", "CUBE pointer API");
        constexpr uint32_t alignment = Space == Hardware::L0C ? 1024 :
            ((Space == Hardware::L0A || Space == Hardware::L0B) ? 512 : 32);
        CheckTensorAlignment(operand, alignment, "operand", "CUBE pointer API");
#endif
    }
}
} // namespace Impl
} // namespace AscendC
#endif
