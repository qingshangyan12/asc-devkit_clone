/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <algorithm>

#include <gtest/gtest.h>

#include "tensor_api/stub/cce_stub.h"
#include "include/tensor_api/tensor.h"

namespace {

using tensor_api_print_test::output;
using tensor_api_print_test::records;
using tensor_api_print_test::reset_records;

class TensorApiPrintTest : public testing::Test {
protected:
    void SetUp() override
    {
        reset_records();
    }
};

void expect_single_head()
{
    ASSERT_FALSE(records().empty());
    EXPECT_EQ(records().front().head, "[AIC Block 0/1] ");
    EXPECT_TRUE(std::all_of(records().begin() + 1, records().end(), [](const auto& record) {
        return record.head.empty();
    }));
}

TEST_F(TensorApiPrintTest, FormatOverloadsForwardOnePrintRecord)
{
    asc::te::print((__gm__ const char*)"plain");
    EXPECT_EQ(output(), "[AIC Block 0/1] plain");
    ASSERT_EQ(records().size(), 1U);

    reset_records();
    asc::te::print((__gm__ const char*)"value=%d, text=%s\n", 7, (__gm__ const char*)"ok");
    EXPECT_EQ(output(), "[AIC Block 0/1] value=7, text=ok\n");
    ASSERT_EQ(records().size(), 1U);
}

TEST_F(TensorApiPrintTest, ScalarValuesUseExpectedFormatsAndEndWithNewline)
{
    asc::te::print(-7);
    EXPECT_EQ(output(), "[AIC Block 0/1] -7\n");
    expect_single_head();

    reset_records();
    asc::te::print(uint64_t{42});
    EXPECT_EQ(output(), "[AIC Block 0/1] 42\n");
    expect_single_head();

    reset_records();
    asc::te::print(1.5F);
    EXPECT_EQ(output(), "[AIC Block 0/1] 1.500000\n");
    expect_single_head();
}

TEST_F(TensorApiPrintTest, NestedTuplePreservesStructureAndUsesOneHead)
{
    using namespace asc::te;

    print(make_coord(2, make_coord(3, 4)));

    EXPECT_EQ(output(), "[AIC Block 0/1] (2, (3, 4))\n");
    expect_single_head();
}

TEST_F(TensorApiPrintTest, NonRankTwoLayoutUsesCompactRepresentation)
{
    using namespace asc::te;

    print(make_layout(make_shape(2, 3, 4)));

    EXPECT_EQ(output(), "[AIC Block 0/1] layout(shape=(2, 3, 4), stride=(12, 4, 1))\n");
    expect_single_head();
}

TEST_F(TensorApiPrintTest, RankTwoLayoutPrintsIndexTable)
{
    using namespace asc::te;

    print(make_layout(make_shape(2, 3)));

    EXPECT_EQ(output(),
        "[AIC Block 0/1] layout(shape=(2, 3), stride=(3, 1))\n"
        "      0   1   2 \n"
        "    +---+---+---+\n"
        " 0  | 0 | 1 | 2 |\n"
        "    +---+---+---+\n"
        " 1  | 3 | 4 | 5 |\n"
        "    +---+---+---+\n");
    expect_single_head();
}

TEST_F(TensorApiPrintTest, RankOneAndTwoTensorsPrintAllElements)
{
    using namespace asc::te;

    __gm__ int32_t rank_one_data[2] = {1, 2};
    auto rank_one = make_tensor(make_mem_ptr(rank_one_data), make_layout(make_shape(2)));
    print(rank_one);
    EXPECT_EQ(output(),
        "[AIC Block 0/1] tensor<layout(shape=(2), stride=(1))>:\n"
        "1  \n"
        "2  \n");
    expect_single_head();

    reset_records();
    __gm__ int32_t rank_two_data[4] = {1, 2, 3, 4};
    auto rank_two = make_tensor(make_mem_ptr(rank_two_data), make_layout(make_shape(2, 2)));
    print(rank_two);
    EXPECT_EQ(output(),
        "[AIC Block 0/1] tensor<layout(shape=(2, 2), stride=(2, 1))>:\n"
        "1  2  \n"
        "3  4  \n");
    expect_single_head();
}

TEST_F(TensorApiPrintTest, RankThreeAndFourTensorsPrintSliceSeparators)
{
    using namespace asc::te;

    __gm__ int32_t rank_three_data[4] = {1, 2, 3, 4};
    auto rank_three = make_tensor(make_mem_ptr(rank_three_data), make_layout(make_shape(1, 2, 2)));
    print(rank_three);
    EXPECT_EQ(output(),
        "[AIC Block 0/1] tensor<layout(shape=(1, 2, 2), stride=(4, 2, 1))>:\n"
        "1  3  \n"
        "----------\n"
        "2  4  \n");
    expect_single_head();

    reset_records();
    __gm__ int32_t rank_four_data[4] = {1, 2, 3, 4};
    auto rank_four = make_tensor(make_mem_ptr(rank_four_data), make_layout(make_shape(1, 1, 2, 2)));
    print(rank_four);
    EXPECT_EQ(output(),
        "[AIC Block 0/1] tensor<layout(shape=(1, 1, 2, 2), stride=(4, 4, 2, 1))>:\n"
        "1  \n"
        "-----\n"
        "3  \n"
        "=====\n"
        "2  \n"
        "-----\n"
        "4  \n");
    expect_single_head();
}

} // namespace
