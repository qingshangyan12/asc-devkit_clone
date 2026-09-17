/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
/*!
 * \file kernel_utils_dma_pad_traits.h
 * \brief DataCopyPad operand constraints.
 */
#ifndef ASCENDC_KERNEL_UTILS_DMA_PAD_TRAITS_H
#define ASCENDC_KERNEL_UTILS_DMA_PAD_TRAITS_H

#include "kernel_utils_dma_pointer.h"

#if !defined(ASCENDC_CPU_DEBUG) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 2201))
namespace AscendC {
namespace DmaInternal {
template <typename O>
struct PadTraitOperand : Std::false_type {};

template <typename T>
struct PadTraitOperand<LocalTensor<T>> : Std::integral_constant<bool, !Std::is_same<T, PrimT<T>>::value> {};

template <typename T>
struct PadTraitOperand<GlobalTensor<T>> : Std::integral_constant<bool, !Std::is_same<T, PrimT<T>>::value> {};

template <typename T, typename D, typename S>
struct PadUsesTrait {
    static constexpr bool value = !Std::is_same<T, PrimT<T>>::value ||
                                  PadTraitOperand<typename Std::decay<D>::type>::value ||
                                  PadTraitOperand<typename Std::decay<S>::type>::value;
};

template <typename T, typename D, typename S>
struct PadOperands {
    static constexpr bool value = DmaHasPointer<D, S>::value && DmaIsSupported<D>::value && DmaIsSupported<S>::value &&
                                  DmaTraits<D>::isWritable && Std::is_same<DmaElem<D>, DmaElem<S>>::value &&
                                  DmaMatches<T, D>::value;
};

template <typename T, typename D, typename S>
struct PadGmToLocal {
    static constexpr bool value = PadOperands<T, D, S>::value && DmaCanUse<S, Hardware::GM>::value &&
                                  (DmaCanUse<D, Hardware::UB>::value || DmaCanUse<D, Hardware::L1>::value);
};

template <typename T, typename D, typename S>
struct PadLocalToGm {
    static constexpr bool value = PadOperands<T, D, S>::value && DmaCanUse<D, Hardware::GM>::value &&
                                  (DmaCanUse<S, Hardware::UB>::value || DmaCanUse<S, Hardware::L1>::value);
};

template <typename T, typename D, typename S>
struct PadUbToL1 {
    static constexpr bool value =
        PadOperands<T, D, S>::value && DmaCanUse<D, Hardware::L1>::value && DmaCanUse<S, Hardware::UB>::value;
};

} // namespace DmaInternal
} // namespace AscendC
#endif
#endif // ASCENDC_KERNEL_UTILS_DMA_PAD_TRAITS_H
