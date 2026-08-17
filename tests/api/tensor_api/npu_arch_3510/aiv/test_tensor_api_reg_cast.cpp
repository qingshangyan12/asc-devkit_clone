/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This file is a part of the CANN Open Software.
 * Licensed under CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <cstddef>
#include <type_traits>
#include <utility>

#include <gtest/gtest.h>

#include "c_api/stub/cce_stub.h"

#include "tensor_api/arch/vector/experimental/type_conversion.h"

namespace {

using asc::te::cast_layout;
using asc::te::cast_round_mode;
using asc::te::cast_sat_mode;

static_assert(asc::te::detail::reg_cast_op<float, half, cast_layout::zero,
    cast_round_mode::rint, cast_sat_mode::not_sat>::supported);
static_assert(asc::te::detail::reg_cast_op<half, float, cast_layout::one,
    cast_round_mode::ceil, cast_sat_mode::sat>::supported);
static_assert(asc::te::detail::reg_cast_op<uint8_t, int32_t, cast_layout::two,
    cast_round_mode::rint, cast_sat_mode::sat>::supported);
static_assert(!asc::te::detail::reg_cast_op<float, half, cast_layout::two,
    cast_round_mode::rint, cast_sat_mode::not_sat>::supported);

using half_reg = asc::te::reg_tensor<half>;
static_assert(AscendC::Std::is_same_v<decltype(asc::te::cast<float>(AscendC::Std::declval<half_reg>())),
    asc::te::reg_tensor<float>>);

class tensor_api_reg_cast_3510 : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}

    void SetUp() override
    {
        AscendC::SetGCoreType(2);
    }

    void TearDown() override
    {
        AscendC::SetGCoreType(0);
    }
};

TEST_F(tensor_api_reg_cast_3510, default_policy_converts_half_to_float_and_preserves_mask)
{
    asc::te::reg_tensor<half> src {};
    src.mask = pset_b16(PAT_ALL);
    asc_duplicate_scalar(src.reg, static_cast<half>(3.5F), src.mask);
    auto dst = asc::te::cast<float>(src);

    EXPECT_EQ(dst.mask, src.mask);
    for (float value : dst.reg) {
        EXPECT_FLOAT_EQ(value, 3.5F);
    }
}

TEST_F(tensor_api_reg_cast_3510, explicit_policy_converts_float_to_half_with_layout_one_and_saturation)
{
    asc::te::reg_tensor<float> src {};
    src.mask = pset_b32(PAT_ALL);
    asc_duplicate_scalar(src.reg, 70000.0F, src.mask);
    auto dst = asc::te::cast<half, cast_layout::one, cast_round_mode::ceil,
        cast_sat_mode::sat>(src);

    EXPECT_EQ(dst.mask, src.mask);
    size_t converted_count = 0;
    size_t zeroed_count = 0;
    for (half value : dst.reg) {
        const float actual = static_cast<float>(value);
        converted_count += actual == 65504.0F;
        zeroed_count += actual == 0.0F;
    }
    EXPECT_EQ(converted_count, src.reg.size());
    EXPECT_EQ(zeroed_count, dst.reg.size() - src.reg.size());
}

TEST_F(tensor_api_reg_cast_3510, converts_int32_to_float)
{
    asc::te::reg_tensor<int32_t> src {};
    src.mask = pset_b32(PAT_ALL);
    asc_duplicate_scalar(src.reg, -42, src.mask);
    auto dst = asc::te::cast<float>(src);

    EXPECT_EQ(dst.mask, src.mask);
    for (float value : dst.reg) {
        EXPECT_FLOAT_EQ(value, -42.0F);
    }
}

} // namespace
