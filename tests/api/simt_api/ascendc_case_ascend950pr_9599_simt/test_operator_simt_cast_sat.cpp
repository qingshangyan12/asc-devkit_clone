/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <cstdint>
#include <gtest/gtest.h>
#include "simt_stub.h"
#include "simt_api/device_types.h"

namespace {
enum class ROUND { R, A, F, C, Z, O, H};

enum class RoundMode : uint8_t {
    CAST_NONE = 0,
    CAST_RINT, // round
    CAST_FLOOR,
    CAST_CEIL,
    CAST_ROUND,  // away-zero
    CAST_TRUNC,  // to-zero
    CAST_ODD,    // Von Neumann rounding
    CAST_HYBRID, // hybrid round
    CAST_EVEN,
    CAST_ZERO,
    UNKNOWN = 0xFF,
};

template <typename... Arg>
struct Tuple {};
}

#undef ASCENDC_CPU_DEBUG
#include "impl/simt_api/cpp/dav_c310/kernel_simt_cast_sat_impl.h"

class CastSatTestsuite : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

TEST_F(CastSatTestsuite, CastTestCase)
{
    uint32_t x = 1;
    int64_t ret = AscendC::Simt::CastSat<RoundMode::CAST_RINT>(x);
}