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

#undef __CHECK_FEATURE_AT_PRECOMPILE
#define ASCENDC_DUMP 1
#define __simt_callee__ 
#define __gm__ 
#define __aicore__

#undef ASCENDC_CPU_DEBUG
#include "simt_api/simt_api.h"

class ClockTestsuite : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};



TEST_F(ClockTestsuite, ClockTest)
{
    uint64_t t1 = __asc_simt_vf::clock();
    uint64_t t2 = __asc_simt_vf::clock();
    //t2-t1>=0
    //EXPECT_EQ(tlv1->param[1], tlv2->param[1]);
}
