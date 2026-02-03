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

class Tensor_Api_Datacopy : public testing::Test {
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

TEST_F(Tensor_Api_Datacopy, DataCopyGmToL1NdToNzOperation)
{
    uint8_t aGM[256] = {0};
    auto gmA = reinterpret_cast<__gm__ float*>(aGM);
    auto gmIterator = MakeGMmemPtr(gmA);
    auto gmMatrixLayout = MakeRowMajorLayout<float>(32, 32);
    auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout); 

    auto a1Addr = reinterpret_cast<__cbuf__ float*>(0);
    auto aIterator = MakeL1memPtr(a1Addr);
    auto aMatrixLayout = MakeNZLayout<float>(32, 32);
    auto aTensor = MakeTensor(aIterator, aMatrixLayout);

    DataCopy<DEFAULT_DATA_COPY_TRAIT>(aTensor, gmTensor);

    auto coord = MakeCoord(MakeCoord(Std::Int<0>{}, Std::Int<0>{}), MakeCoord(Std::Int<0>{}, Std::Int<0>{}) );
    DataCopy<DEFAULT_DATA_COPY_TRAIT>(aTensor, gmTensor, coord);
}

TEST_F(Tensor_Api_Datacopy, DataCopyGmToL1DnToZnOperation)
{
    uint8_t aGM[256] = {0};
    auto gmA = reinterpret_cast<__gm__ float*>(aGM);
    auto gmIterator = MakeGMmemPtr(gmA);
    auto gmMatrixLayout = MakeColumnMajorLayout<float>(32, 32);
    auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout); 

    auto a1Addr = reinterpret_cast<__cbuf__ float*>(0);
    auto aIterator = MakeL1memPtr(a1Addr);
    auto aMatrixLayout = MakeZNLayout<float>(32, 32);
    auto aTensor = MakeTensor(aIterator, aMatrixLayout);

    DataCopy<DEFAULT_DATA_COPY_TRAIT>(aTensor, gmTensor);

    auto coord = MakeCoord(MakeCoord(Std::Int<0>{}, Std::Int<0>{}), MakeCoord(Std::Int<0>{}, Std::Int<0>{}) );
    DataCopy<DEFAULT_DATA_COPY_TRAIT>(aTensor, gmTensor, coord);
}

TEST_F(Tensor_Api_Datacopy, DataCopyGmToL1NdToNdOperation)
{
    uint8_t aGM[256] = {0};
    auto gmA = reinterpret_cast<__gm__ float*>(aGM);
    auto gmIterator = MakeGMmemPtr(gmA);
    auto gmMatrixLayout = MakeRowMajorLayout<float>(32, 32);
    auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout); 

    auto a1Addr = reinterpret_cast<__cbuf__ float*>(0);
    auto aIterator = MakeL1memPtr(a1Addr);
    auto aMatrixLayout = MakeRowMajorLayout<float>(32, 32);
    auto aTensor = MakeTensor(aIterator, aMatrixLayout);

    DataCopy<DEFAULT_DATA_COPY_TRAIT>(aTensor, gmTensor);

    auto coord = MakeCoord(MakeCoord(Std::Int<0>{}, Std::Int<0>{}), MakeCoord(Std::Int<0>{}, Std::Int<0>{}) );
    DataCopy<DEFAULT_DATA_COPY_TRAIT>(aTensor, gmTensor, coord);
}

TEST_F(Tensor_Api_Datacopy, DataCopyGmToL1NzToNzOperation)
{
    uint8_t aGM[256] = {0};
    auto gmA = reinterpret_cast<__gm__ float*>(aGM);
    auto gmIterator = MakeGMmemPtr(gmA);
    auto gmMatrixLayout = MakeNZLayout<float>(32, 32);
    auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout); 

    auto a1Addr = reinterpret_cast<__cbuf__ float*>(0);
    auto aIterator = MakeL1memPtr(a1Addr);
    auto aMatrixLayout = MakeNZLayout<float>(32, 32);
    auto aTensor = MakeTensor(aIterator, aMatrixLayout);

    DataCopy<DEFAULT_DATA_COPY_TRAIT>(aTensor, gmTensor);

    auto coord = MakeCoord(MakeCoord(Std::Int<0>{}, Std::Int<0>{}), MakeCoord(Std::Int<0>{}, Std::Int<0>{}) );
    DataCopy<DEFAULT_DATA_COPY_TRAIT>(aTensor, gmTensor, coord);
}

TEST_F(Tensor_Api_Datacopy, DataCopyL1ToFbOperation)
{
    auto a1Addr = reinterpret_cast<__cbuf__ float*>(0);
    auto aIterator = MakeL1memPtr(a1Addr);
    auto aMatrixLayout = MakeRowMajorLayout<float>(1, 32);
    auto aTensor = MakeTensor(aIterator, aMatrixLayout);

    auto fbAddr = reinterpret_cast<__fbuf__ float*>(0);
    auto fbIterator = MakeFixbufmemPtr(fbAddr);
    auto fbMatrixLayout = MakeRowMajorLayout<float>(1, 32);
    auto fbTensor = MakeTensor(fbIterator, fbMatrixLayout);

    DataCopy<DEFAULT_DATA_COPY_TRAIT>(fbTensor, aTensor);

    auto coord = MakeCoord(MakeCoord(Std::Int<0>{}, Std::Int<0>{}), MakeCoord(Std::Int<0>{}, Std::Int<0>{}) );
    DataCopy<DEFAULT_DATA_COPY_TRAIT>(fbTensor, aTensor, coord);
}

TEST_F(Tensor_Api_Datacopy, DataCopyL1ToBtOperation)
{
    auto a1Addr = reinterpret_cast<__cbuf__ float*>(0);
    auto aIterator = MakeL1memPtr(a1Addr);
    auto aMatrixLayout = MakeRowMajorLayout<float>(1, 32);
    auto aTensor = MakeTensor(aIterator, aMatrixLayout);

    auto btAddr = reinterpret_cast<__biasbuf__ float*>(0);
    auto btIterator = MakeBiasmemPtr(btAddr);
    auto btMatrixLayout = MakeRowMajorLayout<float>(1, 32);
    auto btTensor = MakeTensor(btIterator, btMatrixLayout);

    DataCopy<DEFAULT_DATA_COPY_TRAIT>(btTensor, aTensor);

    auto coord = MakeCoord(MakeCoord(Std::Int<0>{}, Std::Int<0>{}), MakeCoord(Std::Int<0>{}, Std::Int<0>{}) );
    DataCopy<DEFAULT_DATA_COPY_TRAIT>(btTensor, aTensor, coord);
}

TEST_F(Tensor_Api_Datacopy, DataCopyEmptyRoutingOperation)
{
    auto a1Addr = reinterpret_cast<__cbuf__ float*>(0);
    auto aIterator = MakeL1memPtr(a1Addr);
    auto aMatrixLayout = MakeNZLayout<float>(32, 32);
    auto aTensor = MakeTensor(aIterator, aMatrixLayout);

    auto tmpAddr = reinterpret_cast<__cbuf__ float*>(0);
    auto tmpIterator = MakeL1memPtr(tmpAddr);
    auto tmpMatrixLayout = MakeNZLayout<float>(32, 32);
    auto tmpTensor = MakeTensor(tmpIterator, tmpMatrixLayout);

    DataCopy<DEFAULT_DATA_COPY_TRAIT>(tmpTensor, aTensor);

    auto coord = MakeCoord(MakeCoord(Std::Int<0>{}, Std::Int<0>{}), MakeCoord(Std::Int<0>{}, Std::Int<0>{}) );
    DataCopy<DEFAULT_DATA_COPY_TRAIT>(tmpTensor, aTensor, coord);
}