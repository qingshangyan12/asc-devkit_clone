/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_TENSOR_API_ARCH_CUBE_GM_TO_L1_COPY_IMPL_SOURCE_GEOMETRY_H
#define IMPL_TENSOR_API_ARCH_CUBE_GM_TO_L1_COPY_IMPL_SOURCE_GEOMETRY_H

#include "impl/tensor_api/utils/utils_impl.h"

namespace asc {
namespace te {

template <
    typename SrcTensor, typename Pattern, auto FirstAxis, auto SecondAxis, auto StrideAxis,
    bool FlattenFirstAxis = false, typename SrcLayout, typename First, typename Second, typename Stride>
__aicore__ inline void read_gm_to_l1_source_geometry(
    const SrcLayout& src_layout, First& first, Second& second, Stride& stride)
{
    if constexpr (is_satisfied_ptn_format_v<SrcTensor, Pattern>) {
        first = get_element<attr_info::shape, FirstAxis>(src_layout);
        second = get_element<attr_info::shape, SecondAxis>(src_layout);
        stride = get_element<attr_info::stride, StrideAxis>(src_layout);
    } else {
        if constexpr (FlattenFirstAxis) {
            first = get_element<attr_info::shape, FirstAxis, 1>(src_layout) *
                    get_element<attr_info::shape, FirstAxis, 0>(src_layout);
        } else {
            first = get_element<attr_info::shape, FirstAxis, 1>(src_layout);
        }
        second = get_element<attr_info::shape, SecondAxis, 1>(src_layout);
        stride = get_element<attr_info::stride, StrideAxis, 1>(src_layout);
    }
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_CUBE_GM_TO_L1_COPY_IMPL_SOURCE_GEOMETRY_H
