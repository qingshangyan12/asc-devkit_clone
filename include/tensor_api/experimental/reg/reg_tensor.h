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
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif

#ifndef INCLUDE_TENSOR_API_EXPERIMENTAL_REG_REG_TENSOR_H
#define INCLUDE_TENSOR_API_EXPERIMENTAL_REG_REG_TENSOR_H

#include "impl/tensor_api/experimental/reg/type_get.h"

namespace asc {
namespace te {

template <typename DataType>
struct reg_tensor;

template <>
struct reg_tensor<bool> {
    using type = bool;
    using elem_type = bool;
    using reg_type = typename detail::type_get<bool>::type;

    __simd_callee__ reg_tensor() = default;
    __simd_callee__ reg_tensor(reg_type regValue) : reg(regValue) {}

    reg_type reg;
};

template <typename DataType>
struct reg_tensor {
    using type = DataType;
    using elem_type = DataType;
    using reg_type = typename detail::type_get<DataType>::type;

    __simd_callee__ reg_tensor() = default;
    __simd_callee__ reg_tensor(vector_bool maskValue) : mask(maskValue) {}

    template <typename ScalarType>
    __simd_callee__ inline reg_tensor& operator=(ScalarType src)
    {
        asc_duplicate_scalar(reg, static_cast<DataType>(src), mask);
        return *this;
    }

    __simd_callee__ inline reg_tensor& with_mask(const reg_tensor<bool>& maskReg)
    {
        mask = maskReg.reg;
        return *this;
    }

    reg_type reg;
    vector_bool mask;
};

} // namespace te
} // namespace asc

#endif // INCLUDE_TENSOR_API_EXPERIMENTAL_REG_REG_TENSOR_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif
