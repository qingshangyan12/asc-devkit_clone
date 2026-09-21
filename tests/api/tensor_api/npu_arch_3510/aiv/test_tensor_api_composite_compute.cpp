/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <gtest/gtest.h>

#include "tensor_api/stub/cce_stub.h"
#include "tensor_api/experimental/vector_compute.h"
#include "tensor_api/tensor.h"

namespace {

template <typename T>
using reg_type = asc::te::experimental::reg_tensor<T>;

template <typename T>
__aicore__ inline void check_common_composite()
{
    static_assert(AscendC::Std::is_same_v<
                  decltype(asc::te::experimental::axpy(
                      AscendC::Std::declval<reg_type<T>&>(), AscendC::Std::declval<const reg_type<T>&>(),
                      AscendC::Std::declval<const T&>())),
                  reg_type<T>>);
    static_assert(AscendC::Std::is_same_v<
                  decltype(asc::te::experimental::abs_diff(
                      AscendC::Std::declval<const reg_type<T>&>(), AscendC::Std::declval<const reg_type<T>&>())),
                  reg_type<T>>);
    reg_type<T> dst{};
    reg_type<T> src{};
    dst.mask = asc::te::experimental::all_mask<T>().reg;
    T scalar{};
    auto axpy_result = asc::te::experimental::axpy(dst, src, scalar);
    auto abs_diff_result = asc::te::experimental::abs_diff(dst, src);
    EXPECT_EQ(axpy_result.mask, dst.mask);
    EXPECT_EQ(abs_diff_result.mask, dst.mask);
}

template <typename T>
__aicore__ inline void check_float_fused()
{
    static_assert(AscendC::Std::is_same_v<
                  decltype(asc::te::experimental::madd(
                      AscendC::Std::declval<reg_type<T>&>(), AscendC::Std::declval<const reg_type<T>&>(),
                      AscendC::Std::declval<const reg_type<T>&>())),
                  reg_type<T>>);
    static_assert(AscendC::Std::is_same_v<
                  decltype(asc::te::experimental::fma(
                      AscendC::Std::declval<const reg_type<T>&>(), AscendC::Std::declval<const reg_type<T>&>(),
                      AscendC::Std::declval<const reg_type<T>&>())),
                  reg_type<T>>);

    reg_type<T> dst{};
    reg_type<T> src0{};
    reg_type<T> src1{};
    dst.mask = asc::te::experimental::all_mask<T>().reg;
    auto madd_result = asc::te::experimental::madd(dst, src0, src1);
    auto fma_result = asc::te::experimental::fma(dst, src0, src1);
    EXPECT_EQ(madd_result.mask, dst.mask);
    EXPECT_EQ(fma_result.mask, dst.mask);
}

template <typename T>
__aicore__ inline void check_mula()
{
    static_assert(AscendC::Std::is_same_v<
                  decltype(asc::te::experimental::mula(
                      AscendC::Std::declval<reg_type<T>&>(), AscendC::Std::declval<const reg_type<T>&>(),
                      AscendC::Std::declval<const reg_type<T>&>())),
                  reg_type<T>>);

    reg_type<T> dst{};
    reg_type<T> src0{};
    reg_type<T> src1{};
    dst.mask = asc::te::experimental::all_mask<T>().reg;
    auto result = asc::te::experimental::mula(dst, src0, src1);
    EXPECT_EQ(result.mask, dst.mask);
}

__aicore__ inline void check_exp_diff_positions_and_muls_cast()
{
    static_assert(
        AscendC::Std::is_same_v<
            decltype(asc::te::experimental::exp_diff<float, float>(
                AscendC::Std::declval<const reg_type<float>&>(), AscendC::Std::declval<const reg_type<float>&>())),
            reg_type<float>>);
    static_assert(AscendC::Std::is_same_v<
                  decltype(asc::te::experimental::exp_diff<float, half>(
                      AscendC::Std::declval<const reg_type<half>&>(), AscendC::Std::declval<const reg_type<half>&>(),
                      ASC_POSITION_EVEN)),
                  reg_type<float>>);
    static_assert(AscendC::Std::is_same_v<
                  decltype(asc::te::experimental::exp_diff<float, half>(
                      AscendC::Std::declval<const reg_type<half>&>(), AscendC::Std::declval<const reg_type<half>&>(),
                      ASC_POSITION_ODD)),
                  reg_type<float>>);
    static_assert(AscendC::Std::is_same_v<
                  decltype(asc::te::experimental::muls_cast<asc::te::experimental::cast_layout::zero, half>(
                      AscendC::Std::declval<const reg_type<float>&>(), AscendC::Std::declval<const float&>())),
                  reg_type<half>>);
    static_assert(AscendC::Std::is_same_v<
                  decltype(asc::te::experimental::muls_cast<asc::te::experimental::cast_layout::one, half>(
                      AscendC::Std::declval<const reg_type<float>&>(), AscendC::Std::declval<const float&>())),
                  reg_type<half>>);

    reg_type<half> half_src0{};
    reg_type<half> half_src1{};
    auto output_mask = asc::te::experimental::make_mask<asc::te::experimental::mask_pattern::vl3, float>();
    half_src0.mask = output_mask.reg;
    auto even_exp_diff = asc::te::experimental::exp_diff<float, half>(half_src0, half_src1, ASC_POSITION_EVEN);
    auto odd_exp_diff = asc::te::experimental::exp_diff<float, half>(half_src0, half_src1, ASC_POSITION_ODD);
    EXPECT_EQ(even_exp_diff.mask, output_mask.reg);
    EXPECT_EQ(odd_exp_diff.mask, output_mask.reg);

    reg_type<float> float_src{};
    float_src.mask = asc::te::experimental::all_mask<float>().reg;
    auto float_exp_diff = asc::te::experimental::exp_diff<float, float>(float_src, float_src);
    EXPECT_EQ(float_exp_diff.mask, float_src.mask);
    auto zero_result =
        asc::te::experimental::muls_cast<asc::te::experimental::cast_layout::zero, half>(float_src, 1.0f);
    auto one_result = asc::te::experimental::muls_cast<asc::te::experimental::cast_layout::one, half>(float_src, 1.0f);
    EXPECT_EQ(zero_result.mask, float_src.mask);
    EXPECT_EQ(one_result.mask, float_src.mask);
}

TEST(test_tensor_api_composite_compute, compile_public_apis_and_supported_types)
{
    check_common_composite<half>();
    check_common_composite<float>();

    check_float_fused<half>();
    check_float_fused<bfloat16_t>();
    check_float_fused<float>();

    check_mula<int16_t>();
    check_mula<uint16_t>();
    check_mula<half>();
    check_mula<bfloat16_t>();
    check_mula<int32_t>();
    check_mula<uint32_t>();
    check_mula<float>();

    check_exp_diff_positions_and_muls_cast();
}

} // namespace
