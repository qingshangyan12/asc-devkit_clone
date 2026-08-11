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
    "impl/tensor_api/arch/cube/l1_to_l0a/copy.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file copy.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_L1_TO_L0A_COPY_H
#define IMPL_TENSOR_API_ARCH_CUBE_L1_TO_L0A_COPY_H

#include "impl/tensor_api/arch/cube/l1_to_l0a/routing.h"

namespace asc {
namespace te {

constexpr copy_l1_to_l0a_trait default_copy_l1_to_l0a_trait;

struct copy_l1_to_l0a_trait_default {
    using trait_type = copy_l1_to_l0a_trait;
    static constexpr const trait_type value = default_copy_l1_to_l0a_trait;
};

struct copy_l1_to_l0a_base {
public:
    template <const copy_l1_to_l0a_trait& trait = default_copy_l1_to_l0a_trait, typename T, typename U,
              typename PadT = int16_t>
    __aicore__ inline static void load_data_impl(const T& dst, const U& src,
                                                 const img2col_params<PadT>& params = default_img2col_params)
    {
        using dst_pos = get_mem_location<T>;
        using src_pos = get_mem_location<U>;
        static_assert(Std::is_same_v<dst_pos, location::l0a>,
                      "For copy_l1_to_l0a, the destination tensor must be located in L0A.");
        static_assert(Std::is_same_v<src_pos, location::l1>,
                      "For copy_l1_to_l0a, the source tensor must be located in L1.");
        using dst_layout = typename T::layout_type;
        using src_layout = typename U::layout_type;
        using dst_pattern = get_layout_pattern<dst_layout>;
        using src_pattern = get_layout_pattern<src_layout>;
        TENSOR_API_DEBUG_CHECK(debug_check_layout, dst.layout(), "dst", "copy_l1_to_l0a");
        TENSOR_API_DEBUG_CHECK(debug_check_layout, src.layout(), "src", "copy_l1_to_l0a");
        TENSOR_API_DEBUG_CHECK(debug_check_copy_size, src, dst, "copy_l1_to_l0a");
        if constexpr (Std::is_same_v<dst_pattern, nz_layout_ptn> &&
                      Std::is_same_v<src_pattern, nc1hwc0_layout_ptn> &&
                      !is_b4_type<typename T::element_type>) {
            TENSOR_API_DEBUG_CHECK(debug_check_img2col_coord<typename T::element_type>, params,
                                   "copy_l1_to_l0a");
        }
        if constexpr (Std::is_same_v<src_pattern, nc1hwc0_layout_ptn> || Std::is_same_v<src_pattern, ndc1hwc0_layout_ptn>) {
            // conv2D (NC1HWC0) and conv3D (NDC1HWC0) both take the img2col path; routing picks the
            // 2D or 3D implementation selected by the source pattern.
            using copy_l1_to_l0a_impl =
                typename copy_l1_to_l0a_routing<current_arch_version, dst_pattern, src_pattern, false>::type;
            copy_l1_to_l0a_impl::template run<trait, T, U, PadT>(dst, src, params);
        } else {
            if constexpr (dst_layout::depth == five_dim_data && src_layout::depth == five_dim_data) {
                TENSOR_API_DEBUG_CHECK(debug_check_batch_match, get<0>(src.layout().shape()),
                                       get<0>(dst.layout().shape()), "copy_l1_to_l0a");
                TENSOR_API_DEBUG_CHECK(debug_check_l0_batch_stride, get<0>(src.layout().stride()),
                                       remove_batch_dim(src.layout()).capacity(), get<0>(dst.layout().stride()),
                                       remove_batch_dim(dst.layout()).capacity(), "copy_l1_to_l0a");
            }
            constexpr bool is_b8b4 = sizeof(typename T::element_type) == 1;
            using copy_l1_to_l0a_impl =
                typename copy_l1_to_l0a_routing<current_arch_version, dst_pattern, src_pattern, is_b8b4>::type;
            copy_l1_to_l0a_impl::template run<trait, T, U>(dst, src);
        }
    }
};

struct copy_l1_to_l0a : public copy_l1_to_l0a_base {
public:
    template <typename Tp, const Tp& trait, typename... Args>
    __aicore__ inline static void copy(const Args&... args)
    {
        if ASCEND_IS_AIC {
            load_data_impl<trait>(args...);
        }
    }
};

struct copy_l1_to_l0a_with : public copy_l1_to_l0a_base {
public:
    template <typename Tp, const Tp& trait, typename... Args>
    __aicore__ inline static void copy(const Args&... args)
    {
        if ASCEND_IS_AIC {
            load_data_impl<trait>(args...);
        }
    }
};

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_CUBE_L1_TO_L0A_COPY_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
