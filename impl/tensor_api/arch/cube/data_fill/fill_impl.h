/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and limitations under the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning \
    "impl/tensor_api/arch/cube/data_fill/fill_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use \"#include \"tensor_api/arch/cube/fill.h\" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file fill_impl.h
 * \brief Cube L1 fill implementation.
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_DATA_FILL_FILL_IMPL_H
#define IMPL_TENSOR_API_ARCH_CUBE_DATA_FILL_FILL_IMPL_H

#include "c_api/asc_simd.h"

namespace asc {
namespace te {
namespace detail {

template <typename To, typename From>
__aicore__ inline To fill_bit_cast(From value)
{
    static_assert(sizeof(To) == sizeof(From), "Fill bit-cast size mismatch");
    To result{};
    auto* out = reinterpret_cast<unsigned char*>(&result);
    const auto* in = reinterpret_cast<const unsigned char*>(&value);
    for (uint32_t i = 0; i < sizeof(To); ++i) {
        out[i] = in[i];
    }
    return result;
}

} // namespace detail

template <typename Tensor, typename T>
__aicore__ inline void fill(const Tensor& dst, const fill_params<T>& params)
{
    static_assert(
        Std::is_same_v<typename Tensor::data_type, T>, "Fill value must match the writable destination element type");
    static_assert(is_hardware_mem_v<location::l1, Tensor>, "Fill requires an L1 tensor on Ascend 950");
    static_assert(
        Std::is_same_v<T, half> || Std::is_same_v<T, bfloat16_t> || Std::is_same_v<T, int16_t> ||
            Std::is_same_v<T, uint16_t> || Std::is_same_v<T, int32_t> || Std::is_same_v<T, uint32_t> ||
            Std::is_same_v<T, float>,
        "Unsupported Fill element type");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ != 3510)
    static_assert(sizeof(T) == 0, "This Fill implementation requires dav-3510");
#endif
    if (params.repeat_times > 32767 || params.block_num > 32767 || params.dst_gap > 32767) {
        __builtin_trap();
    }
    if (params.repeat_times == 0 || params.block_num == 0) {
        return;
    }

    asc_fill_value_config config{};
    config.repeat = params.repeat_times;
    config.blk_num = params.block_num;
    config.dst_gap = params.dst_gap;
    if constexpr (Std::is_same_v<T, half> || Std::is_same_v<T, bfloat16_t>) {
        asc_fill_l1(dst.data().get(), params.init_value, config);
    } else if constexpr (sizeof(T) == 2) {
        asc_fill_l1(dst.data().get(), detail::fill_bit_cast<half>(params.init_value), config);
    } else {
        asc_fill_l1(dst.data().get(), detail::fill_bit_cast<uint32_t>(params.init_value), config);
    }
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_CUBE_DATA_FILL_FILL_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
