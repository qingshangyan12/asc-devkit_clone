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

class TensorApiDataStructureAdditionalValidation : public testing::Test {
protected:
    void SetUp() override { AscendC::SetGCoreType(2); }

    void TearDown() override { AscendC::SetGCoreType(0); }
};

template <typename LocationType, typename PointerType, typename LayoutType>
auto make_tensor_at(PointerType pointer, const LayoutType& layout)
{
    return AscendC::Te::MakeTensor(AscendC::Te::MakeMemPtr<LocationType>(pointer), layout);
}

template <typename LocationType, typename PointerType, typename LayoutType>
auto make_tensor_directly(PointerType pointer, const LayoutType& layout)
{
    using namespace AscendC::Te;

    auto iterator = MakeMemPtr<LocationType>(pointer);
    using TensorType = decltype(MakeTensor(iterator, layout));
    using EngineType = typename TensorType::engineType;
    return TensorType{EngineType{iterator}, layout};
}

TEST_F(TensorApiDataStructureAdditionalValidation, MakeTensorLayoutRejectsNegativeNestedShape)
{
    using namespace AscendC::Te;

    alignas(512) __gm__ int32_t data[32] = {};
    auto shape = MakeShape(MakeShape(16, -1), MakeShape(8, 2));
    auto stride = MakeStride(MakeStride(8, 128), MakeStride(1, 256));
    auto layout = MakeLayout(shape, stride);

    EXPECT_THROW((void)MakeTensor(MakeMemPtr<Location::GM>(data), layout), TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, MakeTensorLayoutRejectsNegativeNestedStride)
{
    using namespace AscendC::Te;

    alignas(512) __gm__ int32_t data[32] = {};
    auto shape = MakeShape(MakeShape(16, 2), MakeShape(8, 2));
    auto stride = MakeStride(MakeStride(8, 128), MakeStride(1, -1));
    auto layout = MakeLayout(shape, stride);

    EXPECT_THROW((void)MakeTensor(MakeMemPtr<Location::GM>(data), layout), TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, MakeTensorShapeStrideRejectsZeroNestedShape)
{
    using namespace AscendC::Te;

    alignas(512) __gm__ int32_t data[32] = {};
    auto shape = MakeShape(MakeShape(16, 2), MakeShape(8, 0));
    auto stride = MakeStride(MakeStride(8, 128), MakeStride(1, 256));

    EXPECT_THROW((void)MakeTensor(MakeMemPtr<Location::GM>(data), shape, stride), TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, MakeTensorLocalRejectsZeroBatchShape)
{
    using namespace AscendC::Te;

    alignas(512) __ubuf__ int32_t data[32] = {};
    auto layout = MakeFrameLayout<NDExtLayoutPtn, int32_t>(0, 4, 4);

    EXPECT_THROW((void)MakeTensor(MakeMemPtr<Location::UB>(data), layout), TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, MakeTensorShapeStrideRejectsNegativeBatchShape)
{
    using namespace AscendC::Te;

    alignas(512) __gm__ int32_t data[32] = {};
    auto matrix_shape = MakeShape(MakeShape(16, 2), MakeShape(8, 2));
    auto matrix_stride = MakeStride(MakeStride(8, 128), MakeStride(1, 256));
    auto shape = MakeShape(-1, matrix_shape);
    auto stride = MakeStride(512, matrix_stride);

    EXPECT_THROW((void)MakeTensor(MakeMemPtr<Location::GM>(data), shape, stride), TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, MakeTensorLayoutRejectsNegativeBatchStride)
{
    using namespace AscendC::Te;

    alignas(512) __gm__ int32_t data[32] = {};
    auto matrix_shape = MakeShape(MakeShape(16, 2), MakeShape(8, 2));
    auto matrix_stride = MakeStride(MakeStride(8, 128), MakeStride(1, 256));
    auto layout = MakeLayout(MakeShape(2, matrix_shape), MakeStride(-512, matrix_stride));

    EXPECT_THROW((void)MakeTensor(MakeMemPtr<Location::GM>(data), layout), TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, FreeSliceShapeRejectsNegativeSliceShape)
{
    using namespace AscendC::Te;

    alignas(512) __gm__ int32_t data[32] = {};
    auto tensor = make_tensor_at<Location::GM>(data, MakeFrameLayout<NDLayoutPtn, int32_t>(4, 4));

    EXPECT_THROW((void)Slice(tensor, MakeCoord(0, 0), MakeShape(1, -1)), TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, FreeSliceLayoutRejectsNegativeSliceShape)
{
    using namespace AscendC::Te;

    alignas(512) __gm__ int32_t data[32] = {};
    auto tensor = make_tensor_at<Location::GM>(data, MakeFrameLayout<NDLayoutPtn, int32_t>(4, 4));
    auto slice_layout = MakeLayout(MakeShape(-1, 1), MakeStride(4, 1));

    EXPECT_THROW((void)Slice(tensor, MakeCoord(0, 0), slice_layout), TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, FreeSliceRejectsInvalidSourceShape)
{
    using namespace AscendC::Te;

    alignas(512) __gm__ int32_t data[32] = {};
    auto layout = MakeLayout(MakeShape(4, 0), MakeStride(4, 1));
    auto tensor = make_tensor_directly<Location::GM>(data, layout);

    EXPECT_THROW((void)Slice(tensor, MakeCoord(0, 0), MakeShape(1, 1)), TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, FreeSliceRejectsInvalidSourceStride)
{
    using namespace AscendC::Te;

    alignas(512) __gm__ int32_t data[32] = {};
    auto layout = MakeLayout(MakeShape(4, 4), MakeStride(-4, 1));
    auto tensor = make_tensor_directly<Location::GM>(data, layout);

    EXPECT_THROW((void)Slice(tensor, MakeCoord(0, 0), MakeShape(1, 1)), TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, TensorMemberSliceLayoutRejectsZeroSliceShape)
{
    using namespace AscendC::Te;

    alignas(512) __gm__ int32_t data[32] = {};
    auto tensor = make_tensor_at<Location::GM>(data, MakeFrameLayout<NDLayoutPtn, int32_t>(4, 4));
    auto slice_layout = MakeLayout(MakeShape(1, 0), MakeStride(4, 1));

    EXPECT_THROW((void)tensor.Slice(MakeCoord(0, 0), slice_layout), TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, ConstTensorMemberSliceLayoutRejectsNegativeSliceShape)
{
    using namespace AscendC::Te;

    alignas(512) __gm__ int32_t data[32] = {};
    const auto tensor = make_tensor_at<Location::GM>(data, MakeFrameLayout<NDLayoutPtn, int32_t>(4, 4));
    auto slice_layout = MakeLayout(MakeShape(1, -1), MakeStride(4, 1));

    EXPECT_THROW((void)tensor.Slice(MakeCoord(0, 0), slice_layout), TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, FreeSliceConstTensorRejectsNegativeCoordinate)
{
    using namespace AscendC::Te;

    alignas(512) __gm__ int32_t data[32] = {};
    const auto tensor = make_tensor_at<Location::GM>(data, MakeFrameLayout<NDLayoutPtn, int32_t>(4, 4));

    EXPECT_THROW((void)Slice(tensor, MakeCoord(0, -1), MakeShape(1, 1)), TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, TensorMemberSliceRejectsLogicalCoordinateOutsideNzShape)
{
    using namespace AscendC::Te;

    alignas(512) __cbuf__ int32_t data[32 * 32] = {};
    auto tensor = make_tensor_at<Location::L1>(data, MakeFrameLayout<NZLayoutPtn, int32_t>(32, 32));

    auto invalid_coord = MakeCoord(32, 0);
    EXPECT_THROW((void)tensor.Slice(invalid_coord, MakeShape(1, 1)), TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, TensorMemberSliceRejectsBatchCoordinateOutsideNzShape)
{
    using namespace AscendC::Te;

    alignas(512) __cbuf__ int32_t data[2 * 32 * 32] = {};
    auto tensor = make_tensor_at<Location::L1>(data, MakeFrameLayout<NZLayoutPtn, int32_t>(2, 32, 32));
    auto slice_shape = MakeShape(1, MakeShape(16, 16));

    auto matrix_origin = MakeCoord(0, 0);
    EXPECT_THROW((void)tensor.Slice(MakeCoord(2, matrix_origin), slice_shape), TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, Crd2IdxShapeStrideRejectsCoordinateAtUpperBound)
{
    using namespace AscendC::Te;

    auto shape = MakeShape(4, 8);
    auto stride = MakeStride(8, 1);

    EXPECT_THROW((void)Crd2Idx(MakeCoord(4, 0), shape, stride), TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, Crd2IdxLayoutRejectsNegativeCoordinate)
{
    using namespace AscendC::Te;

    auto layout = MakeLayout(MakeShape(4, 8), MakeStride(8, 1));

    EXPECT_THROW((void)Crd2Idx(MakeCoord(0, -1), layout), TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, Crd2IdxShapeStrideRejectsLinearCoordinateAtUpperBound)
{
    using namespace AscendC::Te;

    auto shape = MakeShape(4, 8);
    auto stride = MakeStride(8, 1);

    EXPECT_THROW((void)Crd2Idx(32, shape, stride), TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, Crd2IdxLayoutRejectsNestedCoordinateOutsideShape)
{
    using namespace AscendC::Te;

    auto shape = MakeShape(MakeShape(16, 2), MakeShape(8, 4));
    auto stride = MakeStride(MakeStride(8, 128), MakeStride(1, 256));
    auto layout = MakeLayout(shape, stride);
    auto coord = MakeCoord(MakeCoord(16, 0), MakeCoord(0, 0));

    EXPECT_THROW((void)Crd2Idx(coord, layout), TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, Crd2IdxShapeStrideRejectsLogicalCoordinateOutsideNestedShape)
{
    using namespace AscendC::Te;

    auto shape = MakeShape(MakeShape(16, 2), MakeShape(8, 4));
    auto stride = MakeStride(MakeStride(8, 128), MakeStride(1, 256));

    EXPECT_THROW((void)Crd2Idx(MakeCoord(32, 0), shape, stride), TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, Crd2IdxLayoutRejectsBatchCoordinateOutsideShape)
{
    using namespace AscendC::Te;

    auto matrix_shape = MakeShape(MakeShape(16, 2), MakeShape(8, 4));
    auto matrix_stride = MakeStride(MakeStride(8, 128), MakeStride(1, 256));
    auto layout = MakeLayout(MakeShape(2, matrix_shape), MakeStride(1024, matrix_stride));

    EXPECT_THROW((void)Crd2Idx(MakeCoord(2, MakeCoord(0, 0)), layout), TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, LayoutIndexRejectsNegativeCoordinate)
{
    using namespace AscendC::Te;

    auto layout = MakeLayout(MakeShape(4, 8), MakeStride(8, 1));

    EXPECT_THROW((void)layout(MakeCoord(-1, 0)), TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, LayoutIndexRejectsLinearCoordinateAtUpperBound)
{
    using namespace AscendC::Te;

    auto layout = MakeLayout(MakeShape(4, 8), MakeStride(8, 1));

    EXPECT_THROW((void)layout(32), TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, LayoutIndexRejectsBatchCoordinateOutsideNestedShape)
{
    using namespace AscendC::Te;

    auto matrix_shape = MakeShape(MakeShape(16, 2), MakeShape(8, 4));
    auto matrix_stride = MakeStride(MakeStride(8, 128), MakeStride(1, 256));
    auto layout = MakeLayout(MakeShape(2, matrix_shape), MakeStride(1024, matrix_stride));

    EXPECT_THROW((void)layout(MakeCoord(2, MakeCoord(0, 0))), TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, TensorElementAccessRejectsCoordinateOutsideNzShapeInUb)
{
    using namespace AscendC::Te;

    alignas(512) __ubuf__ int32_t data[32 * 32] = {};
    auto tensor = make_tensor_at<Location::UB>(data, MakeFrameLayout<NZLayoutPtn, int32_t>(32, 32));

    auto invalid_coord = MakeCoord(MakeCoord(0, 0), MakeCoord(8, 0));
    EXPECT_THROW((void)tensor[invalid_coord], TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, TensorElementAccessRejectsLinearCoordinateAtUpperBound)
{
    using namespace AscendC::Te;

    alignas(512) __gm__ int32_t data[8] = {};
    auto tensor = make_tensor_at<Location::GM>(data, MakeLayout(MakeShape(8), MakeStride(1)));

    EXPECT_THROW((void)tensor[8], TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, TensorSubTensorRejectsCoordinateOutsideNzShapeInL1)
{
    using namespace AscendC::Te;

    alignas(512) __cbuf__ int32_t data[32 * 32] = {};
    auto tensor = make_tensor_at<Location::L1>(data, MakeFrameLayout<NZLayoutPtn, int32_t>(32, 32));
    auto coord = MakeCoord(MakeCoord(0, 0), MakeCoord(8, 0));

    EXPECT_THROW((void)tensor(coord), TrapException);
}

TEST_F(TensorApiDataStructureAdditionalValidation, TensorVariadicSubTensorRejectsBatchCoordinateOutsideNzShape)
{
    using namespace AscendC::Te;

    alignas(512) __cbuf__ int32_t data[2 * 32 * 32] = {};
    auto tensor = make_tensor_at<Location::L1>(data, MakeFrameLayout<NZLayoutPtn, int32_t>(2, 32, 32));
    auto matrix_coord = MakeCoord(MakeCoord(0, 0), MakeCoord(0, 0));

    EXPECT_THROW((void)tensor(2, matrix_coord), TrapException);
}

} // namespace
