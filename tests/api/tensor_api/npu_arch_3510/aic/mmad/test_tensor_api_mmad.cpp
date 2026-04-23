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

TEST_F(Tensor_Api_Mmad_3510, TestMakeMmadAtom)
{
    using namespace AscendC::Te;

    auto atom = MakeMmad(MmadOperation{});
    auto withAtom = atom.with(MmadParams{1, 2, 3, 4, false});

    EXPECT_EQ(withAtom.params.m, 1);
    EXPECT_EQ(withAtom.params.n, 2);
    EXPECT_EQ(withAtom.params.k, 3);
    EXPECT_EQ(withAtom.params.unitFlag, 4);
    EXPECT_EQ(withAtom.params.cmatrixInitVal, false);
}
