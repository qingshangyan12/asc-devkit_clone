/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <cstdint>
#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include "tests/api/c_api/stub/cce_stub.h"
#include "include/c_api/asc_simd.h"

namespace {
int64_t clz_stub(uint64_t value) { return value == 0 ? 64 : __builtin_clzll(value); }

int64_t bcnt1_stub(uint64_t value) { return __builtin_popcountll(value); }

template <typename T>
void CheckBoundaryParameters()
{
    struct Case {
        T divisor;
        T magic;
        T shift;
    };
    constexpr T bits = sizeof(T) * 8;
    constexpr T half = T{1} << (bits - 1);
    const Case cases[] = {
        {1, 1, 0}, {2, 1, 1},
        {half >> 1, 1, bits - 2},
        {(half >> 1) + 1, static_cast<T>(~T{0}) - 6, bits - 1},
        {half - 2, 5, bits - 1}, {half - 1, 3, bits - 1},
    };
    for (const auto& item : cases) {
        SCOPED_TRACE(item.divisor);
        T magic = 0;
        T shift = 0;
        MOCKER_CPP(clz, int64_t(uint64_t)).times(1).will(invoke(clz_stub));
        MOCKER_CPP(bcnt1, int64_t(uint64_t)).times(1).will(invoke(bcnt1_stub));
        asc_get_uintdiv_magic_and_shift(&magic, &shift, item.divisor);
        EXPECT_EQ(magic, item.magic);
        EXPECT_EQ(shift, item.shift);
        GlobalMockObject::verify();
    }
}
} // namespace

TEST(TestGetUintDivMagicAndShift, Uint32Boundaries)
{
    CheckBoundaryParameters<uint32_t>();
}

TEST(TestGetUintDivMagicAndShift, Uint64Boundaries)
{
    CheckBoundaryParameters<uint64_t>();
}

TEST(TestGetUintDivMagicAndShift, Uint32)
{
    uint32_t magic = 0;
    uint32_t shift = 0;
    MOCKER_CPP(clz, int64_t(uint64_t)).times(1).will(invoke(clz_stub));
    MOCKER_CPP(bcnt1, int64_t(uint64_t)).times(1).will(invoke(bcnt1_stub));
    asc_get_uintdiv_magic_and_shift(&magic, &shift, static_cast<uint32_t>(7));
    EXPECT_EQ(magic, 613566757U);
    EXPECT_EQ(shift, 3U);
    GlobalMockObject::verify();
}

TEST(TestGetUintDivMagicAndShift, Uint64)
{
    uint64_t magic = 0;
    uint64_t shift = 0;
    MOCKER_CPP(clz, int64_t(uint64_t)).times(1).will(invoke(clz_stub));
    MOCKER_CPP(bcnt1, int64_t(uint64_t)).times(1).will(invoke(bcnt1_stub));
    asc_get_uintdiv_magic_and_shift(&magic, &shift, static_cast<uint64_t>(7));
    EXPECT_EQ(magic, UINT64_C(2635249153387078803));
    EXPECT_EQ(shift, UINT64_C(3));
    GlobalMockObject::verify();
}
