/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file test_operator_popstackbuffer_pointer_args.cpp
 * \brief The address PopStackBuffer's pointer form publishes, against the Tensor form's.
 *
 * Both forms read the same two TPipe figures -- the end of the requested position and the end of its
 * queue area -- and publish the start of the region between them.  The Tensor form publishes it as a
 * TBuffAddr, the pointer form as the pointer itself; this file asserts the two values agree, for a
 * TPipe whose queue area has actually been grown by InitBuffer, so the figure is not zero.
 *
 * What the pointer form does NOT publish, by construction, is the region's length and position: a
 * bare pointer has neither field.  That difference is the documented boundary of this overload and
 * is not asserted here.
 */

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include "kernel_operator.h"
#include "stub_fun.h"

namespace AscendC {
namespace {
constexpr uint32_t bufferSize = 1024;
} // namespace

class TEST_POPSTACKBUFFER_POINTER_ARGS : public testing::Test {
protected:
    void SetUp() { SetGCoreType(1); }
    void TearDown()
    {
        GlobalMockObject::verify();
        SetGCoreType(0);
    }
};

/* Both arms publish the same start address of the free region. */
TEST_F(TEST_POPSTACKBUFFER_POINTER_ARGS, PublishesTensorAddress)
{
    TPipe tpipe;
    TBuf<TPosition::VECCALC> buf;
    tpipe.InitBuffer(buf, bufferSize);

    LocalTensor<float> tensor = buf.Get<float>();
    /* The template arguments carry commas, which the ASSERT macros would split; bind first. */
    const bool tensorPopped = PopStackBuffer<float, TPosition::VECCALC>(tensor);
    ASSERT_TRUE(tensorPopped);
    const uint64_t tensorAddr = reinterpret_cast<uint64_t>(tensor.GetPhyAddr());
    ASSERT_NE(tensorAddr, 0U);

    __ubuf__ float* pointer = nullptr;
    const bool pointerPopped = PopStackBuffer<float, TPosition::VECCALC>(pointer);
    ASSERT_TRUE(pointerPopped);
    ASSERT_NE(pointer, nullptr);

    EXPECT_EQ(reinterpret_cast<uint64_t>(pointer), tensorAddr);
    /* The region starts at a block boundary; both calls must keep that property. */
    EXPECT_EQ(reinterpret_cast<uint64_t>(pointer) % ONE_BLK_SIZE, 0U);
}

} // namespace AscendC
