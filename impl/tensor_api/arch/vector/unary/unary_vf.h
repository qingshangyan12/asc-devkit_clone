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
    "impl/tensor_api/arch/vector/unary/unary_vf.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file unary_vf.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_UNARY_UNARY_VF_H
#define IMPL_TENSOR_API_ARCH_VECTOR_UNARY_UNARY_VF_H

#include "impl/tensor_api/arch/vector/unary/instruction.h"
#include "impl/tensor_api/arch/vector/utils/mask_utils.h"

namespace asc {
namespace te {

template <typename CalcFunc, typename TraitType>
class unary_vf {
public:
    template <typename RegType, typename T>
    __simd_vf__ inline static void run(__ubuf__ T* dst, __ubuf__ T* src, uint32_t data_size)
    {
        constexpr uint32_t one_rep_size = static_cast<uint32_t>(asc_get_vf_len() / sizeof(T));
        uint16_t repeat = static_cast<uint16_t>((data_size + one_rep_size - 1) / one_rep_size);

        vector_bool vmask;
        RegType reg_src;
        RegType reg_dst;

        for (uint16_t i = 0; i < repeat; ++i) {
            vmask = inst::update_mask::template run<uint32_t>(data_size);
            asc_loadalign(reg_src, src + i * one_rep_size);
            CalcFunc::template run<RegType>(reg_dst, reg_src, vmask);
            asc_storealign(dst + i * one_rep_size, reg_dst, vmask);
        }
    }
};

template <typename CalcFunc, typename TraitType>
class transform2_unary_vf {
public:
    template <typename T, typename U>
    __aicore__ inline static void run(const T& dst, const U& src)
    {
        using type = get_attribute_element_type<typename T::elementType*>;
        using RegType = typename vector_type_transform::template Get<type>;
        uint32_t data_size = dst.size();

        unary_vf<CalcFunc, TraitType>::template run<RegType, type>(dst.data().get(), src.data().get(), data_size);
    }
};

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_VECTOR_UNARY_UNARY_VF_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
