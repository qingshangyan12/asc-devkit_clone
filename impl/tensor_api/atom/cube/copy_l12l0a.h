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
    "impl/tensor_api/atom/cube/copy_l12l0a.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file copy_l12l0a.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ATOM_CUBE_COPY_L12L0A_H
#define IMPL_TENSOR_API_ATOM_CUBE_COPY_L12L0A_H

#include "impl/tensor_api/arch/cube/l1_to_l0a/copy.h"
#include "impl/tensor_api/atom/copy_traits_impl.h"

namespace asc {
namespace te {

template <typename Traits>
struct copy_traits<copy_l1_to_l0a, Traits> : public copy_traits<copy_l1_to_l0a, Traits, CopyL12L0AWith, Traits> {
    using BaseType = copy_traits<copy_l1_to_l0a, Traits, CopyL12L0AWith, Traits>;
    using TraitType = typename BaseType::TraitType;
    using BaseType::with;

    template <typename PadT>
    __aicore__ inline constexpr auto with(const Img2ColParams<PadT>& params) const
    {
        return copy_traits<CopyL12L0AWith, Traits, PadT>{normalize_img2col_params(params)};
    }

    template <typename PadT>
    __aicore__ inline constexpr auto with(const img2col_params<PadT>& params) const
    {
        return copy_traits<CopyL12L0AWith, Traits, PadT>{params};
    }
};

template <>
struct copy_traits<copy_l1_to_l0a> : public copy_traits<copy_l1_to_l0a, copy_l1_to_l0a_trait_default> {};

template <typename TraitStruct, typename PadT>
struct copy_traits<CopyL12L0AWith, TraitStruct, PadT> {
    using TraitType = typename TraitStruct::TraitType;
    static constexpr const TraitType defaultTrait = TraitStruct::value;

    template <const TraitType& trait = defaultTrait, typename... Args>
    __aicore__ inline void copy_unpack(const Args&... args) const
    {
        CopyL12L0AWith::copy<TraitType, trait, Args...>(args..., params);
    }
    img2col_params<PadT> params;
};

template <typename TraitStruct>
struct copy_traits<CopyL12L0AWith, TraitStruct> {
    using TraitType = typename TraitStruct::TraitType;
    static constexpr const TraitType defaultTrait = TraitStruct::value;

    template <const TraitType& trait = defaultTrait, typename... Args>
    __aicore__ inline void copy_unpack(const Args&... args) const
    {
        CopyL12L0AWith::copy<TraitType, trait, Args...>(args..., params);
    }
    img2col_params<int16_t> params{};
};

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ATOM_CUBE_COPY_L12L0A_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
