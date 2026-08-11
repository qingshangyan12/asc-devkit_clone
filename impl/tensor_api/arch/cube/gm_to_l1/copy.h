/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning                                                                                                              \
    "impl/tensor_api/arch/cube/gm_to_l1/copy.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file copy.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_GM_TO_L1_COPY_H
#define IMPL_TENSOR_API_ARCH_CUBE_GM_TO_L1_COPY_H

#include "impl/tensor_api/arch/cube/gm_to_l1/routing.h"

namespace asc {
namespace te {

constexpr copy_gm_to_l1_trait DEFAULT_COPY_GM_TO_L1_TRAIT;

struct copy_gm_to_l1_trait_default {
    using trait_type = copy_gm_to_l1_trait;
    static constexpr const trait_type value = DEFAULT_COPY_GM_TO_L1_TRAIT;
};

struct copy_gm_to_l1 {
public:
    template <typename Tp, const Tp& traits, typename... arg_types>
    __aicore__ inline static void copy(const arg_types&... args)
    {
        if ASCEND_IS_AIC {
            data_copy_impl<traits, arg_types...>(args...);
        }
    }

private:
    template <const copy_gm_to_l1_trait& trait = DEFAULT_COPY_GM_TO_L1_TRAIT, typename T, typename U>
    __aicore__ inline static void data_copy_impl(const T& dst, const U& src)
    {
        using dst_pos = get_mem_location<T>;
        using src_pos = get_mem_location<U>;
        static_assert(Std::is_same_v<dst_pos, location::l1>,
            "copy_gm_to_l1 requires destination on l1");
        static_assert(Std::is_same_v<src_pos, location::gm>,
            "copy_gm_to_l1 requires source on gm");
        using dst_layout = typename T::layout_type;
        using src_layout = typename U::layout_type;
        using dst_layout_ptn = get_layout_pattern<dst_layout>;
        using src_layout_ptn = get_layout_pattern<src_layout>;
        using copy_gm_to_l1_impl =
            typename copy_gm_to_l1_routing<CURRENT_ARCH_VERSION, dst_layout_ptn, src_layout_ptn>::type;
        copy_gm_to_l1_impl::template run<trait, T, U>(dst, src);
    }

    template <const copy_gm_to_l1_trait& trait = DEFAULT_COPY_GM_TO_L1_TRAIT, typename T, typename U,
        typename coord_dst_type, typename coord_src_type, typename shape_type>
    __aicore__ inline static void data_copy_impl(
        const T& dst, const U& src, const coord_dst_type& coord_dst, const coord_src_type& coord_src, const shape_type& shape)
    {
        using dst_pos = get_mem_location<T>;
        using src_pos = get_mem_location<U>;
        static_assert(Std::is_same_v<dst_pos, location::l1>, "copy_gm_to_l1 requires destination on l1");
        static_assert(Std::is_same_v<src_pos, location::gm>, "copy_gm_to_l1 requires source on gm");
        using dst_layout_ptn = get_layout_pattern<typename T::layout_type>;
        using src_layout_ptn = get_layout_pattern<typename U::layout_type>;
        using impl_type = typename copy_gm_to_l1_routing<CURRENT_ARCH_VERSION, dst_layout_ptn, src_layout_ptn>::type;
        auto resolved_coord_dst = resolve_copy_coord(dst.layout(), shape, coord_dst);
        auto resolved_coord_src = resolve_copy_coord(src.layout(), shape, coord_src);
        impl_type::template run<trait, T, U>(dst, src, resolved_coord_dst, resolved_coord_src, shape);
    }
};

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_CUBE_GM_TO_L1_COPY_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
