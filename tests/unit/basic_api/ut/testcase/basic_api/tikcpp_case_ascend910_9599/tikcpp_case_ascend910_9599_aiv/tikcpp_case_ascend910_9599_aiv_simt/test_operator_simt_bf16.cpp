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
#include <random>
#include "simt_compiler_stub.h"
#include "kernel_operator.h"
#include "simt_bf16.h"

using namespace std;
using namespace AscendC;
using namespace AscendC::Simt;

struct BF16Params {
    int32_t mode;
};

class BF16Testsuite : public testing::Test, public testing::WithParamInterface<BF16Params> {
protected:
    void SetUp() {}
    void TearDown() {}
};

TEST_F(BF16Testsuite, MathApiTest)
{
    float x = static_cast<float>(rand()) / RAND_MAX;
    bfloat16_t x_h = bfloat16_t(x);

    bfloat16_t result = hcos(x_h);
    bfloat16_t expect = bfloat16_t(cosf((float)x_h));
    EXPECT_EQ(expect, result);
    
    result = hsin(x_h);
    expect = bfloat16_t(sinf((float)x_h));
    EXPECT_EQ(expect, result);

    result = htanh(x_h);
    expect = bfloat16_t(tanhf((float)x_h));
    EXPECT_EQ(expect, result);

    result = hexp(x_h);
    expect = bfloat16_t(expf((float)x_h));
    EXPECT_EQ(expect, result);

    result = hexp2(x_h);
    expect = bfloat16_t(exp2f((float)x_h));
    EXPECT_EQ(expect, result);
    
    result = hexp10(x_h);
    expect = bfloat16_t(powf(10.0, (float)x_h));
    EXPECT_EQ(expect, result);

    result = hrcp(x_h);
    expect = bfloat16_t(1.0f / (float)x_h);
    EXPECT_EQ(expect, result);
}