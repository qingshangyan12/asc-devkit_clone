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
#warning                                                                                                               \
    "impl/tensor_api/arch/cube/l1_to_l0scaleb/copy.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file copy.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_L1_TO_L0SCALEB_COPY_H
#define IMPL_TENSOR_API_ARCH_CUBE_L1_TO_L0SCALEB_COPY_H

#include "impl/tensor_api/arch/cube/l1_to_l0scaleb/routing.h"

namespace asc {
namespace te {

constexpr copy_l1_to_l0scaleb_trait DEFAULT_COPY_L1_TO_L0SCALEB_TRAIT;

struct copy_l1_to_l0scaleb_trait_default {
    using TraitType = copy_l1_to_l0scaleb_trait;
    static constexpr const TraitType value = DEFAULT_COPY_L1_TO_L0SCALEB_TRAIT;
};

struct copy_l1_to_l0scaleb {
public:
    template <typename Tp, const Tp& traits, typename... Args>
    __aicore__ inline static void copy(const Args&... args)
    {
        if ASCEND_IS_AIC {
            load_data<traits, Args...>(args...);
        }
    }

private:
    template <const copy_l1_to_l0scaleb_trait& trait = DEFAULT_COPY_L1_TO_L0SCALEB_TRAIT, typename T, typename U>
    __aicore__ inline static void load_data(const T& dst, const U& src)
    {
        using dstPos = get_mem_location<T>;
        using srcPos = get_mem_location<U>;
        static_assert(Std::is_same_v<dstPos, location::l0scaleb>,
                      "When Copy tensor from L1 to L0ScaleB, dst tensor must on L0ScaleB");
        static_assert(Std::is_same_v<srcPos, location::l1>,
                      "When Copy tensor from L1 to L0ScaleB, src tensor must on L1");
        using DstLayout = typename T::layoutType;
        using SrcLayout = typename U::layoutType;
        using DstPattern = get_layout_pattern<DstLayout>;
        using SrcPattern = get_layout_pattern<SrcLayout>;
        using CopyL12L0ScaleBImpl =
            typename copy_l1_to_l0scaleb_routing<CURRENT_ARCH_VERSION, DstPattern, SrcPattern>::type;
        CopyL12L0ScaleBImpl::template run<trait, T, U>(dst, src);
    }
};

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_CUBE_L1_TO_L0SCALEB_COPY_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
