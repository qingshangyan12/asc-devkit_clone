/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning \
    "impl/tensor_api/arch/cube/mmad/mmad.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file mmad.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_MMAD_MMAD_H
#define IMPL_TENSOR_API_ARCH_CUBE_MMAD_MMAD_H

#include "impl/tensor_api/arch/cube/mmad/routing.h"
#include "impl/tensor_api/utils/npu_debug_check.h"

namespace AscendC {
namespace Te {

constexpr MmadTrait DEFAULT_MMAD_TRAIT;

constexpr MmadParams defaultMmadParams = {0, 0, 0, 0, true};

constexpr MmadParams defaultMmadWithBiasParams = {0, 0, 0, 0, false};

struct MmadTraitDefault {
    using TraitType = MmadTrait;
    static constexpr const TraitType value = DEFAULT_MMAD_TRAIT;
};

struct MmadOperation {
public:
    template <typename Tp, const Tp& traits, typename... Args>
    __aicore__ inline static void Mmad(const Args&... args)
    {
        if ASCEND_IS_AIC {
            MmadImpl<traits, Args...>(args...);
        }
    }

private:
    template <const MmadTrait& trait = DEFAULT_MMAD_TRAIT, typename T, typename U, typename S, typename Params>
    __aicore__ inline static void MmadImpl(const T& dst, const U& fm, const S& filter, const Params& params)
    {
        using dstPos = GetMemLocation<T>;
        using fmPos = GetMemLocation<U>;
        using filterPos = GetMemLocation<S>;
        static_assert(Std::is_same_v<dstPos, Location::L0C>, "When Mmad, dst tensor must be from L0C.");
        static_assert(Std::is_same_v<fmPos, Location::L0A>, "When Mmad, fm tensor must be from L0A.");
        static_assert(Std::is_same_v<filterPos, Location::L0B>, "When Mmad, filter tensor must be from L0B.");
        ASCENDC_DEBUG_ASSERT((reinterpret_cast<uintptr_t>(dst.Data().Get()) % 64 == 0),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check dst address in Mmad, dst address must be 64-byte aligned."));
        ASCENDC_DEBUG_ASSERT((reinterpret_cast<uintptr_t>(fm.Data().Get()) % 512 == 0),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check fm address in Mmad, fm address must be 512-byte aligned."));
        ASCENDC_DEBUG_ASSERT((reinterpret_cast<uintptr_t>(filter.Data().Get()) % 512 == 0),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check filter address in Mmad, filter address must be 512-byte aligned."));
        using DstLayout = typename T::layoutType;
        using FmLayout = typename U::layoutType;
        using FilterLayout = typename S::layoutType;
        using DstLayoutPtn = GetLayoutPattern<DstLayout>;
        using FmLayoutPtn = GetLayoutPattern<FmLayout>;
        using FilterLayoutPtn = GetLayoutPattern<FilterLayout>;
        ASCENDC_DEBUG_ASSERT((params.m > 0 && params.m <= 4095), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check m in Mmad, m must be in range (0, 4095]."));
        ASCENDC_DEBUG_ASSERT((params.n > 0 && params.n <= 4095), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check n in Mmad, n must be in range (0, 4095]."));
        ASCENDC_DEBUG_ASSERT((params.k > 0 && params.k <= 4095), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check k in Mmad, k must be in range (0, 4095]."));
        ASCENDC_DEBUG_ASSERT((params.unitFlag == 0 || params.unitFlag == 2 || params.unitFlag == 3),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check unitFlag in Mmad, unitFlag must be 0, 2, or 3."));
        ASCENDC_DEBUG_ASSERT((trait.disableGemv || params.m == 1), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check m in Mmad, m must be 1 when GEMV is enabled."));
        ASCENDC_DEBUG_ASSERT((IsShapeValid(dst.Layout().Shape())), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check dst shape in Mmad, all dst shape sizes must be greater than 0."));
        ASCENDC_DEBUG_ASSERT((IsLayoutStrideValid(dst.Layout())), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check dst stride in Mmad, all dst strides must be greater than 0; a zero stride is allowed "
            "only when the dst format defines it and the corresponding dst shape size is 1."));
        ASCENDC_DEBUG_ASSERT((IsShapeValid(fm.Layout().Shape())), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check fm shape in Mmad, all fm shape sizes must be greater than 0."));
        ASCENDC_DEBUG_ASSERT((IsLayoutStrideValid(fm.Layout())), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check fm stride in Mmad, all fm strides must be greater than 0; a zero stride is allowed "
            "only when the fm format defines it and the corresponding fm shape size is 1."));
        ASCENDC_DEBUG_ASSERT((IsShapeValid(filter.Layout().Shape())), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check filter shape in Mmad, all filter shape sizes must be greater than 0."));
        ASCENDC_DEBUG_ASSERT((IsLayoutStrideValid(filter.Layout())), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check filter stride in Mmad, all filter strides must be greater than 0; a zero stride is "
            "allowed only when the filter format defines it and the corresponding filter shape size is 1."));
        using MmadImpl = typename MmadRouting<
            CURRENT_ARCH_VERSION, DstLayoutPtn, FmLayoutPtn, FilterLayoutPtn, Location::INVALID>::type;
        MmadImpl::template Run<trait>(dst, fm, filter, params);
    }

    template <
        const MmadTrait& trait = DEFAULT_MMAD_TRAIT, typename T, typename U, typename S, typename V, typename Params>
    __aicore__ inline static void MmadImpl(
        const T& dst, const U& fm, const S& filter, const V& bias, const Params& params)
    {
        using dstPos = GetMemLocation<T>;
        using fmPos = GetMemLocation<U>;
        using filterPos = GetMemLocation<S>;
        using biasPos = GetMemLocation<V>;
        static_assert(Std::is_same_v<dstPos, Location::L0C>, "When Mmad, dst tensor must be from L0C.");
        static_assert(Std::is_same_v<fmPos, Location::L0A>, "When Mmad, fm tensor must be from L0A.");
        static_assert(Std::is_same_v<filterPos, Location::L0B>, "When Mmad, filter tensor must be from L0B.");
        static_assert(
            Std::is_same_v<biasPos, Location::L0C> || Std::is_same_v<biasPos, Location::BIAS>,
            "When Mmad, bias tensor must be from L0C or BIAS.");
        ASCENDC_DEBUG_ASSERT((reinterpret_cast<uintptr_t>(dst.Data().Get()) % 64 == 0),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check dst address in Mmad with bias, dst address must be 64-byte aligned."));
        ASCENDC_DEBUG_ASSERT((reinterpret_cast<uintptr_t>(fm.Data().Get()) % 512 == 0),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check fm address in Mmad with bias, fm address must be 512-byte aligned."));
        ASCENDC_DEBUG_ASSERT((reinterpret_cast<uintptr_t>(filter.Data().Get()) % 512 == 0),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check filter address in Mmad with bias, filter address must be 512-byte aligned."));
        using DstLayout = typename T::layoutType;
        using FmLayout = typename U::layoutType;
        using FilterLayout = typename S::layoutType;
        using DstLayoutPtn = GetLayoutPattern<DstLayout>;
        using FmLayoutPtn = GetLayoutPattern<FmLayout>;
        using FilterLayoutPtn = GetLayoutPattern<FilterLayout>;
        ASCENDC_DEBUG_ASSERT((params.m > 0 && params.m <= 4095), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check m in Mmad with bias, m must be in range (0, 4095]."));
        ASCENDC_DEBUG_ASSERT((params.n > 0 && params.n <= 4095), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check n in Mmad with bias, n must be in range (0, 4095]."));
        ASCENDC_DEBUG_ASSERT((params.k > 0 && params.k <= 4095), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check k in Mmad with bias, k must be in range (0, 4095]."));
        ASCENDC_DEBUG_ASSERT((params.unitFlag == 0 || params.unitFlag == 2 || params.unitFlag == 3),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check unitFlag in Mmad with bias, unitFlag must be 0, 2, or 3."));
        ASCENDC_DEBUG_ASSERT((trait.disableGemv || params.m == 1), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check m in Mmad with bias, m must be 1 when GEMV is enabled."));
        ASCENDC_DEBUG_ASSERT((IsShapeValid(dst.Layout().Shape())), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check dst shape in Mmad with bias, all dst shape sizes must be greater than 0."));
        ASCENDC_DEBUG_ASSERT((IsLayoutStrideValid(dst.Layout())), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check dst stride in Mmad with bias, all dst strides must be greater than 0; a zero stride "
            "is allowed only when the dst format defines it and the corresponding dst shape size is 1."));
        ASCENDC_DEBUG_ASSERT((IsShapeValid(fm.Layout().Shape())), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check fm shape in Mmad with bias, all fm shape sizes must be greater than 0."));
        ASCENDC_DEBUG_ASSERT((IsLayoutStrideValid(fm.Layout())), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check fm stride in Mmad with bias, all fm strides must be greater than 0; a zero stride is "
            "allowed only when the fm format defines it and the corresponding fm shape size is 1."));
        ASCENDC_DEBUG_ASSERT((IsShapeValid(filter.Layout().Shape())), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check filter shape in Mmad with bias, all filter shape sizes must be greater than 0."));
        ASCENDC_DEBUG_ASSERT((IsLayoutStrideValid(filter.Layout())), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check filter stride in Mmad with bias, all filter strides must be greater than 0; a zero "
            "stride is allowed only when the filter format defines it and the corresponding filter shape size is 1."));
        ASCENDC_DEBUG_ASSERT((IsShapeValid(bias.Layout().Shape())), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check bias shape in Mmad with bias, all bias shape sizes must be greater than 0."));
        ASCENDC_DEBUG_ASSERT((IsLayoutStrideValid(bias.Layout())), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check bias stride in Mmad with bias, all bias strides must be greater than 0; a zero stride "
            "is allowed only when the bias format defines it and the corresponding bias shape size is 1."));
        using MmadImpl =
            typename MmadRouting<CURRENT_ARCH_VERSION, DstLayoutPtn, FmLayoutPtn, FilterLayoutPtn, biasPos>::type;
        MmadImpl::template Run<trait>(dst, fm, filter, bias, params);
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_CUBE_MMAD_MMAD_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
