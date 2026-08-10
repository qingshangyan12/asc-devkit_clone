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

constexpr int VALID_ROWS = 16;
constexpr int VALID_COLUMNS = 16;
constexpr int SMALL_ROWS = 16;
constexpr int LARGE_ROWS = 64;
constexpr int ROUTE_COLUMNS = 32;
constexpr int BLOCK_COUNT_LIMIT_PLUS_ONE = 4096;
constexpr int FRACTAL_BLOCK_COUNT_LIMIT_PLUS_ONE = BLOCK_COUNT_LIMIT_PLUS_ONE * 16;
constexpr int HALF_ELEMENTS_FOR_BLOCK_LEN_LIMIT_PLUS_ONE = 1048576;
constexpr int FRACTAL_ELEMENTS_FOR_BLOCK_LEN_LIMIT_PLUS_ONE = 65536;
constexpr int MISALIGNED_STRIDE_ELEMENTS = 17;

class TensorApiVectorCopyAbnormalValidation : public testing::Test {
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

template <typename CopyOperationType, typename DstTensorType, typename SrcTensorType>
void copy_tensor(const DstTensorType& dst, const SrcTensorType& src)
{
    using namespace AscendC::Te;
    Copy(MakeCopy(CopyOperationType{}), dst, src);
}

template <
    typename CopyOperationType, typename SrcLocationType, typename DstLocationType, typename SrcLayoutType,
    typename DstLayoutType>
void expect_copy_trap(const SrcLayoutType& src_layout, const DstLayoutType& dst_layout)
{
    alignas(512) half src_data[1] = {};
    alignas(512) half dst_data[1] = {};
    auto src = make_tensor_at<SrcLocationType>(src_data, src_layout);
    auto dst = make_tensor_at<DstLocationType>(dst_data, dst_layout);

    EXPECT_THROW((copy_tensor<CopyOperationType>(dst, src)), TrapException);
}

template <typename CopyOperationType, typename SrcLocationType, typename DstLocationType>
void expect_operand_layout_traps()
{
    using namespace AscendC::Te;

    alignas(512) half src_data[1] = {};
    alignas(512) half dst_data[1] = {};
    auto valid_layout = MakeFrameLayout<NDExtLayoutPtn, half>(VALID_ROWS, VALID_COLUMNS);
    auto zero_shape_layout = MakeFrameLayout<NDExtLayoutPtn, half>(VALID_ROWS, 0);
    using LayoutTraitType = GetLayoutTrait<decltype(valid_layout)>;
    auto negative_shape_frame = MakeFrameLayout<NDExtLayoutPtn, half>(VALID_ROWS, -1);
    auto negative_shape_layout =
        MakePatternLayout<NDExtLayoutPtn, LayoutTraitType>(negative_shape_frame.Shape(), valid_layout.Stride());
    auto negative_stride = MakeStride(MakeStride(_0{}, -1), MakeStride(_0{}, _1{}));
    auto negative_stride_layout =
        MakePatternLayout<NDExtLayoutPtn, LayoutTraitType>(valid_layout.Shape(), negative_stride);

    auto valid_src = make_tensor_at<SrcLocationType>(src_data, valid_layout);
    auto valid_dst = make_tensor_at<DstLocationType>(dst_data, valid_layout);
    auto zero_shape_src = make_tensor_directly<SrcLocationType>(src_data, zero_shape_layout);
    auto negative_shape_src = make_tensor_directly<SrcLocationType>(src_data, negative_shape_layout);
    auto negative_stride_src = make_tensor_directly<SrcLocationType>(src_data, negative_stride_layout);
    auto zero_shape_dst = make_tensor_directly<DstLocationType>(dst_data, zero_shape_layout);
    auto negative_shape_dst = make_tensor_directly<DstLocationType>(dst_data, negative_shape_layout);
    auto negative_stride_dst = make_tensor_directly<DstLocationType>(dst_data, negative_stride_layout);

    EXPECT_THROW((copy_tensor<CopyOperationType>(valid_dst, zero_shape_src)), TrapException);
    EXPECT_THROW((copy_tensor<CopyOperationType>(valid_dst, negative_shape_src)), TrapException);
    EXPECT_THROW((copy_tensor<CopyOperationType>(valid_dst, negative_stride_src)), TrapException);
    EXPECT_THROW((copy_tensor<CopyOperationType>(zero_shape_dst, valid_src)), TrapException);
    EXPECT_THROW((copy_tensor<CopyOperationType>(negative_shape_dst, valid_src)), TrapException);
    EXPECT_THROW((copy_tensor<CopyOperationType>(negative_stride_dst, valid_src)), TrapException);
}

template <typename LayoutPattern>
auto make_block_len_boundary_layout()
{
    using namespace AscendC::Te;

    if constexpr (AscendC::Std::is_same_v<LayoutPattern, NDExtLayoutPtn>) {
        return MakeFrameLayout<NDExtLayoutPtn, half>(1, HALF_ELEMENTS_FOR_BLOCK_LEN_LIMIT_PLUS_ONE);
    } else if constexpr (AscendC::Std::is_same_v<LayoutPattern, DNExtLayoutPtn>) {
        return MakeFrameLayout<DNExtLayoutPtn, half>(HALF_ELEMENTS_FOR_BLOCK_LEN_LIMIT_PLUS_ONE, 1);
    } else if constexpr (AscendC::Std::is_same_v<LayoutPattern, NZLayoutPtn>) {
        return MakeFrameLayout<NZLayoutPtn, half>(FRACTAL_ELEMENTS_FOR_BLOCK_LEN_LIMIT_PLUS_ONE, 16);
    } else {
        static_assert(AscendC::Std::is_same_v<LayoutPattern, ZNLayoutPtn>);
        return MakeFrameLayout<ZNLayoutPtn, half>(16, FRACTAL_ELEMENTS_FOR_BLOCK_LEN_LIMIT_PLUS_ONE);
    }
}

template <typename LayoutPattern>
auto make_stride_test_layout()
{
    using namespace AscendC::Te;

    if constexpr (AscendC::Std::is_same_v<LayoutPattern, NDExtLayoutPtn>) {
        return MakeFrameLayout<NDExtLayoutPtn, half>(2, VALID_COLUMNS);
    } else if constexpr (AscendC::Std::is_same_v<LayoutPattern, DNExtLayoutPtn>) {
        return MakeFrameLayout<DNExtLayoutPtn, half>(VALID_ROWS, 2);
    } else if constexpr (AscendC::Std::is_same_v<LayoutPattern, NZLayoutPtn>) {
        return MakeFrameLayout<NZLayoutPtn, half>(VALID_ROWS, 32);
    } else {
        static_assert(AscendC::Std::is_same_v<LayoutPattern, ZNLayoutPtn>);
        return MakeFrameLayout<ZNLayoutPtn, half>(32, VALID_COLUMNS);
    }
}

template <typename LayoutPattern>
auto make_stride_test_layout(int selected_stride)
{
    using namespace AscendC::Te;

    auto valid_layout = make_stride_test_layout<LayoutPattern>();
    using LayoutTraitType = GetLayoutTrait<decltype(valid_layout)>;
    if constexpr (AscendC::Std::is_same_v<LayoutPattern, NDExtLayoutPtn>) {
        auto stride = MakeStride(MakeStride(_0{}, selected_stride), MakeStride(_0{}, _1{}));
        return MakePatternLayout<NDExtLayoutPtn, LayoutTraitType>(valid_layout.Shape(), stride);
    } else if constexpr (AscendC::Std::is_same_v<LayoutPattern, DNExtLayoutPtn>) {
        auto stride = MakeStride(MakeStride(_0{}, _1{}), MakeStride(_0{}, selected_stride));
        return MakePatternLayout<DNExtLayoutPtn, LayoutTraitType>(valid_layout.Shape(), stride);
    } else if constexpr (AscendC::Std::is_same_v<LayoutPattern, NZLayoutPtn>) {
        auto stride = MakeStride(Get<0>(valid_layout.Stride()), MakeStride(_1{}, selected_stride));
        return MakePatternLayout<NZLayoutPtn, LayoutTraitType>(valid_layout.Shape(), stride);
    } else {
        static_assert(AscendC::Std::is_same_v<LayoutPattern, ZNLayoutPtn>);
        auto stride = MakeStride(MakeStride(_1{}, selected_stride), Get<1>(valid_layout.Stride()));
        return MakePatternLayout<ZNLayoutPtn, LayoutTraitType>(valid_layout.Shape(), stride);
    }
}

TEST_F(TensorApiVectorCopyAbnormalValidation, AllPublicFormatRoutesRejectSmallerDestination)
{
    using namespace AscendC::Te;

    expect_copy_trap<CopyGM2UB, Location::GM, Location::UB>(
        MakeFrameLayout<NDExtLayoutPtn, half>(LARGE_ROWS, ROUTE_COLUMNS),
        MakeFrameLayout<NDExtLayoutPtn, half>(SMALL_ROWS, ROUTE_COLUMNS));
    expect_copy_trap<CopyGM2UB, Location::GM, Location::UB>(
        MakeFrameLayout<DNExtLayoutPtn, half>(LARGE_ROWS, ROUTE_COLUMNS),
        MakeFrameLayout<DNExtLayoutPtn, half>(SMALL_ROWS, ROUTE_COLUMNS));
    expect_copy_trap<CopyGM2UB, Location::GM, Location::UB>(
        MakeFrameLayout<NZLayoutPtn, half>(LARGE_ROWS, ROUTE_COLUMNS),
        MakeFrameLayout<NZLayoutPtn, half>(SMALL_ROWS, ROUTE_COLUMNS));
    expect_copy_trap<CopyGM2UB, Location::GM, Location::UB>(
        MakeFrameLayout<ZNLayoutPtn, half>(LARGE_ROWS, ROUTE_COLUMNS),
        MakeFrameLayout<ZNLayoutPtn, half>(SMALL_ROWS, ROUTE_COLUMNS));

    expect_copy_trap<CopyUB2GM, Location::UB, Location::GM>(
        MakeFrameLayout<NDExtLayoutPtn, half>(LARGE_ROWS, ROUTE_COLUMNS),
        MakeFrameLayout<NDExtLayoutPtn, half>(SMALL_ROWS, ROUTE_COLUMNS));
    expect_copy_trap<CopyUB2GM, Location::UB, Location::GM>(
        MakeFrameLayout<DNExtLayoutPtn, half>(LARGE_ROWS, ROUTE_COLUMNS),
        MakeFrameLayout<DNExtLayoutPtn, half>(SMALL_ROWS, ROUTE_COLUMNS));
    expect_copy_trap<CopyUB2GM, Location::UB, Location::GM>(
        MakeFrameLayout<NZLayoutPtn, half>(LARGE_ROWS, ROUTE_COLUMNS),
        MakeFrameLayout<NZLayoutPtn, half>(SMALL_ROWS, ROUTE_COLUMNS));

    expect_copy_trap<CopyUB2L1, Location::UB, Location::L1>(
        MakeFrameLayout<NDExtLayoutPtn, half>(LARGE_ROWS, ROUTE_COLUMNS),
        MakeFrameLayout<NDExtLayoutPtn, half>(SMALL_ROWS, ROUTE_COLUMNS));
    expect_copy_trap<CopyUB2L1, Location::UB, Location::L1>(
        MakeFrameLayout<DNExtLayoutPtn, half>(LARGE_ROWS, ROUTE_COLUMNS),
        MakeFrameLayout<DNExtLayoutPtn, half>(SMALL_ROWS, ROUTE_COLUMNS));
    expect_copy_trap<CopyUB2L1, Location::UB, Location::L1>(
        MakeFrameLayout<NZLayoutPtn, half>(LARGE_ROWS, ROUTE_COLUMNS),
        MakeFrameLayout<NZLayoutPtn, half>(SMALL_ROWS, ROUTE_COLUMNS));
    expect_copy_trap<CopyUB2L1, Location::UB, Location::L1>(
        MakeFrameLayout<ZNLayoutPtn, half>(LARGE_ROWS, ROUTE_COLUMNS),
        MakeFrameLayout<ZNLayoutPtn, half>(SMALL_ROWS, ROUTE_COLUMNS));

    expect_copy_trap<CopyUB2UB, Location::UB, Location::UB>(
        MakeFrameLayout<NDExtLayoutPtn, half>(LARGE_ROWS, ROUTE_COLUMNS),
        MakeFrameLayout<NDExtLayoutPtn, half>(SMALL_ROWS, ROUTE_COLUMNS));
    expect_copy_trap<CopyUB2UB, Location::UB, Location::UB>(
        MakeFrameLayout<DNExtLayoutPtn, half>(LARGE_ROWS, ROUTE_COLUMNS),
        MakeFrameLayout<DNExtLayoutPtn, half>(SMALL_ROWS, ROUTE_COLUMNS));
    expect_copy_trap<CopyUB2UB, Location::UB, Location::UB>(
        MakeFrameLayout<NZLayoutPtn, half>(LARGE_ROWS, ROUTE_COLUMNS),
        MakeFrameLayout<NZLayoutPtn, half>(SMALL_ROWS, ROUTE_COLUMNS));
    expect_copy_trap<CopyUB2UB, Location::UB, Location::UB>(
        MakeFrameLayout<ZNLayoutPtn, half>(LARGE_ROWS, ROUTE_COLUMNS),
        MakeFrameLayout<ZNLayoutPtn, half>(SMALL_ROWS, ROUTE_COLUMNS));
}

TEST_F(TensorApiVectorCopyAbnormalValidation, AllPublicFormatRoutesRejectBlockCountAboveLimit)
{
    using namespace AscendC::Te;

    auto nd_layout = MakeFrameLayout<NDExtLayoutPtn, half>(BLOCK_COUNT_LIMIT_PLUS_ONE, VALID_COLUMNS);
    auto dn_layout = MakeFrameLayout<DNExtLayoutPtn, half>(VALID_ROWS, BLOCK_COUNT_LIMIT_PLUS_ONE);
    auto nz_layout = MakeFrameLayout<NZLayoutPtn, half>(VALID_ROWS, FRACTAL_BLOCK_COUNT_LIMIT_PLUS_ONE);
    auto zn_layout = MakeFrameLayout<ZNLayoutPtn, half>(FRACTAL_BLOCK_COUNT_LIMIT_PLUS_ONE, VALID_COLUMNS);

    expect_copy_trap<CopyGM2UB, Location::GM, Location::UB>(nd_layout, nd_layout);
    expect_copy_trap<CopyGM2UB, Location::GM, Location::UB>(dn_layout, dn_layout);
    expect_copy_trap<CopyGM2UB, Location::GM, Location::UB>(nz_layout, nz_layout);
    expect_copy_trap<CopyGM2UB, Location::GM, Location::UB>(zn_layout, zn_layout);

    expect_copy_trap<CopyUB2GM, Location::UB, Location::GM>(nd_layout, nd_layout);
    expect_copy_trap<CopyUB2GM, Location::UB, Location::GM>(dn_layout, dn_layout);
    expect_copy_trap<CopyUB2GM, Location::UB, Location::GM>(nz_layout, nz_layout);

    expect_copy_trap<CopyUB2L1, Location::UB, Location::L1>(nd_layout, nd_layout);
    expect_copy_trap<CopyUB2L1, Location::UB, Location::L1>(dn_layout, dn_layout);
    expect_copy_trap<CopyUB2L1, Location::UB, Location::L1>(nz_layout, nz_layout);
    expect_copy_trap<CopyUB2L1, Location::UB, Location::L1>(zn_layout, zn_layout);

    expect_copy_trap<CopyUB2UB, Location::UB, Location::UB>(nd_layout, nd_layout);
    expect_copy_trap<CopyUB2UB, Location::UB, Location::UB>(dn_layout, dn_layout);
    expect_copy_trap<CopyUB2UB, Location::UB, Location::UB>(nz_layout, nz_layout);
    expect_copy_trap<CopyUB2UB, Location::UB, Location::UB>(zn_layout, zn_layout);
}

TEST_F(TensorApiVectorCopyAbnormalValidation, CopyGM2UBRejectsAllInvalidSrcAndDstLayouts)
{
    expect_operand_layout_traps<AscendC::Te::CopyGM2UB, AscendC::Te::Location::GM, AscendC::Te::Location::UB>();
}

TEST_F(TensorApiVectorCopyAbnormalValidation, CopyUB2GMRejectsAllInvalidSrcAndDstLayouts)
{
    expect_operand_layout_traps<AscendC::Te::CopyUB2GM, AscendC::Te::Location::UB, AscendC::Te::Location::GM>();
}

TEST_F(TensorApiVectorCopyAbnormalValidation, CopyUB2L1RejectsAllInvalidSrcAndDstLayouts)
{
    expect_operand_layout_traps<AscendC::Te::CopyUB2L1, AscendC::Te::Location::UB, AscendC::Te::Location::L1>();
}

TEST_F(TensorApiVectorCopyAbnormalValidation, CopyUB2UBRejectsAllInvalidSrcAndDstLayouts)
{
    expect_operand_layout_traps<AscendC::Te::CopyUB2UB, AscendC::Te::Location::UB, AscendC::Te::Location::UB>();
}

TEST_F(TensorApiVectorCopyAbnormalValidation, EveryPublicFormatRouteRejectsItsBlockLenUpperBoundary)
{
    using namespace AscendC::Te;

    auto nd_layout = make_block_len_boundary_layout<NDExtLayoutPtn>();
    auto dn_layout = make_block_len_boundary_layout<DNExtLayoutPtn>();
    auto nz_layout = make_block_len_boundary_layout<NZLayoutPtn>();
    auto zn_layout = make_block_len_boundary_layout<ZNLayoutPtn>();

    expect_copy_trap<CopyGM2UB, Location::GM, Location::UB>(nd_layout, nd_layout);
    expect_copy_trap<CopyGM2UB, Location::GM, Location::UB>(dn_layout, dn_layout);
    expect_copy_trap<CopyGM2UB, Location::GM, Location::UB>(nz_layout, nz_layout);
    expect_copy_trap<CopyGM2UB, Location::GM, Location::UB>(zn_layout, zn_layout);

    expect_copy_trap<CopyUB2GM, Location::UB, Location::GM>(nd_layout, nd_layout);
    expect_copy_trap<CopyUB2GM, Location::UB, Location::GM>(dn_layout, dn_layout);
    expect_copy_trap<CopyUB2GM, Location::UB, Location::GM>(nz_layout, nz_layout);

    expect_copy_trap<CopyUB2L1, Location::UB, Location::L1>(nd_layout, nd_layout);
    expect_copy_trap<CopyUB2L1, Location::UB, Location::L1>(dn_layout, dn_layout);
    expect_copy_trap<CopyUB2L1, Location::UB, Location::L1>(nz_layout, nz_layout);
    expect_copy_trap<CopyUB2L1, Location::UB, Location::L1>(zn_layout, zn_layout);

    expect_copy_trap<CopyUB2UB, Location::UB, Location::UB>(nd_layout, nd_layout);
    expect_copy_trap<CopyUB2UB, Location::UB, Location::UB>(dn_layout, dn_layout);
    expect_copy_trap<CopyUB2UB, Location::UB, Location::UB>(nz_layout, nz_layout);
    expect_copy_trap<CopyUB2UB, Location::UB, Location::UB>(zn_layout, zn_layout);
}

TEST_F(TensorApiVectorCopyAbnormalValidation, CopyUB2UBRejectsPublicBlockLenBelowOneDataBlock)
{
    using namespace AscendC::Te;

    auto layout = MakeFrameLayout<NDExtLayoutPtn, half>(1, 1);
    expect_copy_trap<CopyUB2UB, Location::UB, Location::UB>(layout, layout);
}

TEST_F(TensorApiVectorCopyAbnormalValidation, DirectionalStrideRestrictionsCoverEveryPublicFormatRoute)
{
    using namespace AscendC::Te;

    auto nd_layout = make_stride_test_layout<NDExtLayoutPtn>();
    auto dn_layout = make_stride_test_layout<DNExtLayoutPtn>();
    auto nz_layout = make_stride_test_layout<NZLayoutPtn>();
    auto zn_layout = make_stride_test_layout<ZNLayoutPtn>();
    auto nd_zero_stride = make_stride_test_layout<NDExtLayoutPtn>(0);
    auto dn_zero_stride = make_stride_test_layout<DNExtLayoutPtn>(0);
    auto nz_zero_stride = make_stride_test_layout<NZLayoutPtn>(0);
    auto zn_zero_stride = make_stride_test_layout<ZNLayoutPtn>(0);
    auto nd_misaligned_stride = make_stride_test_layout<NDExtLayoutPtn>(MISALIGNED_STRIDE_ELEMENTS);
    auto dn_misaligned_stride = make_stride_test_layout<DNExtLayoutPtn>(MISALIGNED_STRIDE_ELEMENTS);
    auto nz_misaligned_stride = make_stride_test_layout<NZLayoutPtn>(MISALIGNED_STRIDE_ELEMENTS);
    auto zn_misaligned_stride = make_stride_test_layout<ZNLayoutPtn>(MISALIGNED_STRIDE_ELEMENTS);

    expect_copy_trap<CopyGM2UB, Location::GM, Location::UB>(nd_layout, nd_zero_stride);
    expect_copy_trap<CopyGM2UB, Location::GM, Location::UB>(dn_layout, dn_zero_stride);
    expect_copy_trap<CopyGM2UB, Location::GM, Location::UB>(nz_layout, nz_zero_stride);
    expect_copy_trap<CopyGM2UB, Location::GM, Location::UB>(zn_layout, zn_zero_stride);
    expect_copy_trap<CopyGM2UB, Location::GM, Location::UB>(nd_layout, nd_misaligned_stride);
    expect_copy_trap<CopyGM2UB, Location::GM, Location::UB>(dn_layout, dn_misaligned_stride);
    expect_copy_trap<CopyGM2UB, Location::GM, Location::UB>(nz_layout, nz_misaligned_stride);
    expect_copy_trap<CopyGM2UB, Location::GM, Location::UB>(zn_layout, zn_misaligned_stride);

    expect_copy_trap<CopyUB2GM, Location::UB, Location::GM>(nd_zero_stride, nd_layout);
    expect_copy_trap<CopyUB2GM, Location::UB, Location::GM>(dn_zero_stride, dn_layout);
    expect_copy_trap<CopyUB2GM, Location::UB, Location::GM>(nz_zero_stride, nz_layout);
    expect_copy_trap<CopyUB2GM, Location::UB, Location::GM>(nd_misaligned_stride, nd_layout);
    expect_copy_trap<CopyUB2GM, Location::UB, Location::GM>(dn_misaligned_stride, dn_layout);
    expect_copy_trap<CopyUB2GM, Location::UB, Location::GM>(nz_misaligned_stride, nz_layout);
}

} // namespace
