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
    "impl/tensor_api/arch/vector/experimental/histogram_compute_impl.h is internal and must not be used directly."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_HISTOGRAM_COMPUTE_IMPL
#endif

/*!
 * \file histogram_compute_impl.h
 * \brief Register histogram compute API implementations for asc::te.
 */
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_HISTOGRAM_COMPUTE_IMPL_H
#define IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_HISTOGRAM_COMPUTE_IMPL_H

#include "impl/tensor_api/experimental/reg/reg_tensor_impl.h"

namespace asc {
namespace te {

template <histogram_bin Bin, histogram_type Type>
__simd_callee__ inline reg_tensor<uint16_t> histograms(
    reg_tensor<uint16_t> dst, reg_tensor<uint8_t> src)
{
    static_assert(Bin == histogram_bin::bin0 || Bin == histogram_bin::bin1,
        "histograms supports histogram_bin::bin0 and histogram_bin::bin1");
    static_assert(Type == histogram_type::frequency || Type == histogram_type::accumulate,
        "histograms supports histogram_type::frequency and histogram_type::accumulate");

    if constexpr (Type == histogram_type::frequency) {
        if constexpr (Bin == histogram_bin::bin0) {
            asc_frequency_histogram_bin0(dst.reg, src.reg, src.mask);
        } else {
            asc_frequency_histogram_bin1(dst.reg, src.reg, src.mask);
        }
    } else {
        if constexpr (Bin == histogram_bin::bin0) {
            asc_cumulative_histogram_bin0(dst.reg, src.reg, src.mask);
        } else {
            asc_cumulative_histogram_bin1(dst.reg, src.reg, src.mask);
        }
    }
    return dst;
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_HISTOGRAM_COMPUTE_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_HISTOGRAM_COMPUTE_IMPL)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_HISTOGRAM_COMPUTE_IMPL
#endif
