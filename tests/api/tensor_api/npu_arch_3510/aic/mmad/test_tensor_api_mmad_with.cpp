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
#include "include/tensor_api/tensor.h"

class Tensor_Api_Mmad_3510 : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}

    void SetUp() override 
    {
        AscendC::SetGCoreType(1);
    }

    void TearDown() override 
    {
        AscendC::SetGCoreType(0);
    }
};

TEST_F(Tensor_Api_Mmad_With_3510, TestMmadWithAtom)
{
    using namespace AscendC::Te;

    MmadParams params{16, 32, 64, 1, true};
    auto atom = MakeMmad(MmadOperation{}).with(params);

    EXPECT_EQ(atom.params.m, 16);
    EXPECT_EQ(atom.params.n, 32);
    EXPECT_EQ(atom.params.k, 64);
    EXPECT_EQ(atom.params.unitFlag, 1);
    EXPECT_EQ(atom.params.cmatrixInitVal, true);
}
