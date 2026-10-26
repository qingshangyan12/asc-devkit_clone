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
struct is_mem_bar_supported {
    static constexpr bool value = false;
};

} // namespace detail

template <mem_type src, mem_type dst>
__simd_callee__ inline void mem_bar()
{
    static_assert(detail::is_mem_bar_supported<src, dst>::value, "Unsupported mem_bar source/destination combination");
}

#define REGISTER_MEM_BAR(SRC, DST, VALUE)           \
    template <>                                     \
    struct detail::is_mem_bar_supported<SRC, DST> { \
        static constexpr bool value = true;         \
    };                                              \
    template <>                                     \
    __simd_callee__ inline void mem_bar<SRC, DST>() \
    {                                               \
        static constexpr auto value = VALUE;        \
        asc_mem_bar(value);                         \
    }

REGISTER_MEM_BAR(mem_type::vec_store, mem_type::vec_load, VST_VLD);
REGISTER_MEM_BAR(mem_type::vec_load, mem_type::vec_store, VLD_VST);
REGISTER_MEM_BAR(mem_type::vec_store, mem_type::vec_store, VST_VST);
REGISTER_MEM_BAR(mem_type::vec_store, mem_type::scalar_load, VST_LD);
REGISTER_MEM_BAR(mem_type::vec_load, mem_type::scalar_store, VLD_ST);
REGISTER_MEM_BAR(mem_type::vec_store, mem_type::scalar_store, VST_ST);
REGISTER_MEM_BAR(mem_type::scalar_store, mem_type::vec_load, ST_VLD);
REGISTER_MEM_BAR(mem_type::scalar_load, mem_type::vec_store, LD_VST);
REGISTER_MEM_BAR(mem_type::scalar_store, mem_type::vec_store, ST_VST);
REGISTER_MEM_BAR(mem_type::vec_all, mem_type::vec_all, VV_ALL);
REGISTER_MEM_BAR(mem_type::vec_all, mem_type::scalar_all, VS_ALL);
REGISTER_MEM_BAR(mem_type::scalar_all, mem_type::vec_all, SV_ALL);

#undef REGISTER_MEM_BAR

} // namespace experimental
} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_SYNC_CONTROL_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_SYNC_CONTROL_IMPL_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_SYNC_CONTROL_IMPL_H
#endif
