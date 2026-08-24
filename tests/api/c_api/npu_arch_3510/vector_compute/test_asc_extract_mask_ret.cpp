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
#include <mockcpp/mockcpp.hpp>
#include "tests/api/c_api/stub/cce_stub.h"
#include "c_api/reg_compute/compute/reg_mask.h"
#include "c_api/reg_compute/reg_copy.h"

class TestVectorComputeExtractMask : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void movvp_u16_RetStub(vector_bool& dst, vector_uint16_t src, int16_t part) { EXPECT_EQ(part, static_cast<int16_t>(22)); }

void movvp_u32_RetStub(vector_bool& dst, vector_uint32_t src, int16_t part) { EXPECT_EQ(part, static_cast<int16_t>(22)); }
} // namespace

TEST_F(TestVectorComputeExtractMask, ExtractMaskU16_RetSucc)
{
    vector_uint16_t src;
    int16_t part = static_cast<int16_t>(22);

    MOCKER_CPP(movvp, void(vector_bool&, vector_uint16_t, int16_t)).times(1).will(invoke(movvp_u16_RetStub));

    vector_bool result = asc_extract_mask(src, part);
    GlobalMockObject::verify();
}

TEST_F(TestVectorComputeExtractMask, ExtractMaskU32_RetSucc)
{
    vector_uint32_t src;
    int16_t part = static_cast<int16_t>(22);

    MOCKER_CPP(movvp, void(vector_bool&, vector_uint32_t, int16_t)).times(1).will(invoke(movvp_u32_RetStub));

    vector_bool result = asc_extract_mask(src, part);
    GlobalMockObject::verify();
}
