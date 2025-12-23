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
#include <mockcpp/mockcpp.hpp>
#include "c_api/stub/cce_stub.h"
#include "c_api/asc_simd.h"

class TestSimdAtomicSetAtomicf16 : public testing::Test { 
protected:
    void SetUp() {}
    void TearDown() {}
};

namespace {
void set_atomic_f16_Stub() {
}

void set_atomic_cfg_Stub(atomic_type_t type, atomic_op_t op) {}
}

TEST_F(TestSimdAtomicSetAtomicf16, set_atomic_f16_Succ)
{
    MOCKER(set_atomic_f16, void(void))
        .times(1)
        .will(invoke(set_atomic_f16_Stub));
    MOCKER(set_st_atomic_cfg, void(atomic_type_t, atomic_op_t))
        .times(1)
        .will(invoke(set_atomic_cfg_Stub));
    
    asc_set_atomic_f16();
    asc_set_store_atomic_config_none_sum();
    asc_set_store_atomic_config_f32_sum();
    asc_set_store_atomic_config_f16_sum();
    asc_set_store_atomic_config_s16_sum();
    asc_set_store_atomic_config_s32_sum();
    asc_set_store_atomic_config_s8_sum();
    asc_set_store_atomic_config_bf16_sum();
    GlobalMockObject::verify();
}