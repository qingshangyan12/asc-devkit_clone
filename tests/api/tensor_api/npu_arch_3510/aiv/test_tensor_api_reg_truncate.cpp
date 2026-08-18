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

template <typename T>
using truncate_result = decltype(asc::te::trunc(AscendC::Std::declval<asc::te::reg_tensor<T>>()));

static_assert(asc::te::detail::is_supported<half>::value);
static_assert(asc::te::detail::is_supported<bfloat16_t>::value);
static_assert(asc::te::detail::is_supported<float>::value);
static_assert(!asc::te::detail::is_supported<int32_t>::value);
static_assert(AscendC::Std::is_same_v<truncate_result<half>, asc::te::reg_tensor<half>>);
static_assert(AscendC::Std::is_same_v<truncate_result<bfloat16_t>, asc::te::reg_tensor<bfloat16_t>>);
static_assert(AscendC::Std::is_same_v<truncate_result<float>, asc::te::reg_tensor<float>>);

class tensor_api_reg_truncate_3510 : public testing::Test {
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

template <typename T>
void expect_truncate_result(vector_bool mask)
{
    asc::te::reg_tensor<T> src {};
    for (size_t i = 0; i < src.reg.size(); ++i) {
        src.reg[i] = static_cast<T>(i % 2 == 0 ? 3.75F : -2.5F);
    }
    src.mask = mask;
    auto dst = asc::te::trunc(src);

    EXPECT_EQ(dst.mask, mask);
    for (size_t i = 0; i < dst.reg.size(); ++i) {
        const float expected = i % 2 == 0 ? 3.0F : -2.0F;
        EXPECT_FLOAT_EQ(static_cast<float>(dst.reg[i]), expected);
    }
}

TEST_F(tensor_api_reg_truncate_3510, truncates_half_and_preserves_mask)
{
    expect_truncate_result<half>(pset_b16(PAT_ALL));
}

TEST_F(tensor_api_reg_truncate_3510, truncates_bfloat16_and_preserves_mask)
{
    expect_truncate_result<bfloat16_t>(pset_b16(PAT_ALL));
}

TEST_F(tensor_api_reg_truncate_3510, truncates_float_and_preserves_mask)
{
    expect_truncate_result<float>(pset_b32(PAT_ALL));
}

} // namespace
