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

class TensorApiDataStructureStrictValidation : public testing::Test {
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

template <typename LocationType, typename PointerType, typename LayoutType>
auto make_tensor_directly(PointerType pointer, const LayoutType& layout)
{
    using namespace AscendC::Te;

    auto iterator = MakeMemPtr<LocationType>(pointer);
    using TensorType = decltype(MakeTensor(iterator, layout));
    using EngineType = typename TensorType::engineType;
    return TensorType{EngineType{iterator}, layout};
}

template <typename TensorType>
void expect_all_slice_source_entry_traps(TensorType& tensor)
{
    using namespace AscendC::Te;

    const auto const_tensor = tensor;
    auto slice_shape = MakeShape(1, 1);
    auto slice_layout = MakeFrameLayout<NDLayoutPtn, int32_t>(1, 1);
    auto coord = MakeCoord(0, 0);

    EXPECT_THROW((void)Slice(tensor, coord, slice_shape), TrapException);
    EXPECT_THROW((void)Slice(tensor, coord, slice_layout), TrapException);
    EXPECT_THROW((void)Slice(const_tensor, coord, slice_shape), TrapException);
    EXPECT_THROW((void)Slice(const_tensor, coord, slice_layout), TrapException);
    EXPECT_THROW((void)tensor.Slice(coord, slice_shape), TrapException);
    EXPECT_THROW((void)tensor.Slice(coord, slice_layout), TrapException);
    EXPECT_THROW((void)const_tensor.Slice(coord, slice_shape), TrapException);
    EXPECT_THROW((void)const_tensor.Slice(coord, slice_layout), TrapException);
}

template <typename TensorType>
void expect_flat_tensor_coordinate_traps(TensorType& tensor)
{
    using namespace AscendC::Te;

    const auto const_tensor = tensor;
    auto invalid_coord = MakeCoord(4, 0);

    EXPECT_THROW((void)tensor[invalid_coord], TrapException);
    EXPECT_THROW((void)const_tensor[invalid_coord], TrapException);
    EXPECT_THROW((void)tensor[32], TrapException);
    EXPECT_THROW((void)const_tensor[32], TrapException);
    EXPECT_THROW((void)tensor(invalid_coord), TrapException);
    EXPECT_THROW((void)const_tensor(invalid_coord), TrapException);
    EXPECT_THROW((void)tensor(4, 0), TrapException);
    EXPECT_THROW((void)const_tensor(4, 0), TrapException);
}

template <typename TensorType>
void expect_nested_tensor_coordinate_traps(TensorType& tensor)
{
    using namespace AscendC::Te;

    const auto const_tensor = tensor;
    auto valid_row_coord = MakeCoord(0, 0);
    auto invalid_column_coord = MakeCoord(8, 0);
    auto invalid_coord = MakeCoord(valid_row_coord, invalid_column_coord);
    auto invalid_logical_coord = MakeCoord(0, 32);

    EXPECT_THROW((void)tensor[invalid_coord], TrapException);
    EXPECT_THROW((void)const_tensor[invalid_coord], TrapException);
    EXPECT_THROW((void)tensor[invalid_logical_coord], TrapException);
    EXPECT_THROW((void)const_tensor[invalid_logical_coord], TrapException);
    EXPECT_THROW((void)tensor(invalid_coord), TrapException);
    EXPECT_THROW((void)const_tensor(invalid_coord), TrapException);
    EXPECT_THROW((void)tensor(valid_row_coord, invalid_column_coord), TrapException);
    EXPECT_THROW((void)const_tensor(valid_row_coord, invalid_column_coord), TrapException);
}

template <typename TensorType>
void expect_batch_tensor_coordinate_traps(TensorType& tensor)
{
    using namespace AscendC::Te;

    const auto const_tensor = tensor;
    auto matrix_origin = MakeCoord(MakeCoord(0, 0), MakeCoord(0, 0));
    auto negative_batch_coord = MakeCoord(-1, matrix_origin);
    auto invalid_matrix_coord = MakeCoord(MakeCoord(0, 0), MakeCoord(8, 0));
    auto invalid_inner_coord = MakeCoord(0, invalid_matrix_coord);

    EXPECT_THROW((void)tensor[negative_batch_coord], TrapException);
    EXPECT_THROW((void)tensor[invalid_inner_coord], TrapException);
    EXPECT_THROW((void)const_tensor[negative_batch_coord], TrapException);
    EXPECT_THROW((void)const_tensor[invalid_inner_coord], TrapException);
    EXPECT_THROW((void)tensor(negative_batch_coord), TrapException);
    EXPECT_THROW((void)tensor(invalid_inner_coord), TrapException);
    EXPECT_THROW((void)const_tensor(negative_batch_coord), TrapException);
    EXPECT_THROW((void)const_tensor(invalid_inner_coord), TrapException);
    EXPECT_THROW((void)tensor(-1, matrix_origin), TrapException);
    EXPECT_THROW((void)tensor(0, invalid_matrix_coord), TrapException);
    EXPECT_THROW((void)const_tensor(-1, matrix_origin), TrapException);
    EXPECT_THROW((void)const_tensor(0, invalid_matrix_coord), TrapException);
}

TEST_F(TensorApiDataStructureStrictValidation, MakeTensorLocalOverloadsRejectNegativeShapeAndStride)
{
    using namespace AscendC::Te;

    alignas(512) __ubuf__ int32_t data[64] = {};
    auto pointer = MakeMemPtr<Location::UB>(data);

    auto negative_shape_layout =
        MakePatternLayout<NDLayoutPtn, LayoutTrait<AscendC::Std::ignore_t, _1>>(MakeShape(4, -1), MakeStride(4, _1{}));
    auto negative_stride_layout =
        MakePatternLayout<NDLayoutPtn, LayoutTrait<AscendC::Std::ignore_t, _1>>(MakeShape(4, 4), MakeStride(-4, _1{}));

    EXPECT_THROW((void)MakeTensor(pointer, negative_shape_layout), TrapException);
    EXPECT_THROW((void)MakeTensor(pointer, negative_stride_layout), TrapException);
    EXPECT_THROW((void)MakeTensor(pointer, MakeShape(4, -1), MakeStride(4, _1{})), TrapException);
    EXPECT_THROW((void)MakeTensor(pointer, MakeShape(4, 4), MakeStride(-4, _1{})), TrapException);
}

TEST_F(TensorApiDataStructureStrictValidation, SliceSourceValidationCoversAllPublicEntryForms)
{
    using namespace AscendC::Te;

    alignas(512) __gm__ int32_t data[64] = {};
    auto zero_shape_layout =
        MakePatternLayout<NDLayoutPtn, LayoutTrait<AscendC::Std::ignore_t, _1>>(MakeShape(4, 0), MakeStride(4, _1{}));
    auto negative_shape_layout =
        MakePatternLayout<NDLayoutPtn, LayoutTrait<AscendC::Std::ignore_t, _1>>(MakeShape(4, -1), MakeStride(4, _1{}));
    auto negative_stride_layout =
        MakePatternLayout<NDLayoutPtn, LayoutTrait<AscendC::Std::ignore_t, _1>>(MakeShape(4, 4), MakeStride(-4, _1{}));

    auto zero_shape_tensor = make_tensor_directly<Location::GM>(data, zero_shape_layout);
    auto negative_shape_tensor = make_tensor_directly<Location::GM>(data, negative_shape_layout);
    auto negative_stride_tensor = make_tensor_directly<Location::GM>(data, negative_stride_layout);

    expect_all_slice_source_entry_traps(zero_shape_tensor);
    expect_all_slice_source_entry_traps(negative_shape_tensor);
    expect_all_slice_source_entry_traps(negative_stride_tensor);
}

TEST_F(TensorApiDataStructureStrictValidation, SliceRejectsInvalidBatchCoordinatesForNDAndNZ)
{
    using namespace AscendC::Te;

    alignas(512) __gm__ int32_t nd_data[2 * 4 * 8] = {};
    auto nd_tensor = make_tensor_at<Location::GM>(nd_data, MakeFrameLayout<NDLayoutPtn, int32_t>(2, 4, 8));
    const auto const_nd_tensor = nd_tensor;
    auto nd_slice_shape = MakeShape(1, MakeShape(1, 1));
    auto nd_slice_layout = MakeFrameLayout<NDLayoutPtn, int32_t>(1, 1, 1);

    EXPECT_THROW((void)Slice(nd_tensor, MakeCoord(-1, MakeCoord(0, 0)), nd_slice_shape), TrapException);
    EXPECT_THROW((void)const_nd_tensor.Slice(MakeCoord(0, MakeCoord(4, 0)), nd_slice_layout), TrapException);

    alignas(512) __cbuf__ int32_t nz_data[2 * 32 * 32] = {};
    auto nz_tensor = make_tensor_at<Location::L1>(nz_data, MakeFrameLayout<NZLayoutPtn, int32_t>(2, 32, 32));
    const auto const_nz_tensor = nz_tensor;
    auto nz_slice_shape = MakeShape(1, MakeShape(16, 16));
    auto nz_slice_layout = MakeFrameLayout<NZLayoutPtn, int32_t>(1, 16, 16);

    EXPECT_THROW((void)nz_tensor.Slice(MakeCoord(-1, MakeCoord(0, 0)), nz_slice_shape), TrapException);
    auto invalid_nz_coord = MakeCoord(0, MakeCoord(MakeCoord(0, 0), MakeCoord(8, 0)));
    EXPECT_THROW((void)Slice(const_nz_tensor, invalid_nz_coord, nz_slice_layout), TrapException);
}

TEST_F(TensorApiDataStructureStrictValidation, SliceRejectsInvalidLogicalAndNestedNzCoordinates)
{
    using namespace AscendC::Te;

    alignas(512) __cbuf__ int32_t data[32 * 32] = {};
    auto tensor = make_tensor_at<Location::L1>(data, MakeFrameLayout<NZLayoutPtn, int32_t>(32, 32));
    const auto const_tensor = tensor;
    auto slice_layout = MakeFrameLayout<NZLayoutPtn, int32_t>(16, 16);

    EXPECT_THROW((void)const_tensor.Slice(MakeCoord(-1, 0), MakeShape(1, 1)), TrapException);
    EXPECT_THROW((void)Slice(tensor, MakeCoord(MakeCoord(-1, 0), MakeCoord(0, 0)), slice_layout), TrapException);
    EXPECT_THROW((void)tensor.Slice(MakeCoord(MakeCoord(0, 2), MakeCoord(0, 0)), slice_layout), TrapException);
}

TEST_F(TensorApiDataStructureStrictValidation, Crd2IdxAndLayoutRejectInvalidBatchCoordinates)
{
    using namespace AscendC::Te;

    auto flat_shape = MakeShape(2, 4, 8);
    auto flat_stride = MakeStride(32, 8, 1);
    auto flat_layout = MakeLayout(flat_shape, flat_stride);
    auto flat_negative_batch = MakeCoord(-1, 0, 0);
    auto flat_invalid_inner = MakeCoord(0, 4, 0);

    EXPECT_THROW((void)Crd2Idx(flat_negative_batch, flat_shape, flat_stride), TrapException);
    EXPECT_THROW((void)Crd2Idx(flat_invalid_inner, flat_shape, flat_stride), TrapException);
    EXPECT_THROW((void)Crd2Idx(flat_negative_batch, flat_layout), TrapException);
    EXPECT_THROW((void)Crd2Idx(flat_invalid_inner, flat_layout), TrapException);
    EXPECT_THROW((void)flat_layout(flat_negative_batch), TrapException);
    EXPECT_THROW((void)flat_layout(flat_invalid_inner), TrapException);

    auto matrix_shape = MakeShape(MakeShape(16, 2), MakeShape(8, 4));
    auto matrix_stride = MakeStride(MakeStride(8, 128), MakeStride(1, 256));
    auto nested_batch_shape = MakeShape(2, matrix_shape);
    auto nested_batch_stride = MakeStride(1024, matrix_stride);
    auto nested_batch_layout = MakeLayout(nested_batch_shape, nested_batch_stride);
    auto nested_negative_batch = MakeCoord(-1, MakeCoord(0, 0));
    auto nested_invalid_inner = MakeCoord(0, MakeCoord(32, 0));

    EXPECT_THROW((void)Crd2Idx(nested_negative_batch, nested_batch_shape, nested_batch_stride), TrapException);
    EXPECT_THROW((void)Crd2Idx(nested_invalid_inner, nested_batch_shape, nested_batch_stride), TrapException);
    EXPECT_THROW((void)Crd2Idx(nested_negative_batch, nested_batch_layout), TrapException);
    EXPECT_THROW((void)Crd2Idx(nested_invalid_inner, nested_batch_layout), TrapException);
    EXPECT_THROW((void)nested_batch_layout(nested_negative_batch), TrapException);
    EXPECT_THROW((void)nested_batch_layout(nested_invalid_inner), TrapException);
}

TEST_F(TensorApiDataStructureStrictValidation, TensorCoordinateEntriesCoverConstnessAndTopologies)
{
    using namespace AscendC::Te;

    alignas(512) __gm__ int32_t flat_data[4 * 8] = {};
    auto flat_tensor = make_tensor_at<Location::GM>(flat_data, MakeFrameLayout<NDLayoutPtn, int32_t>(4, 8));
    expect_flat_tensor_coordinate_traps(flat_tensor);

    alignas(512) __ubuf__ int32_t nested_data[32 * 32] = {};
    auto nested_tensor = make_tensor_at<Location::UB>(nested_data, MakeFrameLayout<NZLayoutPtn, int32_t>(32, 32));
    expect_nested_tensor_coordinate_traps(nested_tensor);

    alignas(512) __ubuf__ int32_t batch_data[2 * 32 * 32] = {};
    auto batch_tensor = make_tensor_at<Location::UB>(batch_data, MakeFrameLayout<NZLayoutPtn, int32_t>(2, 32, 32));
    expect_batch_tensor_coordinate_traps(batch_tensor);
}

} // namespace
