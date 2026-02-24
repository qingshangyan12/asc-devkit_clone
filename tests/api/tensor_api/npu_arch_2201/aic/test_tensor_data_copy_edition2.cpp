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
#include "tensor_api/stub/cce_stub.h"
#include "include/experimental/tensor_api/tensor.h"

using namespace AscendC::Te;
using namespace AscendC::Std;

enum class MemoryLayout {
    RowMajor,
    NZ,
    ColumnMajor,
    ZN,
};

enum class Location {
    GM,
    L1,
    Bias,
    Fixbuf,
};

enum class Prefix {
    gm,
    cbuf,
    biasbuf,
    fbuf,
};

struct Test_Params(
    
)


class TEST_TENSOR_API_DATACOPY : public testing::Test {
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
