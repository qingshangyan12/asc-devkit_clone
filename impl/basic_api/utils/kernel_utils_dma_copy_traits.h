/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ASCENDC_KERNEL_UTILS_DMA_COPY_TRAITS_H
#define ASCENDC_KERNEL_UTILS_DMA_COPY_TRAITS_H

#include "kernel_utils_dma_pointer.h"

#if !defined(ASCENDC_CPU_DEBUG) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 2201))
namespace AscendC {
namespace DmaInternal {

template <typename T, typename U, typename D, typename S>
struct CopyPair {
    static constexpr bool value = DmaHasPointer<D, S>::value && DmaIsSupported<D>::value && DmaIsSupported<S>::value &&
                                  DmaTraits<D>::isWritable && DmaMatches<T, D>::value && DmaMatches<U, S>::value;
};

template <typename D, typename S, Hardware dst, Hardware src>
struct CopyRoute {
    static constexpr bool value = DmaCanUse<D, dst>::value && DmaCanUse<S, src>::value;
};

template <typename D, typename S>
struct BasicRoute {
    static constexpr bool same = Std::is_same<DmaElem<D>, DmaElem<S>>::value;
    static constexpr bool gmIn = same && (CopyRoute<D, S, Hardware::UB, Hardware::GM>::value ||
                                          CopyRoute<D, S, Hardware::L1, Hardware::GM>::value);
    static constexpr bool gmOut = same && (CopyRoute<D, S, Hardware::GM, Hardware::UB>::value ||
                                           CopyRoute<D, S, Hardware::GM, Hardware::L1>::value);
    static constexpr bool local = (same && (CopyRoute<D, S, Hardware::UB, Hardware::UB>::value ||
                                            CopyRoute<D, S, Hardware::L1, Hardware::UB>::value ||
                                            CopyRoute<D, S, Hardware::UB, Hardware::L1>::value ||
                                            CopyRoute<D, S, Hardware::FIXBUF, Hardware::L1>::value)) ||
                                  CopyRoute<D, S, Hardware::BIAS, Hardware::L1>::value;
};

template <typename D, typename S>
struct EnhancedRoute {
    using DT = DmaElem<D>;
    using ST = DmaElem<S>;
    static constexpr bool same = Std::is_same<DT, ST>::value;
    static constexpr bool int32ToUb =
        Std::is_same<ST, int32_t>::value && (Std::is_same<DT, half>::value || Std::is_same<DT, int16_t>::value ||
                                             Std::is_same<DT, int8_t>::value || Std::is_same<DT, uint8_t>::value);
    static constexpr bool floatToHalf = Std::is_same<DT, half>::value && Std::is_same<ST, float>::value;
    static constexpr bool halfToFloat = Std::is_same<DT, float>::value && Std::is_same<ST, half>::value;
#if __NPU_ARCH__ == 2201
    static constexpr bool floatToBf16 = Std::is_same<DT, bfloat16_t>::value && Std::is_same<ST, float>::value;
#else
    static constexpr bool floatToBf16 = false;
#endif
    static constexpr bool local =
        (same &&
         (CopyRoute<D, S, Hardware::L1, Hardware::UB>::value || CopyRoute<D, S, Hardware::UB, Hardware::UB>::value ||
          CopyRoute<D, S, Hardware::UB, Hardware::L1>::value)) ||
        ((same || int32ToUb || floatToHalf) && CopyRoute<D, S, Hardware::UB, Hardware::L0C>::value) ||
        ((same || halfToFloat) && CopyRoute<D, S, Hardware::L0C, Hardware::UB>::value) ||
        ((same || floatToHalf || floatToBf16) && CopyRoute<D, S, Hardware::L0C, Hardware::L1>::value);
    static constexpr bool vector = !same && !floatToHalf && !floatToBf16;
};

} // namespace DmaInternal
} // namespace AscendC
#endif
#endif
