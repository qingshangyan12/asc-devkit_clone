/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef ASCENDC_MODULE_UTILS_DMA_POINTER_H
#define ASCENDC_MODULE_UTILS_DMA_POINTER_H

#include "../../../include/basic_api/kernel_tensor.h"

// CPU Debug erases address spaces; it cannot resolve a bare local pointer's route.
#if !defined(ASCENDC_CPU_DEBUG) && defined(__NPU_ARCH__)
namespace AscendC {
namespace DmaInternal {
template <typename O>
struct OperandTraits {
    using Element = void;
    static constexpr bool isTensor = false;
    static constexpr bool isPointer = false;
    static constexpr bool isLocal = false;
    static constexpr bool isGlobal = false;
    static constexpr bool isWritable = false;
    static constexpr Hardware space = Hardware::MAX;
};

template <typename T, Hardware H>
struct PointerTraits {
    using Element = typename Std::remove_cv<T>::type;
    static constexpr bool isTensor = false;
    static constexpr bool isPointer = Std::is_same<T, typename Std::remove_volatile<T>::type>::value &&
                                      !Std::is_void<T>::value && !Std::is_pointer<T>::value &&
                                      !Std::is_function<T>::value;
    static constexpr bool isLocal = H != Hardware::GM;
    static constexpr bool isGlobal = H == Hardware::GM;
    static constexpr bool isWritable = !Std::is_const<T>::value;
    static constexpr Hardware space = H;
};

template <typename T>
struct OperandTraits<__gm__ T*> : PointerTraits<T, Hardware::GM> {};
template <typename T>
struct OperandTraits<__ubuf__ T*> : PointerTraits<T, Hardware::UB> {};
template <typename T>
struct OperandTraits<__cbuf__ T*> : PointerTraits<T, Hardware::L1> {};
template <typename T>
struct OperandTraits<__cc__ T*> : PointerTraits<T, Hardware::L0C> {};
template <typename T>
struct OperandTraits<__biasbuf__ T*> : PointerTraits<T, Hardware::BIAS> {};
template <typename T>
struct OperandTraits<__fbuf__ T*> : PointerTraits<T, Hardware::FIXBUF> {};

template <typename T>
struct OperandTraits<LocalTensor<T>> {
    using Element = PrimT<T>;
    static constexpr bool isTensor = true;
    static constexpr bool isPointer = false;
    static constexpr bool isLocal = true;
    static constexpr bool isGlobal = false;
    static constexpr bool isWritable = true;
    static constexpr Hardware space = Hardware::MAX;
};
template <typename T>
struct OperandTraits<GlobalTensor<T>> {
    using Element = PrimT<T>;
    static constexpr bool isTensor = true;
    static constexpr bool isPointer = false;
    static constexpr bool isLocal = false;
    static constexpr bool isGlobal = true;
    static constexpr bool isWritable = true;
    static constexpr Hardware space = Hardware::GM;
};

template <typename O>
using DmaTraits = OperandTraits<typename Std::decay<O>::type>;
template <typename O>
using DmaElem = typename DmaTraits<O>::Element;
template <typename O>
struct DmaIsSupported {
    static constexpr bool value = DmaTraits<O>::isTensor || DmaTraits<O>::isPointer;
};
template <typename O, Hardware H>
struct DmaCanUse {
    static constexpr bool value =
        DmaIsSupported<O>::value && (DmaTraits<O>::space == H || (DmaTraits<O>::isTensor && DmaTraits<O>::isLocal &&
                                                                  H != Hardware::GM && H != Hardware::MAX));
};
template <typename T, typename O>
struct DmaMatches {
    static constexpr bool value = Std::is_same<PrimT<T>, DmaElem<O>>::value;
};
template <typename O>
struct DmaMatches<void, O> {
    static constexpr bool value = DmaIsSupported<O>::value;
};
template <typename D, typename S>
struct DmaHasPointer {
    static constexpr bool value =
        DmaIsSupported<D>::value && DmaIsSupported<S>::value && (DmaTraits<D>::isPointer || DmaTraits<S>::isPointer);
};

template <typename O>
__aicore__ inline Hardware DmaPosition(const O& operand)
{
    if constexpr (DmaTraits<O>::isTensor && DmaTraits<O>::isLocal) {
        return GetPhyType(static_cast<TPosition>(operand.GetPosition()));
    } else {
        return DmaTraits<O>::space;
    }
}

template <Hardware H, typename O>
__aicore__ inline auto GetUnderlyingPtr(O&& operand)
{
    static_assert(DmaCanUse<O, H>::value, "DMA operand has an incompatible address space");
    if constexpr (DmaTraits<O>::isPointer) {
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
        if constexpr (H == Hardware::GM) {
            return ExtractL2CacheGmAddr(operand);
        } else {
            return operand;
        }
#else
        return operand;
#endif
    } else if constexpr (H == Hardware::GM) {
        return operand.GetPhyAddr(uint64_t(0));
    } else if constexpr (H == Hardware::UB) {
        return reinterpret_cast<__ubuf__ DmaElem<O>*>(operand.GetPhyAddr());
    } else if constexpr (H == Hardware::L1) {
        return reinterpret_cast<__cbuf__ DmaElem<O>*>(operand.GetPhyAddr());
    } else if constexpr (H == Hardware::L0C) {
        return reinterpret_cast<__cc__ DmaElem<O>*>(operand.GetPhyAddr());
    } else if constexpr (H == Hardware::BIAS) {
        return reinterpret_cast<__biasbuf__ DmaElem<O>*>(operand.GetPhyAddr());
    } else {
        return reinterpret_cast<__fbuf__ DmaElem<O>*>(operand.GetPhyAddr());
    }
}

template <typename O>
__aicore__ inline uint8_t DmaCacheMode(const O& operand)
{
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
    if constexpr (DmaTraits<O>::isGlobal) {
        return ExtractCacheMode(operand);
    } else {
        return 0;
    }
#else
    return 0;
#endif
}
} // namespace DmaInternal
} // namespace AscendC
#endif
#endif // ASCENDC_MODULE_UTILS_DMA_POINTER_H
