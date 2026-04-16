/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "gtest/gtest.h"
#include <mockcpp/mockcpp.hpp>

#ifndef private
#define private public
#define protected public
#endif

#include "hccl/hccl_types.h"
#include "aicpu_kfc/aicpu_kfc_interface.h"

#undef private
#undef protected

class MC2AicpuInterface_UT : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(MC2AicpuInterface_UT, RunAicpuKfcResInit_nullptr) { EXPECT_EQ(HCCL_E_PARA, RunAicpuKfcResInit(nullptr)); }

TEST_F(MC2AicpuInterface_UT, RunAicpuKfcSrvLaunch_nullptr) { EXPECT_EQ(HCCL_E_PARA, RunAicpuKfcSrvLaunch(nullptr)); }
