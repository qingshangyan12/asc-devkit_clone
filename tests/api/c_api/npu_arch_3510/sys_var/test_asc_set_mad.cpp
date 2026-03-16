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
#include "include/c_api/asc_simd.h"

#define TEST_SYS_VAR_SET_PADDING(c_api_name, cce_name)\
\
class TestSysVarSetMad##cce_name : public testing::Test {\
protected:\
    void SetUp() {}\
    void TearDown() {}\
};\
\
void cce_name##_Stub() {}\
\
TEST_F(TestSysVarSetMad##cce_name, c_api_name##_Succ)\
{\
    MOCKER_CPP(cce_name, void(void))\
            .times(1)\
            .will(invoke(cce_name##_Stub));\
    c_api_name();\
    asc_init();\
    GlobalMockObject::verify();\
}

TEST_SYS_VAR_SET_PADDING(asc_set_mad_hif8, set_mad_hif8);
TEST_SYS_VAR_SET_PADDING(asc_set_mad_tf32, set_mad_tf32);
TEST_SYS_VAR_SET_PADDING(asc_set_mad_tf32_round_mode, set_mad_tf32_round_mode);