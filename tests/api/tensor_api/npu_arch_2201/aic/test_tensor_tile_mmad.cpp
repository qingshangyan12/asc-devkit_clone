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
using namespace AscendC;

TEST_F(Tensor_Api_Mmad, MmadOperation)
{
    auto a2Addr = reinterpret_cast<__ca__ float*>(0);
    auto l0aIterator = MakeL0AmemPtr(a2Addr);
    auto l0aMatrixLayout = MakeZZLayout<float>(32, 32);
    auto l0aTensor = MakeTensor(l0aIterator, l0aMatrixLayout);

    auto b2Addr = reinterpret_cast<__cb__ float*>(0);
    auto l0bIterator = MakeL0BmemPtr(b2Addr);
    auto l0bMatrixLayout = MakeZNLayout<float>(32, 32);
    auto l0bTensor = MakeTensor(l0bIterator, l0bMatrixLayout);

    auto c2Addr = reinterpret_cast<__cc__ float*>(0);
    auto l0cIterator = MakeL0CmemPtr(c2Addr);
    auto l0cMatrixLayout = MakeNZLayout<Std::ignore_t>(32, 32);
    auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout);

    MOCKER_CPP(cce_name, void(__ubuf__ data_type *,__ubuf__ data_type *,
                __ubuf__ data_type *, uint8_t, uint8_t, uint8_t, uint8_t,
                uint8_t, uint8_t, uint8_t))
            .times(1)
            .will(invoke(cce_name##_##data_type##_##data_type##_##data_type##_uint64_t_Stub));
    Mmad<DEFAULT_MMAD_TRAIT>(l0cTensor, l0aTensor, l0bTensor);
    GlobalMockObject::verify();                        
}
