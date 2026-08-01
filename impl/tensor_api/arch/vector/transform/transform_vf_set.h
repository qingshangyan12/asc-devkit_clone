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
    "impl/tensor_api/arch/vector/transform/transform_vf_set.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file transform_vf_set.h
 * \brief Set definitions for instruction-to-wrapper mapping
 */
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_TRANSFORM_TRANSFORM_VF_SET_H
#define IMPL_TENSOR_API_ARCH_VECTOR_TRANSFORM_TRANSFORM_VF_SET_H

#include "impl/tensor_api/arch/vector/axpy/axpy_vf.h"
#include "impl/tensor_api/arch/vector/binary/binary_vf.h"
#include "impl/tensor_api/arch/vector/binary_scalar/binary_scalar_vf.h"
#include "impl/tensor_api/arch/vector/cast/cast_vf.h"
#include "impl/tensor_api/arch/vector/dual/dual_vf.h"
#include "impl/tensor_api/arch/vector/unary/unary_vf.h"
#include "impl/tensor_api/utils/map_impl.h"

namespace asc {
namespace te {

template <typename CalcFunc, typename TraitType>
struct transform2_vf_set {
    using binarySet =
        Std::tuple<inst::add, inst::sub, inst::madd, inst::bitwise_and, inst::select, inst::abs_sub, inst::min,
                   inst::max, inst::bitwise_or, inst::mul, inst::exp_sub_even, inst::exp_sub_odd, inst::shift_left,
                   inst::shift_right, inst::div, inst::bitwise_xor, inst::prelu>;
    using castSet = Std::tuple<inst::cast, inst::Ceil, inst::Rint, inst::Round, inst::Trunc>;
    using binaryScalarSet =
        Std::tuple<inst::mul_scalar, inst::add_scalar, inst::max_scalar, inst::min_scalar, inst::shift_left_scalar,
                   inst::shift_right_scalar, inst::muls_even, inst::muls_odd>;
    using unarySet = Std::tuple<inst::bitwise_not, inst::ln, inst::abs, inst::sqrt, inst::exp>;
    using axpySet = Std::tuple<inst::axpy>;
    using dualSet = Std::tuple<inst::mull>;

    using transformSet = TupleMap<Std::tuple<binarySet, transform2_binary_vf<CalcFunc, TraitType>>,
                                  Std::tuple<castSet, transform2_cast_inst_match<CalcFunc, TraitType>>,
                                  Std::tuple<binaryScalarSet, transform2_binary_scalar_vf<CalcFunc, TraitType>>,
                                  Std::tuple<unarySet, transform2_unary_vf<CalcFunc, TraitType>>,
                                  Std::tuple<axpySet, transform2_axpy_vf<CalcFunc, TraitType>>,
                                  Std::tuple<dualSet, transform2_dual_vf<CalcFunc, TraitType>>>;
};

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_VECTOR_TRANSFORM_TRANSFORM_VF_SET_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
