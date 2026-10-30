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
#include "tensor_api/experimental/vector_ops.h"

class TensorApiVectorRegDataCopy3510 : public testing::Test {};

template <typename T>
__aicore__ inline void TestMove()
{
    asc::te::experimental::reg_tensor<T> dst{};
    asc::te::experimental::reg_tensor<T> src{};
    auto allMask = asc::te::experimental::all_mask<T>();
    auto noneMask = asc::te::experimental::none_mask<T>();
    dst.mask = noneMask.reg;
    src.mask = allMask.reg;

    asc::te::experimental::move(dst, src);
    EXPECT_EQ(dst.mask, allMask.reg);

    dst.mask = allMask.reg;
    src.mask = noneMask.reg;
    asc::te::experimental::move(dst, src);
    EXPECT_EQ(dst.mask, noneMask.reg);
}

template <int16_t Offset, typename T>
__aicore__ inline void TestMoveMask()
{
    asc::te::experimental::reg_tensor<T> src{};
    auto dst = asc::te::experimental::move_mask<Offset>(src);
    static_assert(AscendC::Std::is_same_v<decltype(dst), asc::te::experimental::reg_tensor<bool>>);
    EXPECT_EQ(dst.mask, asc::te::experimental::all_mask<uint8_t>().reg);
}

#define REG_DATA_MOVE_TEST(DataType)                        \
    TEST_F(TensorApiVectorRegDataCopy3510, Move_##DataType) \
    {                                                       \
        TestMove<DataType>();                               \
        SUCCEED();                                          \
    }

REG_DATA_MOVE_TEST(bool)
REG_DATA_MOVE_TEST(uint8_t)
REG_DATA_MOVE_TEST(int8_t)
REG_DATA_MOVE_TEST(uint16_t)
REG_DATA_MOVE_TEST(int16_t)
REG_DATA_MOVE_TEST(half)
REG_DATA_MOVE_TEST(bfloat16_t)
REG_DATA_MOVE_TEST(uint32_t)
REG_DATA_MOVE_TEST(int32_t)
REG_DATA_MOVE_TEST(float)

TEST_F(TensorApiVectorRegDataCopy3510, MoveMaskUint16)
{
    TestMoveMask<0, uint16_t>();
    TestMoveMask<15, uint16_t>();
}

TEST_F(TensorApiVectorRegDataCopy3510, MoveMaskUint32)
{
    TestMoveMask<0, uint32_t>();
    TestMoveMask<31, uint32_t>();
}
