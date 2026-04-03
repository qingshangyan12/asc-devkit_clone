/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include <gtest/gtest.h>
#include <type_traits>
#include "simt_compiler_stub.h"
#include "simt_api/asc_bf16.h"
using namespace std;
using namespace AscendC;
using namespace AscendC::Simt;

// ================================ Test bfloat16_t start ================================
struct CmpParams_bfloat16t {
    int32_t mode;
};

class CmpTestsuite_bfloat16t : public testing::Test, public testing::WithParamInterface<CmpParams_bfloat16t> {
protected:
    void SetUp() {}
    void TearDown() {}
};


INSTANTIATE_TEST_CASE_P(CmpTestCase_bfloat16t, CmpTestsuite_bfloat16t,
    ::testing::Values(CmpParams_bfloat16t {0}));

TEST_P(CmpTestsuite_bfloat16t, CmpTestCase_bfloat16t)
{
    auto param = GetParam();
    int32_t mode = mode = param.mode;
    int fpByteSize = 4;
    int shapeSize = 128;

    uint8_t dstGm[shapeSize * fpByteSize] = {0};
    uint8_t srcGm[shapeSize * fpByteSize] = {0};
    uint8_t src1Gm[shapeSize * fpByteSize] = {0};
}
// ================================ Test bfloat16_t end ==================================