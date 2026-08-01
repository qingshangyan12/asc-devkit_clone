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
#include "include/tensor_api/tensor.h"


class Tensor_Api_Layout_Struct : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    void TearDown() {}
};

TEST_F(Tensor_Api_Layout_Struct, TestMakeLayoutFromShapeAndStride)
{
    using namespace AscendC::Te;

    auto layout = make_layout(make_shape(2, 3, 4), make_stride(12, 4, 1));

    EXPECT_EQ(AscendC::Std::get<0>(layout.Shape()), 2);
    EXPECT_EQ(AscendC::Std::get<1>(layout.Shape()), 3);
    EXPECT_EQ(AscendC::Std::get<2>(layout.Shape()), 4);

    EXPECT_EQ(AscendC::Std::get<0>(layout.Stride()), 12);
    EXPECT_EQ(AscendC::Std::get<1>(layout.Stride()), 4);
    EXPECT_EQ(AscendC::Std::get<2>(layout.Stride()), 1);
}

TEST_F(Tensor_Api_Layout_Struct, TestMakeLayoutFromFlatShape)
{
    using namespace AscendC::Te;

    auto layout = make_layout(make_shape(2, 3, 4));

    EXPECT_EQ(AscendC::Std::get<0>(layout.Stride()), 12);
    EXPECT_EQ(AscendC::Std::get<1>(layout.Stride()), 4);
    EXPECT_EQ(AscendC::Std::get<2>(layout.Stride()), 1);
    EXPECT_EQ(layout.Size(), 24);
}

TEST_F(Tensor_Api_Layout_Struct, TestMakeLayoutFromNestedShape)
{
    using namespace AscendC::Te;

    auto shape = make_shape(make_shape(2, 3), make_shape(4, 5));
    auto layout = make_layout(shape);

    auto rowStride = AscendC::Std::get<0>(layout.Stride());
    auto colStride = AscendC::Std::get<1>(layout.Stride());

    EXPECT_EQ(AscendC::Std::get<0>(rowStride), 1);
    EXPECT_EQ(AscendC::Std::get<1>(rowStride), 8);
    EXPECT_EQ(AscendC::Std::get<0>(colStride), 2);
    EXPECT_EQ(AscendC::Std::get<1>(colStride), 24);
}

TEST_F(Tensor_Api_Layout_Struct, TestGetLayoutPatternFromMakeLayoutFractal)
{
    using namespace AscendC::Te;

    auto nzLayout = make_layout(
        make_shape(make_shape(_16{}, _2{}),
            make_shape(_16{}, _3{})),
        make_stride(make_stride(_16{}, _256{}),
            make_stride(_1{}, _512{})));
    auto znLayout = make_layout(
        make_shape(make_shape(_16{}, _2{}),
            make_shape(_16{}, _3{})),
        make_stride(make_stride(_1{}, _512{}),
            make_stride(_16{}, _256{})));
    auto zzLayout = make_layout(
        make_shape(make_shape(_16{}, _2{}),
            make_shape(_16{}, _3{})),
        make_stride(make_stride(_16{}, _512{}),
            make_stride(_1{}, _256{})));
    auto nnLayout = make_layout(
        make_shape(make_shape(_2{}, _4{}),
            make_shape(_16{}, _3{})),
        make_stride(make_stride(_1{}, _32{}),
            make_stride(_2{}, _128{})));
    auto ndExtLayout = make_layout(
        make_shape(make_shape(_1{}, _4{}),
            make_shape(_1{}, _5{})),
        make_stride(make_stride(_0{}, _5{}),
            make_stride(_0{}, _1{})));
    auto dnExtLayout = make_layout(
        make_shape(make_shape(_1{}, _4{}),
            make_shape(_1{}, _5{})),
        make_stride(make_stride(_0{}, _1{}),
            make_stride(_0{}, _4{})));
    auto ndLayout = make_layout(make_shape(_4{}, _5{}),
        make_stride(_5{}, _1{}));
    auto dnLayout = make_layout(make_shape(_4{}, _5{}),
        make_stride(_1{}, _4{}));
    auto batchedNzLayout = make_layout(
        make_shape(_2{}, make_shape(make_shape(_16{}, _2{}),
            make_shape(_16{}, _3{}))),
        make_stride(AscendC::Std::Int<1536>{}, make_stride(make_stride(_16{}, _256{}),
            make_stride(_1{}, _512{}))));
    auto batchedZnLayout = make_layout(
        make_shape(_2{}, make_shape(make_shape(_16{}, _2{}),
            make_shape(_16{}, _3{}))),
        make_stride(AscendC::Std::Int<1536>{}, make_stride(make_stride(_1{}, _512{}),
            make_stride(_16{}, _256{}))));
    auto batchedZzLayout = make_layout(
        make_shape(_2{}, make_shape(make_shape(_16{}, _2{}),
            make_shape(_16{}, _3{}))),
        make_stride(AscendC::Std::Int<1536>{}, make_stride(make_stride(_16{}, _512{}),
            make_stride(_1{}, _256{}))));
    auto batchedNnLayout = make_layout(
        make_shape(_2{}, make_shape(make_shape(_2{}, _4{}),
            make_shape(_16{}, _3{}))),
        make_stride(AscendC::Std::Int<384>{}, make_stride(make_stride(_1{}, _32{}),
            make_stride(_2{}, _128{}))));
    auto batchedNdExtLayout = make_layout(
        make_shape(_2{}, make_shape(make_shape(_1{}, _4{}),
            make_shape(_1{}, _5{}))),
        make_stride(AscendC::Std::Int<20>{}, make_stride(make_stride(_0{}, _5{}),
            make_stride(_0{}, _1{}))));
    auto batchedDnExtLayout = make_layout(
        make_shape(_2{}, make_shape(make_shape(_1{}, _4{}),
            make_shape(_1{}, _5{}))),
        make_stride(AscendC::Std::Int<20>{}, make_stride(make_stride(_0{}, _1{}),
            make_stride(_0{}, _4{}))));
    auto batchedNdLayout = make_layout(
        make_shape(_2{}, make_shape(_4{}, _5{})),
        make_stride(AscendC::Std::Int<20>{}, make_stride(_5{}, _1{})));
    auto batchedDnLayout = make_layout(
        make_shape(_2{}, make_shape(_4{}, _5{})),
        make_stride(AscendC::Std::Int<20>{}, make_stride(_1{}, _4{})));
    auto batchNdUnContinuousLayout = make_layout(
        make_shape(_3{}, make_shape(_4{}, _5{})),
        make_stride(_5{}, make_stride(AscendC::Std::Int<15>{}, _1{})));
    auto batchDnUnContinuousLayout = make_layout(
        make_shape(_3{}, make_shape(_4{}, _5{})),
        make_stride(_4{}, make_stride(_1{}, AscendC::Std::Int<12>{})));
    auto invalidFlatLayout = make_layout(make_shape(_4{}, _5{}),
        make_stride(_4{}, _5{}));
    auto invalidFractalLayout = make_layout(
        make_shape(make_shape(_4{}, _5{}),
            make_shape(_8{}, _9{})),
        make_stride(make_stride(_2{}, _3{}),
            make_stride(_4{}, _5{})));
    auto invalidScaleADNLayout = make_layout(
        make_shape(make_shape(_1{}, _4{}),
            make_shape(_1{}, _5{})),
        make_stride(make_stride(_0{}, _2{}),
            make_stride(_1{}, _8{})));
    auto invalidBatchedFractalLayout = make_layout(
        make_shape(_2{}, make_shape(make_shape(_4{}, _5{}),
            make_shape(_8{}, _9{}))),
        make_stride(AscendC::Std::Int<180>{}, make_stride(make_stride(_2{}, _3{}),
            make_stride(_4{}, _5{}))));

    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(nzLayout)>, NZLayoutPtn>);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(znLayout)>, ZNLayoutPtn>);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(zzLayout)>, ZZLayoutPtn>);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(nnLayout)>, NNLayoutPtn>);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(ndExtLayout)>, NDExtLayoutPtn>);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(dnExtLayout)>, DNExtLayoutPtn>);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(ndLayout)>, NDLayoutPtn>);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(dnLayout)>, DNLayoutPtn>);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(batchedNzLayout)>, NZLayoutPtn>);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(batchedZnLayout)>, ZNLayoutPtn>);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(batchedZzLayout)>, ZZLayoutPtn>);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(batchedNnLayout)>, NNLayoutPtn>);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(batchedNdExtLayout)>, NDExtLayoutPtn>);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(batchedDnExtLayout)>, DNExtLayoutPtn>);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(batchedNdLayout)>, NDLayoutPtn>);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(batchedDnLayout)>, DNLayoutPtn>);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(batchNdUnContinuousLayout)>, NDLayoutPtn>);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(batchDnUnContinuousLayout)>, DNLayoutPtn>);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(invalidFlatLayout)>, AscendC::Std::ignore_t>);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(invalidFractalLayout)>, AscendC::Std::ignore_t>);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(invalidScaleADNLayout)>, AscendC::Std::ignore_t>);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(invalidBatchedFractalLayout)>,
        AscendC::Std::ignore_t>);
}

TEST_F(Tensor_Api_Layout_Struct, TestRemoveBatchDimFromThreeDimLayout)
{
    using namespace AscendC::Te;

    auto layout = make_pattern_layout<NDLayoutPtn, LayoutTraitDefault<>>(
        make_shape(2, make_shape(8, 16)), make_stride(128, make_stride(16, 1)));
    auto noBatchLayout = remove_batch_dim(layout);

    static_assert(decltype(noBatchLayout)::depth == TWO_DIM_DATA);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(noBatchLayout)>, NDLayoutPtn>);
    EXPECT_EQ(AscendC::Std::get<0>(noBatchLayout.Shape()), 8);
    EXPECT_EQ(AscendC::Std::get<1>(noBatchLayout.Shape()), 16);
    EXPECT_EQ(AscendC::Std::get<0>(noBatchLayout.Stride()), 16);
    EXPECT_EQ(AscendC::Std::get<1>(noBatchLayout.Stride()), 1);
}

TEST_F(Tensor_Api_Layout_Struct, TestRemoveBatchDimFromFiveDimLayout)
{
    using namespace AscendC::Te;

    auto layout = make_pattern_layout<NZLayoutPtn, LayoutTraitDefault<>>(
        make_shape(2, make_shape(make_shape(16, 2), make_shape(16, 4))),
        make_stride(2048, make_stride(make_stride(16, 256), make_stride(1, 512))));
    auto noBatchLayout = remove_batch_dim(layout);
    auto shape = noBatchLayout.Shape();
    auto stride = noBatchLayout.Stride();

    static_assert(decltype(noBatchLayout)::depth == FOUR_DIM_DATA);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(noBatchLayout)>, NZLayoutPtn>);
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<0>(shape)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(shape)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<1>(shape)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(shape)), 4);
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<0>(stride)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(stride)), 256);
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<1>(stride)), 1);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(stride)), 512);
}

TEST_F(Tensor_Api_Layout_Struct, TestMakeTuple)
{
    using namespace AscendC::Te;

    auto shape = make_shape(2, 3, 4);
    auto stride = make_stride(12, 4, 1);
    auto tile = make_tile(8, 16);
    auto coord = make_coord(1, 2, 3);

    EXPECT_EQ(AscendC::Std::get<0>(shape), 2);
    EXPECT_EQ(AscendC::Std::get<1>(shape), 3);
    EXPECT_EQ(AscendC::Std::get<2>(shape), 4);

    EXPECT_EQ(AscendC::Std::get<0>(stride), 12);
    EXPECT_EQ(AscendC::Std::get<1>(stride), 4);
    EXPECT_EQ(AscendC::Std::get<2>(stride), 1);

    EXPECT_EQ(AscendC::Std::get<0>(tile), 8);
    EXPECT_EQ(AscendC::Std::get<1>(tile), 16);

    EXPECT_EQ(AscendC::Std::get<0>(coord), 1);
    EXPECT_EQ(AscendC::Std::get<1>(coord), 2);
    EXPECT_EQ(AscendC::Std::get<2>(coord), 3);
}

TEST_F(Tensor_Api_Layout_Struct, TestMakeIntTuple)
{
    using namespace AscendC::Te;

    auto shape = make_shape(_4{}, _5{});
    auto stride = make_stride(_5{}, _1{});
    auto coord = make_coord(_1{}, _3{});

    EXPECT_EQ(AscendC::Std::get<0>(shape).value, 4);
    EXPECT_EQ(AscendC::Std::get<1>(shape).value, 5);
    EXPECT_EQ(AscendC::Std::get<0>(stride).value, 5);
    EXPECT_EQ(AscendC::Std::get<1>(stride).value, 1);
    EXPECT_EQ(AscendC::Std::get<0>(coord).value, 1);
    EXPECT_EQ(AscendC::Std::get<1>(coord).value, 3);
}
