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

#define TEST_CUBE_DATAMOVE_COPY_L12L0A_3D(class_name, c_api_name, cce_name, data_type, cce_type)  \
                                                                                                 \
class TestCubeDmamove##class_name##data_type : public testing::Test {                           \
protected:                                                                                      \
    void SetUp() { g_coreType = C_API_AIC_TYPE; }                                        \
    void TearDown() { g_coreType = C_API_AIV_TYPE; }                                     \
};                                                                                              \
                                                                                                 \
TEST_F(TestCubeDmamove##class_name##data_type, c_api_name##_3d_Succ)                            \
{                                                                                               \
    __ca__ data_type *dst = reinterpret_cast<__ca__ data_type *>(11);                           \
    __cbuf__ data_type *src = reinterpret_cast<__cbuf__ data_type *>(22);                       \
                                                                                                 \
    MOCKER_CPP(cce_name, void(__ca__ cce_type *, __cbuf__ cce_type *, uint16_t, uint16_t,      \
               uint16_t, uint16_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t,       \
               bool, bool, bool, bool, uint16_t))                                                \
            .stubs();                                                                           \
                                                                                                 \
    c_api_name(dst, src, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, false, false, false, false, 1);         \
    GlobalMockObject::verify();                                                                 \
}                                                                                               \
                                                                                                 \
TEST_F(TestCubeDmamove##class_name##data_type, c_api_name##_3d_sync_Succ)                       \
{                                                                                               \
    __ca__ data_type *dst = reinterpret_cast<__ca__ data_type *>(11);                           \
    __cbuf__ data_type *src = reinterpret_cast<__cbuf__ data_type *>(22);                       \
                                                                                                 \
    MOCKER_CPP(cce_name, void(__ca__ cce_type *, __cbuf__ cce_type *, uint16_t, uint16_t,      \
               uint16_t, uint16_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t,       \
               bool, bool, bool, bool, uint16_t))                                                \
            .stubs();                                                                          \
                                                                                                 \
    c_api_name##_sync(dst, src, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, false, false, false, false, 1);  \
    GlobalMockObject::verify();                                                                 \
}

TEST_CUBE_DATAMOVE_COPY_L12L0A_3D(CopyL12L0A3DCApi, asc_copy_l12l0a, img2colv2_cbuf_to_ca_s4, int4b_t, void);
TEST_CUBE_DATAMOVE_COPY_L12L0A_3D(CopyL12L0A3DCApi, asc_copy_l12l0a, img2colv2_cbuf_to_ca, int8_t, int8_t);
TEST_CUBE_DATAMOVE_COPY_L12L0A_3D(CopyL12L0A3DCApi, asc_copy_l12l0a, img2colv2_cbuf_to_ca, uint8_t, uint8_t);
TEST_CUBE_DATAMOVE_COPY_L12L0A_3D(CopyL12L0A3DCApi, asc_copy_l12l0a, img2colv2_cbuf_to_ca, half, half);
TEST_CUBE_DATAMOVE_COPY_L12L0A_3D(CopyL12L0A3DCApi, asc_copy_l12l0a, img2colv2_cbuf_to_ca, bfloat16_t, bfloat16_t);
TEST_CUBE_DATAMOVE_COPY_L12L0A_3D(CopyL12L0A3DCApi, asc_copy_l12l0a, img2colv2_cbuf_to_ca, int32_t, int32_t);
TEST_CUBE_DATAMOVE_COPY_L12L0A_3D(CopyL12L0A3DCApi, asc_copy_l12l0a, img2colv2_cbuf_to_ca, uint32_t, uint32_t);
TEST_CUBE_DATAMOVE_COPY_L12L0A_3D(CopyL12L0A3DCApi, asc_copy_l12l0a, img2colv2_cbuf_to_ca, float, float);
