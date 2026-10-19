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
    "impl/tensor_api/experimental/arch/vector/reg_sync_control_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"tensor_api/experimental/arch/vector/reg_sync_control.h\"\" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_SYNC_CONTROL_IMPL_H
#endif

#ifndef IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_SYNC_CONTROL_IMPL_H
#define IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_SYNC_CONTROL_IMPL_H

#include "tensor_api/experimental/arch/vector/reg_sync_control.h"

namespace asc {
namespace te {
namespace experimental {
namespace detail {

template <mem_type src, mem_type dst>
struct mem_bar_mapping {
    static_assert(src != src, "Unsupported mem_bar source/destination combination");
};

template <>
struct mem_bar_mapping<mem_type::vec_store, mem_type::vec_load> {
    static constexpr MEM_TYPE value = VST_VLD;
};

template <>
struct mem_bar_mapping<mem_type::vec_load, mem_type::vec_store> {
    static constexpr MEM_TYPE value = VLD_VST;
};

template <>
struct mem_bar_mapping<mem_type::vec_store, mem_type::vec_store> {
    static constexpr MEM_TYPE value = VST_VST;
};

template <>
struct mem_bar_mapping<mem_type::vec_store, mem_type::scalar_load> {
    static constexpr MEM_TYPE value = VST_LD;
};

template <>
struct mem_bar_mapping<mem_type::vec_load, mem_type::scalar_store> {
    static constexpr MEM_TYPE value = VLD_ST;
};

template <>
struct mem_bar_mapping<mem_type::vec_store, mem_type::scalar_store> {
    static constexpr MEM_TYPE value = VST_ST;
};

template <>
struct mem_bar_mapping<mem_type::scalar_store, mem_type::vec_load> {
    static constexpr MEM_TYPE value = ST_VLD;
};

template <>
struct mem_bar_mapping<mem_type::scalar_load, mem_type::vec_store> {
    static constexpr MEM_TYPE value = LD_VST;
};

template <>
struct mem_bar_mapping<mem_type::scalar_store, mem_type::vec_store> {
    static constexpr MEM_TYPE value = ST_VST;
};

template <>
struct mem_bar_mapping<mem_type::vec_all, mem_type::vec_all> {
    static constexpr MEM_TYPE value = VV_ALL;
};

template <>
struct mem_bar_mapping<mem_type::vec_all, mem_type::scalar_all> {
    static constexpr MEM_TYPE value = VS_ALL;
};

template <>
struct mem_bar_mapping<mem_type::scalar_all, mem_type::vec_all> {
    static constexpr MEM_TYPE value = SV_ALL;
};

} // namespace detail

template <mem_type src, mem_type dst>
__simd_callee__ inline void mem_bar()
{
    asc_mem_bar(detail::mem_bar_mapping<src, dst>::value);
}

} // namespace experimental
} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_SYNC_CONTROL_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_SYNC_CONTROL_IMPL_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_SYNC_CONTROL_IMPL_H
#endif
