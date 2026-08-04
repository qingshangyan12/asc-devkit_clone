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

class TensorApiDataStructureValidationMatrix : public testing::Test {
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

TEST_F(TensorApiDataStructureValidationMatrix, MakeTensorLayoutRejectsRemainingInvalidShapeAndStrideTopologies)
{
    using namespace AscendC::Te;

    alignas(512) __gm__ int32_t data[1024] = {};
    auto pointer = MakeMemPtr<Location::GM>(data);

    EXPECT_THROW((void)MakeTensor(pointer, MakeLayout(MakeShape(4, -1), MakeStride(4, 1))), TrapException);
    EXPECT_THROW((void)MakeTensor(pointer, MakeLayout(MakeShape(4, 4), MakeStride(-4, 1))), TrapException);

    auto nested_shape_with_zero = MakeShape(MakeShape(16, 2), MakeShape(8, 0));
    auto nested_stride = MakeStride(MakeStride(8, 128), MakeStride(1, 256));
    EXPECT_THROW((void)MakeTensor(pointer, MakeLayout(nested_shape_with_zero, nested_stride)), TrapException);

    auto batch_shape_with_negative = MakeShape(-1, MakeShape(4, 8));
    auto batch_stride = MakeStride(32, MakeStride(8, 1));
    EXPECT_THROW((void)MakeTensor(pointer, MakeLayout(batch_shape_with_negative, batch_stride)), TrapException);
}

TEST_F(TensorApiDataStructureValidationMatrix, MakeTensorShapeStrideRejectsRemainingInvalidTopologies)
{
    using namespace AscendC::Te;

    alignas(512) __gm__ int32_t data[1024] = {};
    auto pointer = MakeMemPtr<Location::GM>(data);

    EXPECT_THROW((void)MakeTensor(pointer, MakeShape(4, 0), MakeStride(4, 1)), TrapException);

    auto nested_shape_with_negative = MakeShape(MakeShape(16, 2), MakeShape(8, -1));
    auto nested_shape = MakeShape(MakeShape(16, 2), MakeShape(8, 4));
    auto nested_stride = MakeStride(MakeStride(8, 128), MakeStride(1, 256));
    auto nested_stride_with_negative = MakeStride(MakeStride(8, 128), MakeStride(1, -1));
    EXPECT_THROW((void)MakeTensor(pointer, nested_shape_with_negative, nested_stride), TrapException);
    EXPECT_THROW((void)MakeTensor(pointer, nested_shape, nested_stride_with_negative), TrapException);

    auto batch_shape_with_zero = MakeShape(0, MakeShape(4, 8));
    auto batch_shape = MakeShape(2, MakeShape(4, 8));
    auto batch_stride = MakeStride(32, MakeStride(8, 1));
    auto batch_stride_with_negative = MakeStride(-32, MakeStride(8, 1));
    EXPECT_THROW((void)MakeTensor(pointer, batch_shape_with_zero, batch_stride), TrapException);
    EXPECT_THROW((void)MakeTensor(pointer, batch_shape, batch_stride_with_negative), TrapException);
}

TEST_F(TensorApiDataStructureValidationMatrix, SliceRejectsInvalidCoordinatesAcrossInfoAndConstOverloads)
{
    using namespace AscendC::Te;

    alignas(512) __gm__ int32_t data[64] = {};
    auto tensor = make_tensor_at<Location::GM>(data, MakeFrameLayout<NDLayoutPtn, int32_t>(4, 8));
    const auto const_tensor = tensor;
    auto slice_layout = MakeLayout(MakeShape(1, 1), MakeStride(8, 1));

    EXPECT_THROW((void)Slice(tensor, MakeCoord(4, 0), slice_layout), TrapException);
    EXPECT_THROW((void)Slice(const_tensor, MakeCoord(-1, 0), slice_layout), TrapException);
    EXPECT_THROW((void)tensor.Slice(MakeCoord(4, 0), slice_layout), TrapException);
    EXPECT_THROW((void)const_tensor.Slice(MakeCoord(4, 0), slice_layout), TrapException);
}

TEST_F(TensorApiDataStructureValidationMatrix, MemberSliceRejectsInvalidShapeInfoAcrossConstOverloads)
{
    using namespace AscendC::Te;

    alignas(512) __gm__ int32_t data[64] = {};
    auto tensor = make_tensor_at<Location::GM>(data, MakeFrameLayout<NDLayoutPtn, int32_t>(4, 8));
    const auto const_tensor = tensor;

    EXPECT_THROW((void)tensor.Slice(MakeCoord(0, 0), MakeShape(1, -1)), TrapException);
    EXPECT_THROW((void)const_tensor.Slice(MakeCoord(0, 0), MakeShape(1, 0)), TrapException);
}

TEST_F(TensorApiDataStructureValidationMatrix, SliceRejectsInvalidBatchCoordinateWithLayoutInfo)
{
    using namespace AscendC::Te;

    alignas(512) __cbuf__ int32_t data[2 * 32 * 32] = {};
    const auto tensor = make_tensor_at<Location::L1>(data, MakeFrameLayout<NZLayoutPtn, int32_t>(2, 32, 32));
    auto slice_layout = MakeFrameLayout<NZLayoutPtn, int32_t>(1, 16, 16);
    auto matrix_coord = MakeCoord(MakeCoord(0, 0), MakeCoord(0, 0));

    EXPECT_THROW((void)tensor.Slice(MakeCoord(2, matrix_coord), slice_layout), TrapException);
}

TEST_F(TensorApiDataStructureValidationMatrix, Crd2IdxShapeStrideRejectsAllCoordinateTopologies)
{
    using namespace AscendC::Te;

    auto flat_shape = MakeShape(4, 8);
    auto flat_stride = MakeStride(8, 1);
    EXPECT_THROW((void)Crd2Idx(-1, flat_shape, flat_stride), TrapException);

    auto nested_shape = MakeShape(MakeShape(16, 2), MakeShape(8, 4));
    auto nested_stride = MakeStride(MakeStride(8, 128), MakeStride(1, 256));
    EXPECT_THROW((void)Crd2Idx(MakeCoord(MakeCoord(-1, 0), MakeCoord(0, 0)), nested_shape, nested_stride),
                 TrapException);
    EXPECT_THROW((void)Crd2Idx(MakeCoord(MakeCoord(16, 0), MakeCoord(0, 0)), nested_shape, nested_stride),
                 TrapException);
    EXPECT_THROW((void)Crd2Idx(MakeCoord(-1, 0), nested_shape, nested_stride), TrapException);

    auto flat_batch_shape = MakeShape(2, 4, 8);
    auto flat_batch_stride = MakeStride(32, 8, 1);
    EXPECT_THROW((void)Crd2Idx(MakeCoord(2, 0, 0), flat_batch_shape, flat_batch_stride), TrapException);

    auto nested_batch_shape = MakeShape(2, nested_shape);
    auto nested_batch_stride = MakeStride(1024, nested_stride);
    EXPECT_THROW((void)Crd2Idx(MakeCoord(2, MakeCoord(0, 0)), nested_batch_shape, nested_batch_stride), TrapException);
}

TEST_F(TensorApiDataStructureValidationMatrix, Crd2IdxLayoutRejectsAllCoordinateTopologies)
{
    using namespace AscendC::Te;

    auto flat_layout = MakeLayout(MakeShape(4, 8), MakeStride(8, 1));
    EXPECT_THROW((void)Crd2Idx(-1, flat_layout), TrapException);
    EXPECT_THROW((void)Crd2Idx(32, flat_layout), TrapException);

    auto nested_shape = MakeShape(MakeShape(16, 2), MakeShape(8, 4));
    auto nested_stride = MakeStride(MakeStride(8, 128), MakeStride(1, 256));
    auto nested_layout = MakeLayout(nested_shape, nested_stride);
    EXPECT_THROW((void)Crd2Idx(MakeCoord(MakeCoord(-1, 0), MakeCoord(0, 0)), nested_layout), TrapException);
    EXPECT_THROW((void)Crd2Idx(MakeCoord(32, 0), nested_layout), TrapException);

    auto flat_batch_layout = MakeLayout(MakeShape(2, 4, 8), MakeStride(32, 8, 1));
    EXPECT_THROW((void)Crd2Idx(MakeCoord(2, 0, 0), flat_batch_layout), TrapException);
}

TEST_F(TensorApiDataStructureValidationMatrix, LayoutIndexRejectsAllCoordinateTopologies)
{
    using namespace AscendC::Te;

    auto flat_layout = MakeLayout(MakeShape(4, 8), MakeStride(8, 1));
    EXPECT_THROW((void)flat_layout(-1), TrapException);

    auto nested_shape = MakeShape(MakeShape(16, 2), MakeShape(8, 4));
    auto nested_stride = MakeStride(MakeStride(8, 128), MakeStride(1, 256));
    auto nested_layout = MakeLayout(nested_shape, nested_stride);
    EXPECT_THROW((void)nested_layout(MakeCoord(MakeCoord(-1, 0), MakeCoord(0, 0))), TrapException);
    EXPECT_THROW((void)nested_layout(MakeCoord(MakeCoord(16, 0), MakeCoord(0, 0))), TrapException);
    EXPECT_THROW((void)nested_layout(MakeCoord(-1, 0)), TrapException);
    EXPECT_THROW((void)nested_layout(MakeCoord(32, 0)), TrapException);

    auto flat_batch_layout = MakeLayout(MakeShape(2, 4, 8), MakeStride(32, 8, 1));
    EXPECT_THROW((void)flat_batch_layout(MakeCoord(2, 0, 0)), TrapException);
}

TEST_F(TensorApiDataStructureValidationMatrix, TensorElementAccessRejectsConstNestedAndBatchCoordinates)
{
    using namespace AscendC::Te;

    alignas(512) __ubuf__ int32_t nested_data[32 * 32] = {};
    const auto nested_tensor = make_tensor_at<Location::UB>(nested_data, MakeFrameLayout<NZLayoutPtn, int32_t>(32, 32));
    auto invalid_nested_coord = MakeCoord(MakeCoord(0, 0), MakeCoord(8, 0));
    EXPECT_THROW((void)nested_tensor[invalid_nested_coord], TrapException);

    alignas(512) __gm__ int32_t batch_data[2 * 4 * 8] = {};
    auto batch_tensor = make_tensor_at<Location::GM>(batch_data, MakeFrameLayout<NDExtLayoutPtn, int32_t>(2, 4, 8));
    auto matrix_origin = MakeCoord(MakeCoord(0, 0), MakeCoord(0, 0));
    EXPECT_THROW((void)batch_tensor[MakeCoord(2, matrix_origin)], TrapException);
}

TEST_F(TensorApiDataStructureValidationMatrix, TensorSubTensorRejectsConstNestedAndBatchCoordinates)
{
    using namespace AscendC::Te;

    alignas(512) __cbuf__ int32_t nested_data[32 * 32] = {};
    const auto nested_tensor = make_tensor_at<Location::L1>(nested_data, MakeFrameLayout<NZLayoutPtn, int32_t>(32, 32));
    EXPECT_THROW((void)nested_tensor(MakeCoord(MakeCoord(0, 0), MakeCoord(8, 0))), TrapException);

    alignas(512) __cbuf__ int32_t batch_data[2 * 32 * 32] = {};
    const auto batch_tensor =
        make_tensor_at<Location::L1>(batch_data, MakeFrameLayout<NZLayoutPtn, int32_t>(2, 32, 32));
    auto matrix_coord = MakeCoord(MakeCoord(0, 0), MakeCoord(0, 0));
    EXPECT_THROW((void)batch_tensor(MakeCoord(2, matrix_coord)), TrapException);
    EXPECT_THROW((void)batch_tensor(2, matrix_coord), TrapException);
}

} // namespace
