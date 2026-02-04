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
#include <mockcpp/mockcpp.hpp>

class Tensor_Api_LoadData : public testing::Test {
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

TEST_F(Tensor_Api_LoadData, LoadDataL1ToL0aNzToZzOperation)
{

    auto a1Addr = reinterpret_cast<__cbuf__ float*>(0);
    auto aIterator = MakeL1memPtr(a1Addr);
    auto aMatrixLayout = MakeNZLayout<float>(32, 32);
    auto aTensor = MakeTensor(aIterator, aMatrixLayout);

    auto a2Addr = reinterpret_cast<__ca__ float*>(0);
    auto l0aIterator = MakeL0AmemPtr(a2Addr);
    auto l0aMatrixLayout = MakeZZLayout<float>(32, 32);
    auto l0aTensor = MakeTensor(l0aIterator, l0aMatrixLayout);

    LoadData<DEFAULT_LOAD_DATA_TRAIT>(l0aTensor, aTensor);

    auto coord = MakeCoord(MakeCoord(Std::Int<0>{}, Std::Int<0>{}), MakeCoord(Std::Int<0>{}, Std::Int<0>{}) );
    
    MOCKER_CPP(img2colv2_cbuf_to_ca, void(__ca__ float *,__cbuf__ float *,
                uint16_t, uint16_t, uint16_t, uint16_t, uint8_t, uint8_t, uint8_t, uint8_t, 
                uint8_t, uint8_t, bool, bool, bool, bool, uint16_t))
            .times(1)
            .will(invoke(cce_name##_##data_type##_##data_type##_##data_type##_uint64_t_Stub));
    LoadData<DEFAULT_LOAD_DATA_TRAIT>(l0aTensor, aTensor, coord);

    GlobalMockObject::verify();
}

TEST_F(Tensor_Api_LoadData, LoadDataL1ToL0aZnToZzOperation)
{

    auto a1Addr = reinterpret_cast<__cbuf__ float*>(0);
    auto aIterator = MakeL1memPtr(a1Addr);
    auto aMatrixLayout = MakeZNLayout<float>(32, 32);
    auto aTensor = MakeTensor(aIterator, aMatrixLayout);

    auto a2Addr = reinterpret_cast<__ca__ float*>(0);
    auto l0aIterator = MakeL0AmemPtr(a2Addr);
    auto l0aMatrixLayout = MakeZZLayout<float>(32, 32);
    auto l0aTensor = MakeTensor(l0aIterator, l0aMatrixLayout);
    static constexpr LoadDataTrait trait = {true};
    LoadData<trait>(l0aTensor, aTensor);

    auto coord = MakeCoord(MakeCoord(Std::Int<0>{}, Std::Int<0>{}), MakeCoord(Std::Int<0>{}, Std::Int<0>{}) );
    LoadData<trait>(l0aTensor, aTensor, coord);
}

TEST_F(Tensor_Api_LoadData, LoadDataL1ToL0bNzToZnOperation)
{

    auto b1Addr = reinterpret_cast<__cbuf__ float*>(0);
    auto bIterator = MakeL1memPtr(b1Addr);
    auto bMatrixLayout = MakeNZLayout<float>(32, 32);
    auto bTensor = MakeTensor(bIterator, bMatrixLayout);

    auto b2Addr = reinterpret_cast<__cb__ float*>(0);
    auto l0bIterator = MakeL0BmemPtr(b2Addr);
    auto l0bMatrixLayout = MakeZNLayout<float>(32, 32);
    auto l0bTensor = MakeTensor(l0bIterator, l0bMatrixLayout);

    LoadData<DEFAULT_LOAD_DATA_TRAIT>(l0bTensor, bTensor);

    auto coord = MakeCoord(MakeCoord(Std::Int<0>{}, Std::Int<0>{}), MakeCoord(Std::Int<0>{}, Std::Int<0>{}) );
    LoadData<DEFAULT_LOAD_DATA_TRAIT>(l0bTensor, bTensor, coord);
}

TEST_F(Tensor_Api_LoadData, LoadDataEmptyRoutingOperation)
{

    auto b1Addr = reinterpret_cast<__cbuf__ float*>(0);
    auto bIterator = MakeL1memPtr(b1Addr);
    auto bMatrixLayout = MakeNZLayout<float>(32, 32);
    auto bTensor = MakeTensor(bIterator, bMatrixLayout);

    LoadData<DEFAULT_LOAD_DATA_TRAIT>(bTensor, bTensor);

    auto coord = MakeCoord(MakeCoord(Std::Int<0>{}, Std::Int<0>{}), MakeCoord(Std::Int<0>{}, Std::Int<0>{}) );
    LoadData<DEFAULT_LOAD_DATA_TRAIT>(bTensor, bTensor, coord);
}
