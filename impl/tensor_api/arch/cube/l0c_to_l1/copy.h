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
    "impl/tensor_api/arch/cube/l0c_to_l1/copy.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file copy.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_L0C_TO_L1_COPY_H
#define IMPL_TENSOR_API_ARCH_CUBE_L0C_TO_L1_COPY_H

#include "impl/tensor_api/utils/utils_impl.h"
#include "impl/tensor_api/arch/cube/utils/l0c2out_utils.h"
#include "impl/tensor_api/arch/cube/l0c_to_l1/routing.h"

namespace asc {
namespace te {

struct copy_l0c_to_l1_base {
public:
    template <const copy_l0c_to_l1_trait& trait, typename T, typename U>
    __aicore__ inline static void data_copy_impl(const T& dst, const U& src,
                                                 const fixpipe_params& params = default_fixpipe_params)
    {
        using dst_pos = get_mem_location<T>;
        using src_pos = get_mem_location<U>;
        static_assert(Std::is_same_v<dst_pos, location::l1>,
                      "For copy_l0c_to_l1, the destination tensor must be located in L1.");
        static_assert(Std::is_same_v<src_pos, location::l0c>,
                      "For copy_l0c_to_l1, the source tensor must be located in L0C.");
        TENSOR_API_DEBUG_CHECK(debug_check_layout, dst.layout(), "dst", "copy_l0c_to_l1");
        TENSOR_API_DEBUG_CHECK(debug_check_layout, src.layout(), "src", "copy_l0c_to_l1");
        TENSOR_API_DEBUG_CHECK(debug_check_copy_size, src, dst, "copy_l0c_to_l1");

        using dst_layout_ptn = get_layout_pattern<typename T::layout_type>;
        using src_layout_ptn = get_layout_pattern<typename U::layout_type>;

        using copy_l0c_to_l1_impl =
            typename copy_l0c_to_l1_routing<current_arch_version, dst_layout_ptn, src_layout_ptn>::type;
        copy_l0c_to_l1_impl::template run<trait>(dst, src, params);
    }

    template <const copy_l0c_to_l1_trait& trait, typename T, typename U, typename S>
    __aicore__ inline static typename Std::enable_if<Std::is_same_v<S, uint64_t>, void>::type
    data_copy_impl(const T& dst, const U& src, const S& quant, const fixpipe_params& params = default_fixpipe_params)
    {
        using dst_pos = get_mem_location<T>;
        using src_pos = get_mem_location<U>;
        static_assert(Std::is_same_v<dst_pos, location::l1>,
                      "For copy_l0c_to_l1, the destination tensor must be located in L1.");
        static_assert(Std::is_same_v<src_pos, location::l0c>,
                      "For copy_l0c_to_l1, the source tensor must be located in L0C.");
        TENSOR_API_DEBUG_CHECK(debug_check_layout, dst.layout(), "dst", "copy_l0c_to_l1");
        TENSOR_API_DEBUG_CHECK(debug_check_layout, src.layout(), "src", "copy_l0c_to_l1");
        TENSOR_API_DEBUG_CHECK(debug_check_copy_size, src, dst, "copy_l0c_to_l1");

        using dst_layout_ptn = get_layout_pattern<typename T::layout_type>;
        using src_layout_ptn = get_layout_pattern<typename U::layout_type>;

        using copy_l0c_to_l1_impl =
            typename copy_l0c_to_l1_routing<current_arch_version, dst_layout_ptn, src_layout_ptn>::type;
        copy_l0c_to_l1_impl::template run<trait>(dst, src, quant, params);
    }

    template <const copy_l0c_to_l1_trait& trait, typename T, typename U, typename S>
    __aicore__ inline static typename Std::enable_if<is_attr_tensor_v<S>, void>::type
    data_copy_impl(const T& dst, const U& src, const S& quant, const fixpipe_params& params = default_fixpipe_params)
    {
        using dst_pos = get_mem_location<T>;
        using src_pos = get_mem_location<U>;
        static_assert(Std::is_same_v<dst_pos, location::l1>,
                      "For copy_l0c_to_l1, the destination tensor must be located in L1.");
        static_assert(Std::is_same_v<src_pos, location::l0c>,
                      "For copy_l0c_to_l1, the source tensor must be located in L0C.");
        TENSOR_API_DEBUG_CHECK(debug_check_layout, dst.layout(), "dst", "copy_l0c_to_l1");
        TENSOR_API_DEBUG_CHECK(debug_check_layout, src.layout(), "src", "copy_l0c_to_l1");
        TENSOR_API_DEBUG_CHECK(debug_check_layout, quant.layout(), "quant", "copy_l0c_to_l1");
        TENSOR_API_DEBUG_CHECK(debug_check_copy_size, src, dst, "copy_l0c_to_l1");

        using dst_layout_ptn = get_layout_pattern<typename T::layout_type>;
        using src_layout_ptn = get_layout_pattern<typename U::layout_type>;

        using copy_l0c_to_l1_impl =
            typename copy_l0c_to_l1_routing<current_arch_version, dst_layout_ptn, src_layout_ptn>::type;
        copy_l0c_to_l1_impl::template run<trait>(dst, src, quant, params);
    }
};

template <typename TraitType, const TraitType& trait, typename... Args>
__aicore__ inline void copy_l0c_to_l1::copy(const Args&... args)
{
    if ASCEND_IS_AIV {
        return;
    }
    copy_l0c_to_l1_base::data_copy_impl<trait>(args...);
}

struct copy_l0c_to_l1_with : public copy_l0c_to_l1_base {
public:
    template <typename Tp, const Tp& trait, typename... Args>
    __aicore__ inline static void copy(const Args&... args)
    {
        if ASCEND_IS_AIV {
            return;
        }
        data_copy_impl<trait>(args...);
    }
};

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_CUBE_L0C_TO_L1_COPY_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
