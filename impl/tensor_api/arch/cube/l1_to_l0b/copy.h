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
    "impl/tensor_api/arch/cube/l1_to_l0b/copy.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file copy.h
* \brief
*/
#ifndef IMPL_TENSOR_API_ARCH_CUBE_L1_TO_L0B_COPY_H
#define IMPL_TENSOR_API_ARCH_CUBE_L1_TO_L0B_COPY_H

#include "impl/tensor_api/arch/cube/l1_to_l0b/routing.h"

namespace asc {
namespace te {

constexpr copy_l1_to_l0b_trait DEFAULT_COPY_L1_TO_L0B_TRAIT;

struct copy_l1_to_l0b_trait_default {
    using TraitType = copy_l1_to_l0b_trait;
    static constexpr const TraitType value = DEFAULT_COPY_L1_TO_L0B_TRAIT;
};

using CopyL12L0BModeSet = TupleMap<
    Std::tuple<Std::tuple<_1, _0>, CopyMode::NORMAL>,
    Std::tuple<Std::tuple<_1, _1>, CopyMode::NORMAL>,
    Std::tuple<Std::tuple<_0, _0>, CopyMode::TRANS>,
    Std::tuple<Std::tuple<_0, _1>, CopyMode::TRANS_B8B4>>;

struct copy_l1_to_l0b {
public:
    template <typename Tp, const Tp& traits, typename... Args>
    __aicore__ inline static void copy(const Args& ...args)
    {
        if ASCEND_IS_AIC {
            load_data<traits, Args...>(args...);
        }
    }

private:
    template<const copy_l1_to_l0b_trait& trait = DEFAULT_COPY_L1_TO_L0B_TRAIT, typename T, typename U>
    __aicore__ inline static void load_data(const T& dst, const U& src)
    {
        using dstPos = GetMemLocation<T>;
        using srcPos = GetMemLocation<U>;
        static_assert(Std::is_same_v<dstPos, Location::L0B>,
            "When Copy tensor from L1 to L0B, dst tensor must be from L0B.");
        static_assert(Std::is_same_v<srcPos, Location::L1>,
            "When Copy tensor from L1 to L0B, src tensor must be from L1.");
        using DstLayout = typename T::layoutType;
        using SrcLayout = typename U::layoutType;
        using DstPattern = GetLayoutPattern<DstLayout>;
        using SrcPattern = GetLayoutPattern<SrcLayout>;
        constexpr auto is_b8_b4_type = sizeof(typename T::elementType) == 1;
        constexpr auto no_trans = Std::is_same_v<DstPattern, SrcPattern>;
        using CopyL12L0BMode = typename CopyL12L0BModeSet::template Get<Std::tuple<Std::Int<no_trans>, Std::Int<is_b8_b4_type>>>;
        static_assert(!Std::is_same_v<CopyL12L0BMode, Std::ignore_t>, "Unsupported CopyL12L0BMode.");
        using CopyL12L0BImpl = typename CopyL12L0BRouting<CURRENT_ARCH_VERSION, DstPattern, SrcPattern, CopyL12L0BMode>::type;
        CopyL12L0BImpl::template run<trait, T, U>(dst, src);
    }
};

}
}

#endif // IMPL_TENSOR_API_ARCH_CUBE_L1_TO_L0B_COPY_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
