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
#include "tensor_api/stub/cce_stub.h"
#include <mockcpp/mockcpp.hpp>
#include "include/experimental/tensor_api/tensor.h"

class Tensor_Api_Mmad : public testing::Test {
protected:
    void SetUp()
    {
        AscendC::SetGCoreType(1);
    }
    void TearDown()
    {
        AscendC::SetGCoreType(0);
    }
};
using namespace AscendC::Te;

#define MMAD_TEST(DST_TYPE, SRC_TYPE, M, N, K, UNIT_FLAG, K_DIRECTION_ALIGN, CMATRIX_SOURCE, C_MATRIX_INIT_VAL)\
uint8_t A2##DST_TYPE##_##SRC_TYPE[256 * 256 * sizeof(SRC_TYPE)] = {0};\
uint8_t B2##DST_TYPE##_##SRC_TYPE[256 * 256 * sizeof(SRC_TYPE)] = {0};\
uint8_t C2##DST_TYPE##_##SRC_TYPE[256 * 256 * sizeof(DST_TYPE)] = {0};\
void MmadTest##DST_TYPE##_##SRC_TYPE(__cc__ DST_TYPE* dst, __ca__ SRC_TYPE* fm, __cb__ SRC_TYPE* filter, uint16_t m, uint16_t k, uint16_t n,\
        uint8_t unitFlag, bool kDirectionAlign, bool cmatrixSource, bool cmatrixInitVal)\
{\
    EXPECT_EQ(dst, reinterpret_cast<__cc__ DST_TYPE*>(C2##DST_TYPE##_##SRC_TYPE));\
    EXPECT_EQ(fm, reinterpret_cast<__ca__ SRC_TYPE*>(A2##DST_TYPE##_##SRC_TYPE));\
    EXPECT_EQ(filter, reinterpret_cast<__cb__ SRC_TYPE*>(B2##DST_TYPE##_##SRC_TYPE));\
    EXPECT_EQ(m, M);\
    EXPECT_EQ(n, N);\
    EXPECT_EQ(k, K);\
    EXPECT_EQ(unitFlag, UNIT_FLAG);\
    EXPECT_EQ(kDirectionAlign, K_DIRECTION_ALIGN);\
    EXPECT_EQ(cmatrixSource, CMATRIX_SOURCE);\
    EXPECT_EQ(cmatrixInitVal, C_MATRIX_INIT_VAL);\
}\
\
TEST_F(Tensor_Api_Mmad, MmadOperation##DST_TYPE##_##SRC_TYPE##_##M##_##N##_##K)\
{\
    auto a2Addr = reinterpret_cast<__ca__ SRC_TYPE*>(A2##DST_TYPE##_##SRC_TYPE);\
    auto l0aIterator = MakeL0AmemPtr(a2Addr);\
    auto l0aMatrixLayout = MakeZZLayout<SRC_TYPE>(M, K);\
    auto l0aTensor = MakeTensor(l0aIterator, l0aMatrixLayout);\
\
    auto b2Addr = reinterpret_cast<__cb__ SRC_TYPE*>(B2##DST_TYPE##_##SRC_TYPE);\
    auto l0bIterator = MakeL0BmemPtr(b2Addr);\
    auto l0bMatrixLayout = MakeZNLayout<SRC_TYPE>(K, N);\
    auto l0bTensor = MakeTensor(l0bIterator, l0bMatrixLayout);\
\
    auto c2Addr = reinterpret_cast<__cc__ DST_TYPE*>(C2##DST_TYPE##_##SRC_TYPE);\
    auto l0cIterator = MakeL0CmemPtr(c2Addr);\
    auto l0cMatrixLayout = MakeNZLayout<AscendC::Std::ignore_t>(M, N);\
    auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout);\
\
    MOCKER_CPP(mad, void(__cc__ DST_TYPE *,\
                __cb__ SRC_TYPE *, uint16_t, uint16_t, uint16_t, uint8_t,\
                bool, bool, bool))\
            .times(1)\
            .will(invoke(MmadTest##DST_TYPE##_##SRC_TYPE));\
    static constexpr MmadTrait triat = {0, 0, 0, 0, UNIT_FLAG, K_DIRECTION_ALIGN, CMATRIX_SOURCE, C_MATRIX_INIT_VAL};\
    Mmad<triat>(l0cTensor, l0aTensor, l0bTensor);\
    GlobalMockObject::verify();\
}

MMAD_TEST(float, half, 16, 16, 16, 0, 0, 0, 0);
MMAD_TEST(float, float, 16, 16, 16, 0, 0, 0, 0);
MMAD_TEST(float, bfloat16_t, 16, 16, 16, 0, 0, 0, 0);
MMAD_TEST(int32_t, int8_t, 32, 32, 32, 0, 0, 0, 0);