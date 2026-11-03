/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License"). You may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and limitations under the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning \
    "impl/tensor_api/arch/vector/copy_pad_val/set_copy_pad_val_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use \"#include \"tensor_api/arch/vector/set_copy_pad_val.h\" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_IMPL_TENSOR_API_ARCH_VECTOR_COPY_PAD_VAL_SET_COPY_PAD_VAL_IMPL_H
#endif

/*!
 * \file set_copy_pad_val_impl.h
 * \brief Vector GM-to-UB padding value implementation.
 */
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_COPY_PAD_VAL_SET_COPY_PAD_VAL_IMPL_H
#define IMPL_TENSOR_API_ARCH_VECTOR_COPY_PAD_VAL_SET_COPY_PAD_VAL_IMPL_H

#include "impl/tensor_api/utils/macro_impl.h"
#include "tensor_api/utils/utils.h"

namespace asc {
namespace te {

template <typename T>
__aicore__ inline void set_copy_pad_val(T pad_value)
{
    static_assert(
        Std::is_same_v<T, int8_t> || Std::is_same_v<T, uint8_t> || Std::is_same_v<T, int16_t> ||
            Std::is_same_v<T, uint16_t> || Std::is_same_v<T, half> || Std::is_same_v<T, bfloat16_t> ||
            Std::is_same_v<T, int32_t> || Std::is_same_v<T, uint32_t> || Std::is_same_v<T, float>,
        "The GM-to-UB padding value does not support this data type.");
    asc_set_copy_pad_val(pad_value);
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_VECTOR_COPY_PAD_VAL_SET_COPY_PAD_VAL_IMPL_H

#if defined( \
    UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_IMPL_TENSOR_API_ARCH_VECTOR_COPY_PAD_VAL_SET_COPY_PAD_VAL_IMPL_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_IMPL_TENSOR_API_ARCH_VECTOR_COPY_PAD_VAL_SET_COPY_PAD_VAL_IMPL_H
#endif
