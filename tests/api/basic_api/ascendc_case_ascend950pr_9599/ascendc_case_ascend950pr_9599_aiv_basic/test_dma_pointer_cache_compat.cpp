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
#include "kernel_operator.h"

using namespace AscendC;

namespace {
uint64_t* expectedSource = nullptr;
int64_t expectedOffset = 0;

template <typename Offset>
void CheckPreloadArguments(uint64_t* source, const Offset offset)
{
    EXPECT_EQ(source, expectedSource);
    EXPECT_EQ(static_cast<int64_t>(offset), expectedOffset);
}
} // namespace

class DmaPointerCacheCompatibility : public testing::Test {
protected:
    void SetUp() override { SetGCoreType(2); }
    void TearDown() override
    {
        GlobalMockObject::verify();
        SetGCoreType(0);
    }
};

// LP-C15: the public template parameter is the offset type, not the element type.
TEST_F(DmaPointerCacheCompatibility, PublicPreloadInt16Offset)
{
    uint64_t storage[32] = {};
    GlobalTensor<uint64_t> tensor;
    tensor.SetGlobalBuffer(storage, 32);
    const int16_t offset = -64;
    expectedSource = storage;
    expectedOffset = offset;
    MOCKER(DataCachePreloadImpl<int16_t>).expects(once()).will(invoke(CheckPreloadArguments<int16_t>));
    DataCachePreload<int16_t>(tensor, offset);
}

TEST_F(DmaPointerCacheCompatibility, PublicPreloadInt64OffsetAndTensorIndex)
{
    uint64_t storage[32] = {};
    GlobalTensor<uint64_t> tensor;
    tensor.SetGlobalBuffer(storage, 32);
    const int64_t offset = 65536;
    expectedSource = storage + 8;
    expectedOffset = offset;
    MOCKER(DataCachePreloadImpl<int64_t>).expects(once()).will(invoke(CheckPreloadArguments<int64_t>));
    DataCachePreload<int64_t>(tensor[8], offset);
}

TEST_F(DmaPointerCacheCompatibility, PublicPreloadDeducedOffsetConstHandle)
{
    uint64_t storage[32] = {};
    GlobalTensor<uint64_t> tensor;
    tensor.SetGlobalBuffer(storage, 32);
    const GlobalTensor<uint64_t>& handle = tensor;
    const int64_t offset = 0;
    expectedSource = storage;
    expectedOffset = offset;
    MOCKER(DataCachePreloadImpl<int64_t>).expects(once()).will(invoke(CheckPreloadArguments<int64_t>));
    DataCachePreload(handle, offset);
}
