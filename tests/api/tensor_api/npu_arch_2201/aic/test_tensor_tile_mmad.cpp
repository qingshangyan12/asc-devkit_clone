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
#include "impl/experimental/tensor_api/tensor_api_impl.h"

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

#define MMAD_TEST(DST_TYPE, SRC_TYPE, M, N, K, UNIT_FALG, K_DIRECTION_ALIGN, CMATRIX_SOURCE, C_MATRIX_INIT_VAL)\
void MmadTest(__cc__ T* dst, __ca__ U* fm, __cb__ S* filter, uint16_t m, uint16_t k, uint16_t n,\
        int8_t unitFlag, bool cmatrixSource, bool cmatrixInitVal)\
{\
    EXPECT_EQ(dst, reinterpret_cast<__cc__ DST_TYPE*>(0));\
    EXPECT_EQ(fm, reinterpret_cast<__ca__ SRC_TYPE*>(0));\
    EXPECT_EQ(filter, reinterpret_cast<__cb__ SRC_TYPE*>(0));\
    EXPECT_EQ(m, M);\
    EXPECT_EQ(n, N);\
    EXPECT_EQ(k, K);\
    EXPECT_EQ(unitFlag, UNIT_FALG);\
    EXPECT_EQ(kDirectionAlign, K_DIRECTION_ALIGN);\
    EXPECT_EQ(cmatrixSource, CMATRIX_SOURCE);\
    EXPECT_EQ(cmatrixInitVal, C_MATRIX_INIT_VAL);\
}\
\
TEST_F(Tensor_Api_Mmad, MmadOperation)\
{\
    auto a2Addr = reinterpret_cast<__ca__ float*>(0);\
    auto l0aIterator = MakeL0AmemPtr(a2Addr);\
    auto l0aMatrixLayout = MakeZZLayout<float>(M, K);\
    auto l0aTensor = MakeTensor(l0aIterator, l0aMatrixLayout);\
\
    auto b2Addr = reinterpret_cast<__cb__ float*>(0);\
    auto l0bIterator = MakeL0BmemPtr(b2Addr);\
    auto l0bMatrixLayout = MakeZNLayout<float>(K, N);\
    auto l0bTensor = MakeTensor(l0bIterator, l0bMatrixLayout);\
\
    auto c2Addr = reinterpret_cast<__cc__ float*>(0);\
    auto l0cIterator = MakeL0CmemPtr(c2Addr);\
    auto l0cMatrixLayout = MakeNZLayout<AscendC::Std::ignore_t>(M, N);\
    auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout);\
\
    MOCKER_CPP(cce_name, void(__cc__ DST_TYPE *,__ca__ SRC_TYPE *,\
                __cb__ SRC_TYPE *, uint16_t, uint16_t, uint16_t, uint8_t,\
                bool, bool, bool))\
            .times(1)\
            .will(invoke(MmadTest));\
    MmadTriat triat = {0, K_DIRECTION_ALIGN, CMATRIX_SOURCE}\
    Mmad<triat>(l0cTensor, l0aTensor, l0bTensor, UNIT_FALG, C_MATRIX_INIT_VAL);\
    GlobalMockObject::verify();\
}

MMAD_TEST(float, half, 16, 16, 16, 0, 0, 0, 0);