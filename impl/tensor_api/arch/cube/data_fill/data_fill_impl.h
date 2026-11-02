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
    "impl/tensor_api/arch/cube/data_fill/data_fill_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use \"#include \"tensor_api/arch/cube/data_fill/data_fill.h\" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_IMPL_TENSOR_API_ARCH_CUBE_DATA_FILL_DATA_FILL_IMPL_H
#endif

/*!
 * \file data_fill_impl.h
 * \brief Cube L1 fill implementation.
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_DATA_FILL_DATA_FILL_IMPL_H
#define IMPL_TENSOR_API_ARCH_CUBE_DATA_FILL_DATA_FILL_IMPL_H

#include "impl/tensor_api/utils/macro_impl.h"
#include "tensor_api/utils/utils.h"

namespace asc {
namespace te {
template <typename Tensor, typename T>
__aicore__ inline void fill(const Tensor& dst, const fill_params<T>& params)
{
    static_assert(
        Std::is_same_v<typename Tensor::data_type, T>, "Fill value must match the writable destination element type");
    using dst_pos = get_mem_location<Tensor>;
    static_assert(Std::is_same_v<dst_pos, location::l1>, "Fill requires the destination tensor to be located in L1.");
    static_assert(
        Std::is_same_v<T, half> || Std::is_same_v<T, bfloat16_t> || Std::is_same_v<T, int16_t> ||
            Std::is_same_v<T, uint16_t> || Std::is_same_v<T, int32_t> || Std::is_same_v<T, uint32_t> ||
            Std::is_same_v<T, float>,
        "Unsupported Fill element type");

    asc_fill_value_config config{};
    config.repeat = params.repeat_times;
    config.blk_num = params.block_num;
    config.dst_gap = params.dst_gap;
    if constexpr (Std::is_same_v<T, half> || Std::is_same_v<T, bfloat16_t>) {
        asc_fill_l1(dst.data().get(), params.init_value, config);
    } else if constexpr (sizeof(T) == 2) {
        asc_fill_l1(dst.data().get(), static_cast<half>(params.init_value), config);
    } else {
        asc_fill_l1(dst.data().get(), static_cast<uint32_t>(params.init_value), config);
    }
}

template <typename Tensor, typename T>
__aicore__ inline void fill(const Tensor& dst, const T& init_value)
{
    constexpr uint32_t block_bytes = 32;
    const auto tensor_bytes = dst.capacity() * sizeof(typename Tensor::data_type);
    fill(dst, fill_params<T>{1, static_cast<uint16_t>(tensor_bytes / block_bytes), 0, init_value});
}
} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_CUBE_DATA_FILL_DATA_FILL_IMPL_H

#if defined( \
    UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_IMPL_TENSOR_API_ARCH_CUBE_DATA_FILL_DATA_FILL_IMPL_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_IMPL_TENSOR_API_ARCH_CUBE_DATA_FILL_DATA_FILL_IMPL_H
#endif
