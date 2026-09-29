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
#include "c_api/asc_simd.h"

TEST(TestShiftLeftScalarLiteralCApi, asc_shiftleft_scalar_vector_uint16_t_literal_Succ)
{
    vector_uint16_t dst;
    vector_uint16_t src;
    vector_bool mask;

    MOCKER_CPP(vshls, void(vector_uint16_t&, vector_uint16_t, int16_t, vector_bool, Literal)).times(2);

    asc_shiftleft_scalar(dst, src, 1, mask);
    vector_uint16_t result = asc_shiftleft_scalar(src, 1, mask);
    (void)result;
    GlobalMockObject::verify();
}

TEST(TestShiftRightScalarLiteralCApi, asc_shiftright_scalar_vector_uint16_t_literal_Succ)
{
    vector_uint16_t dst;
    vector_uint16_t src;
    vector_bool mask;

    MOCKER_CPP(vshrs, void(vector_uint16_t&, vector_uint16_t, int16_t, vector_bool, Literal)).times(2);

    asc_shiftright_scalar(dst, src, 1, mask);
    vector_uint16_t result = asc_shiftright_scalar(src, 1, mask);
    (void)result;
    GlobalMockObject::verify();
}
