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
#include "simt_api/asc_simt.h"

using namespace std;
using namespace AscendC;
using namespace AscendC::Simt;

struct ThreadBarriercParams {
    int32_t mode;
};

class ThreadBarriercTestsuite : public testing::Test, public testing::WithParamInterface<ThreadBarriercParams> {
protected:
    void SetUp() {}
    void TearDown() {}
};

INSTANTIATE_TEST_CASE_P(ThreadBarriercTestCase, ThreadBarriercTestsuite,
    ::testing::Values(ThreadBarriercParams {0}
                      ));

TEST_P(ThreadBarriercTestsuite, ThreadBarriercTestCase)
{
    auto param = GetParam();
    int fpByteSize = 4;
    int shapeSize = 256;
}
