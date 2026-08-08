/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This file is a part of the CANN Open Software.
 * Licensed under CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <type_traits>

#include <gtest/gtest.h>

#include "c_api/stub/cce_stub.h"
#include "tensor_api/arch/vector/reg_cast.h"

namespace {

using asc::te::cast_layout;
using asc::te::cast_round_mode;
using asc::te::cast_sat_mode;

static_assert(asc::te::reg_cast_detail::reg_cast_op<half, float, cast_layout::zero,
    cast_round_mode::rint, cast_sat_mode::not_sat>::supported);
static_assert(asc::te::reg_cast_detail::reg_cast_op<float, half, cast_layout::zero,
    cast_round_mode::rint, cast_sat_mode::not_sat>::supported);
static_assert(asc::te::reg_cast_detail::reg_cast_op<half, float, cast_layout::one,
    cast_round_mode::rint, cast_sat_mode::sat>::supported);
static_assert(!asc::te::reg_cast_detail::reg_cast_op<half, float, cast_layout::two,
    cast_round_mode::rint, cast_sat_mode::not_sat>::supported);

template <typename To, typename From>
__aicore__ inline void CompileRegCast(vector_bool mask)
{
    asc::te::reg_tensor<From> src {};
    src.mask = mask;
    auto dst = asc::te::cast<To>(src);
    static_assert(std::is_same_v<decltype(dst), asc::te::reg_tensor<To>>);
}

TEST(TestTensorApiRegCast, CompilesSupportedPolicies)
{
    auto widen = &CompileRegCast<float, half>;
    auto narrow = &CompileRegCast<half, float>;
    (void)widen;
    (void)narrow;
}

} // namespace
