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

#include "tensor_api_runtime_validation_include.h"

namespace {

using tensor_api_runtime_validation::TrapException;

class TensorApiRuntimeValidation : public testing::Test {
protected:
    void SetUp() override
    {
        AscendC::SetGCoreType(2);
    }

    void TearDown() override
    {
        AscendC::SetGCoreType(0);
    }
};

template <typename LocationType, typename PointerType, typename LayoutType>
auto make_tensor_at(PointerType pointer, const LayoutType& layout)
{
    return AscendC::Te::MakeTensor(AscendC::Te::MakeMemPtr<LocationType>(pointer), layout);
}

TEST_F(TensorApiRuntimeValidation, MakeTensorRejectsZeroShape)
{
    using namespace AscendC::Te;

    int rows = 4;
    int columns = 0;
    __gm__ int32_t data[16] = {};
    auto layout = MakeLayout(MakeShape(rows, columns), MakeStride(4, 1));

    EXPECT_THROW((void)MakeTensor(MakeMemPtr<Location::GM>(data), layout), TrapException);
}

TEST_F(TensorApiRuntimeValidation, MakeTensorRejectsNegativeShape)
{
    using namespace AscendC::Te;

    int rows = 4;
    int columns = -1;
    __gm__ int32_t data[16] = {};
    auto shape = MakeShape(rows, columns);
    auto stride = MakeStride(4, 1);

    EXPECT_THROW((void)MakeTensor(MakeMemPtr<Location::GM>(data), shape, stride), TrapException);
}

TEST_F(TensorApiRuntimeValidation, MakeTensorRejectsNegativeStride)
{
    using namespace AscendC::Te;

    int row_stride = -4;
    __gm__ int32_t data[16] = {};
    auto shape = MakeShape(4, 4);
    auto stride = MakeStride(row_stride, 1);

    EXPECT_THROW((void)MakeTensor(MakeMemPtr<Location::GM>(data), shape, stride), TrapException);
}

TEST_F(TensorApiRuntimeValidation, SliceRejectsOutOfRangeCoordinate)
{
    using namespace AscendC::Te;

    __gm__ int32_t data[32] = {};
    auto tensor = make_tensor_at<Location::GM>(data, MakeFrameLayout<NDLayoutPtn, int32_t>(4, 4));
    auto coord = MakeCoord(4, 0);
    auto slice_shape = MakeShape(1, 1);

    EXPECT_THROW((void)Slice(tensor, coord, slice_shape), TrapException);
}

TEST_F(TensorApiRuntimeValidation, SliceRejectsZeroShape)
{
    using namespace AscendC::Te;

    int slice_columns = 0;
    __gm__ int32_t data[32] = {};
    auto tensor = make_tensor_at<Location::GM>(data, MakeFrameLayout<NDLayoutPtn, int32_t>(4, 4));
    auto coord = MakeCoord(0, 0);
    auto slice_shape = MakeShape(1, slice_columns);

    EXPECT_THROW((void)Slice(tensor, coord, slice_shape), TrapException);
}

TEST_F(TensorApiRuntimeValidation, SliceRejectsNegativeCoordinate)
{
    using namespace AscendC::Te;

    __gm__ int32_t data[32] = {};
    auto tensor = make_tensor_at<Location::GM>(data, MakeFrameLayout<NDLayoutPtn, int32_t>(4, 4));
    auto coord = MakeCoord(-1, 0);
    auto slice_shape = MakeShape(1, 1);

    EXPECT_THROW((void)Slice(tensor, coord, slice_shape), TrapException);
}

TEST_F(TensorApiRuntimeValidation, SliceRejectsZeroShapeInLayout)
{
    using namespace AscendC::Te;

    int slice_columns = 0;
    __gm__ int32_t data[32] = {};
    auto tensor = make_tensor_at<Location::GM>(data, MakeFrameLayout<NDLayoutPtn, int32_t>(4, 4));
    auto coord = MakeCoord(0, 0);
    auto slice_layout = MakeLayout(MakeShape(1, slice_columns), MakeStride(1, 1));

    EXPECT_THROW((void)Slice(tensor, coord, slice_layout), TrapException);
}

TEST_F(TensorApiRuntimeValidation, TensorMemberSliceRejectsOutOfRangeCoordinate)
{
    using namespace AscendC::Te;

    __gm__ int32_t data[32] = {};
    auto tensor = make_tensor_at<Location::GM>(data, MakeFrameLayout<NDLayoutPtn, int32_t>(4, 4));
    auto coord = MakeCoord(4, 0);

    EXPECT_THROW((void)tensor.Slice(coord, MakeShape(1, 1)), TrapException);
}

TEST_F(TensorApiRuntimeValidation, ConstTensorMemberSliceRejectsOutOfRangeCoordinate)
{
    using namespace AscendC::Te;

    __gm__ int32_t data[32] = {};
    const auto tensor = make_tensor_at<Location::GM>(data, MakeFrameLayout<NDLayoutPtn, int32_t>(4, 4));
    auto coord = MakeCoord(4, 0);

    EXPECT_THROW((void)tensor.Slice(coord, MakeShape(1, 1)), TrapException);
}

TEST_F(TensorApiRuntimeValidation, Crd2IdxRejectsNegativeCoordinate)
{
    using namespace AscendC::Te;

    auto coord = MakeCoord(-1, 0);
    auto shape = MakeShape(4, 8);
    auto stride = MakeStride(8, 1);

    EXPECT_THROW((void)Crd2Idx(coord, shape, stride), TrapException);
}

TEST_F(TensorApiRuntimeValidation, Crd2IdxRejectsCoordinateOutsideLayout)
{
    using namespace AscendC::Te;

    auto coord = MakeCoord(4, 0);
    auto layout = MakeLayout(MakeShape(4, 8), MakeStride(8, 1));

    EXPECT_THROW((void)Crd2Idx(coord, layout), TrapException);
}

TEST_F(TensorApiRuntimeValidation, LayoutIndexRejectsOutOfRangeCoordinate)
{
    using namespace AscendC::Te;

    auto coord = MakeCoord(4, 0);
    auto layout = MakeLayout(MakeShape(4, 8), MakeStride(8, 1));

    EXPECT_THROW((void)layout(coord), TrapException);
}

TEST_F(TensorApiRuntimeValidation, TensorElementAccessRejectsOutOfRangeCoordinate)
{
    using namespace AscendC::Te;

    __gm__ int32_t data[64] = {};
    auto tensor = make_tensor_at<Location::GM>(data, MakeFrameLayout<NDLayoutPtn, int32_t>(4, 8));
    auto coord = MakeCoord(4, 0);

    EXPECT_THROW((void)tensor[coord], TrapException);
}

TEST_F(TensorApiRuntimeValidation, ConstTensorElementAccessRejectsNegativeCoordinate)
{
    using namespace AscendC::Te;

    __gm__ int32_t data[64] = {};
    const auto tensor = make_tensor_at<Location::GM>(data, MakeFrameLayout<NDLayoutPtn, int32_t>(4, 8));
    auto coord = MakeCoord(-1, 0);

    EXPECT_THROW((void)tensor[coord], TrapException);
}

TEST_F(TensorApiRuntimeValidation, TensorTupleSubTensorRejectsOutOfRangeCoordinate)
{
    using namespace AscendC::Te;

    __gm__ int32_t data[64] = {};
    auto tensor = make_tensor_at<Location::GM>(data, MakeFrameLayout<NDLayoutPtn, int32_t>(4, 8));
    auto coord = MakeCoord(4, 0);

    EXPECT_THROW((void)tensor(coord), TrapException);
}

TEST_F(TensorApiRuntimeValidation, ConstTensorTupleSubTensorRejectsNegativeCoordinate)
{
    using namespace AscendC::Te;

    __gm__ int32_t data[64] = {};
    const auto tensor = make_tensor_at<Location::GM>(data, MakeFrameLayout<NDLayoutPtn, int32_t>(4, 8));
    auto coord = MakeCoord(-1, 0);

    EXPECT_THROW((void)tensor(coord), TrapException);
}

TEST_F(TensorApiRuntimeValidation, TensorSubTensorRejectsOutOfRangeCoordinate)
{
    using namespace AscendC::Te;

    __gm__ int32_t data[64] = {};
    auto tensor = make_tensor_at<Location::GM>(data, MakeFrameLayout<NDLayoutPtn, int32_t>(4, 8));

    EXPECT_THROW((void)tensor(4, 0), TrapException);
}

TEST_F(TensorApiRuntimeValidation, ConstTensorSubTensorRejectsNegativeCoordinate)
{
    using namespace AscendC::Te;

    __gm__ int32_t data[64] = {};
    const auto tensor = make_tensor_at<Location::GM>(data, MakeFrameLayout<NDLayoutPtn, int32_t>(4, 8));

    EXPECT_THROW((void)tensor(-1, 0), TrapException);
}

} // namespace
