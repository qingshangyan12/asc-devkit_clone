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
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_FILL_H
#endif

/*!
 * \file fill.h
 * \brief Cube L1 physical-block fill interface.
 */
#ifndef INCLUDE_TENSOR_API_ARCH_CUBE_FILL_H
#define INCLUDE_TENSOR_API_ARCH_CUBE_FILL_H

namespace asc {
namespace te {

/** @brief Physical L1 fill configuration. Blocks and gaps are measured in 32 bytes. */
template <typename T>
struct fill_params {
    uint16_t repeat_times = 0;
    uint16_t block_num = 0;
    uint16_t dst_gap = 0;
    T init_value = {};
};

/** @brief Initialize physical L1 blocks on Ascend 950 AIC. */
template <typename Tensor, typename T>
__aicore__ inline void fill(const Tensor& dst, const fill_params<T>& params);

} // namespace te
} // namespace asc

#include "impl/tensor_api/arch/cube/data_fill/fill_impl.h"

#endif // INCLUDE_TENSOR_API_ARCH_CUBE_FILL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_FILL_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_FILL_H
#endif
