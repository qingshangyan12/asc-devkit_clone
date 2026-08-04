/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <cstddef>
#include <cstdint>
#include <vector>

#include <gtest/gtest.h>

#include "tensor_api_runtime_validation_include.h"

namespace {

using tensor_api_runtime_validation::TrapException;

constexpr std::size_t BACKING_BUFFER_ALIGNMENT = 512;

template <typename ElementType>
class aligned_backing_buffer {
public:
    explicit aligned_backing_buffer(std::size_t element_count)
        : storage_(element_count + BACKING_BUFFER_ALIGNMENT / sizeof(ElementType))
    {}

    ElementType* data()
    {
        auto address = reinterpret_cast<std::uintptr_t>(storage_.data());
        auto aligned_address =
            (address + BACKING_BUFFER_ALIGNMENT - 1) & ~(static_cast<std::uintptr_t>(BACKING_BUFFER_ALIGNMENT) - 1);
        return reinterpret_cast<ElementType*>(aligned_address);
    }

private:
    std::vector<ElementType> storage_;
};

template <typename LayoutType>
std::size_t backing_buffer_elements(const LayoutType& layout)
{
    auto size = static_cast<std::size_t>(layout.Size());
    auto capacity = static_cast<std::size_t>(layout.Capacity());
    return size > capacity ? size : capacity;
}

class TensorApiCubeInputAdditionalValidation : public testing::Test {
protected:
    void SetUp() override { AscendC::SetGCoreType(1); }

    void TearDown() override { AscendC::SetGCoreType(0); }
};

template <typename LocationType, typename PointerType, typename LayoutType>
auto make_tensor_at(PointerType pointer, const LayoutType& layout)
{
    return AscendC::Te::MakeTensor(AscendC::Te::MakeMemPtr<LocationType>(pointer), layout);
}

template <typename CopyOperationType, typename CopyTraitType, typename DstTensorType, typename SrcTensorType>
void copy_tensor(const DstTensorType& dst, const SrcTensorType& src)
{
    using namespace AscendC::Te;
    Copy(MakeCopy(CopyOperationType{}, CopyTraitType{}), dst, src);
}

template <typename CopyOperationType, typename DstTensorType, typename SrcTensorType, typename ParamType>
void copy_tensor_with(const DstTensorType& dst, const SrcTensorType& src, const ParamType& param)
{
    using namespace AscendC::Te;
    Copy(MakeCopy(CopyOperationType{}).with(param), dst, src);
}

template <typename ElementType, typename SrcLayoutType, typename DstLayoutType>
void expect_gm2l1_trap(const SrcLayoutType& src_layout, const DstLayoutType& dst_layout)
{
    using namespace AscendC::Te;
    aligned_backing_buffer<ElementType> src(backing_buffer_elements(src_layout));
    aligned_backing_buffer<ElementType> dst(backing_buffer_elements(dst_layout));
    auto src_tensor = make_tensor_at<Location::GM>(src.data(), src_layout);
    auto dst_tensor = make_tensor_at<Location::L1>(dst.data(), dst_layout);

    EXPECT_THROW((copy_tensor<CopyGM2L1, CopyGM2L1TraitDefault>(dst_tensor, src_tensor)), TrapException);
}

template <typename SrcType, typename DstType, typename SrcLayoutType, typename DstLayoutType>
void expect_l12bt_trap(const SrcLayoutType& src_layout, const DstLayoutType& dst_layout)
{
    using namespace AscendC::Te;
    aligned_backing_buffer<SrcType> src(backing_buffer_elements(src_layout));
    aligned_backing_buffer<DstType> dst(backing_buffer_elements(dst_layout));
    auto src_tensor = make_tensor_at<Location::L1>(src.data(), src_layout);
    auto dst_tensor = make_tensor_at<Location::BIAS>(dst.data(), dst_layout);

    EXPECT_THROW((copy_tensor<CopyL12BT, CopyL12BTTraitDefault>(dst_tensor, src_tensor)), TrapException);
}

template <typename ElementType, typename SrcLayoutType, typename DstLayoutType>
void expect_l12ub_trap(const SrcLayoutType& src_layout, const DstLayoutType& dst_layout)
{
    using namespace AscendC::Te;
    aligned_backing_buffer<ElementType> src(backing_buffer_elements(src_layout));
    aligned_backing_buffer<ElementType> dst(backing_buffer_elements(dst_layout));
    auto src_tensor = make_tensor_at<Location::L1>(src.data(), src_layout);
    auto dst_tensor = make_tensor_at<Location::UB>(dst.data(), dst_layout);

    EXPECT_THROW((copy_tensor<CopyL12UB, CopyL12UBTraitDefault>(dst_tensor, src_tensor)), TrapException);
}

template <typename SrcLayoutType, typename DstLayoutType>
void expect_l12fb_trap(const SrcLayoutType& src_layout, const DstLayoutType& dst_layout)
{
    using namespace AscendC::Te;
    aligned_backing_buffer<uint64_t> src(backing_buffer_elements(src_layout));
    aligned_backing_buffer<uint64_t> dst(backing_buffer_elements(dst_layout));
    auto src_tensor = make_tensor_at<Location::L1>(src.data(), src_layout);
    auto dst_tensor = make_tensor_at<Location::FIXBUF>(dst.data(), dst_layout);

    EXPECT_THROW((copy_tensor<CopyL12FB, CopyL12FBTraitDefault>(dst_tensor, src_tensor)), TrapException);
}

template <typename ElementType, typename SrcLayoutType, typename DstLayoutType>
void expect_l12l0a_trap(const SrcLayoutType& src_layout, const DstLayoutType& dst_layout)
{
    using namespace AscendC::Te;
    aligned_backing_buffer<ElementType> src(backing_buffer_elements(src_layout));
    aligned_backing_buffer<ElementType> dst(backing_buffer_elements(dst_layout));
    auto src_tensor = make_tensor_at<Location::L1>(src.data(), src_layout);
    auto dst_tensor = make_tensor_at<Location::L0A>(dst.data(), dst_layout);

    EXPECT_THROW((copy_tensor<CopyL12L0A, CopyL12L0ATraitDefault>(dst_tensor, src_tensor)), TrapException);
}

template <typename ElementType, typename SrcLayoutType, typename DstLayoutType>
void expect_l12l0b_trap(const SrcLayoutType& src_layout, const DstLayoutType& dst_layout)
{
    using namespace AscendC::Te;
    aligned_backing_buffer<ElementType> src(backing_buffer_elements(src_layout));
    aligned_backing_buffer<ElementType> dst(backing_buffer_elements(dst_layout));
    auto src_tensor = make_tensor_at<Location::L1>(src.data(), src_layout);
    auto dst_tensor = make_tensor_at<Location::L0B>(dst.data(), dst_layout);

    EXPECT_THROW((copy_tensor<CopyL12L0B, CopyL12L0BTraitDefault>(dst_tensor, src_tensor)), TrapException);
}

template <typename ElementType, typename SrcLayoutType, typename DstLayoutType>
void expect_gm2l1_size_trap(const SrcLayoutType& src_layout, const DstLayoutType& dst_layout)
{
    ASSERT_LT(static_cast<std::size_t>(dst_layout.Size()), static_cast<std::size_t>(src_layout.Size()));
    expect_gm2l1_trap<ElementType>(src_layout, dst_layout);
}

template <typename SrcType, typename DstType, typename SrcLayoutType, typename DstLayoutType>
void expect_l12bt_size_trap(const SrcLayoutType& src_layout, const DstLayoutType& dst_layout)
{
    ASSERT_LT(static_cast<std::size_t>(dst_layout.Size()), static_cast<std::size_t>(src_layout.Size()));
    expect_l12bt_trap<SrcType, DstType>(src_layout, dst_layout);
}

template <typename ElementType, typename SrcLayoutType, typename DstLayoutType>
void expect_l12ub_size_trap(const SrcLayoutType& src_layout, const DstLayoutType& dst_layout)
{
    ASSERT_LT(static_cast<std::size_t>(dst_layout.Size()), static_cast<std::size_t>(src_layout.Size()));
    expect_l12ub_trap<ElementType>(src_layout, dst_layout);
}

template <typename SrcLayoutType, typename DstLayoutType>
void expect_l12fb_size_trap(const SrcLayoutType& src_layout, const DstLayoutType& dst_layout)
{
    ASSERT_LT(static_cast<std::size_t>(dst_layout.Size()), static_cast<std::size_t>(src_layout.Size()));
    expect_l12fb_trap(src_layout, dst_layout);
}

template <typename ElementType, typename SrcLayoutType, typename DstLayoutType>
void expect_l12l0a_size_trap(const SrcLayoutType& src_layout, const DstLayoutType& dst_layout)
{
    ASSERT_LT(static_cast<std::size_t>(dst_layout.Size()), static_cast<std::size_t>(src_layout.Size()));
    expect_l12l0a_trap<ElementType>(src_layout, dst_layout);
}

template <typename ElementType, typename SrcLayoutType, typename DstLayoutType>
void expect_l12l0b_size_trap(const SrcLayoutType& src_layout, const DstLayoutType& dst_layout)
{
    ASSERT_LT(static_cast<std::size_t>(dst_layout.Size()), static_cast<std::size_t>(src_layout.Size()));
    expect_l12l0b_trap<ElementType>(src_layout, dst_layout);
}

template <typename LayoutPattern, typename LayoutType, typename BatchType, typename StrideType>
auto make_batch_layout(const LayoutType& layout, const BatchType& batch, const StrideType& batch_stride)
{
    using namespace AscendC::Te;
    using LayoutTraitType = GetLayoutTrait<LayoutType>;
    return MakePatternLayout<LayoutPattern, LayoutTraitType>(
        MakeShape(batch, layout.Shape()), MakeStride(batch_stride, layout.Stride()));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyGM2L1RejectsBatchAboveLimitForAllRemainingFormats)
{
    using namespace AscendC::Te;
    int batch = 4096;
    int rows = 64;
    int columns = 32;

    expect_gm2l1_trap<half>(
        MakeFrameLayout<NDExtLayoutPtn, half>(batch, rows, columns),
        MakeFrameLayout<NZLayoutPtn, half>(batch, rows, columns));
    expect_gm2l1_trap<half>(
        MakeFrameLayout<DNExtLayoutPtn, half>(batch, rows, columns),
        MakeFrameLayout<NZLayoutPtn, half>(batch, rows, columns));
    expect_gm2l1_trap<half>(
        MakeFrameLayout<NDExtLayoutPtn, half>(batch, rows, columns),
        MakeFrameLayout<ZNLayoutPtn, half>(batch, rows, columns));
    expect_gm2l1_trap<half>(
        MakeFrameLayout<DNExtLayoutPtn, half>(batch, rows, columns),
        MakeFrameLayout<ZNLayoutPtn, half>(batch, rows, columns));

    expect_gm2l1_trap<fp8_e8m0_t>(
        MakeFrameLayout<ScaleANDLayoutPtn, AscendC::Std::Int<2>>(batch, rows, columns),
        MakeFrameLayout<ZZLayoutPtn, AscendC::Std::Int<2>>(batch, rows, columns));
    expect_gm2l1_trap<fp8_e8m0_t>(
        MakeFrameLayout<ScaleADNLayoutPtn, AscendC::Std::Int<2>>(batch, rows, columns),
        MakeFrameLayout<ZZLayoutPtn, AscendC::Std::Int<2>>(batch, rows, columns));
    expect_gm2l1_trap<fp8_e8m0_t>(
        MakeFrameLayout<ZZLayoutPtn, AscendC::Std::Int<2>>(batch, rows, columns),
        MakeFrameLayout<ZZLayoutPtn, AscendC::Std::Int<2>>(batch, rows, columns));
    expect_gm2l1_trap<fp8_e8m0_t>(
        MakeFrameLayout<ScaleBNDLayoutPtn, AscendC::Std::Int<2>>(batch, rows, columns),
        MakeFrameLayout<NNLayoutPtn, AscendC::Std::Int<2>>(batch, rows, columns));
    expect_gm2l1_trap<fp8_e8m0_t>(
        MakeFrameLayout<ScaleBDNLayoutPtn, AscendC::Std::Int<2>>(batch, rows, columns),
        MakeFrameLayout<NNLayoutPtn, AscendC::Std::Int<2>>(batch, rows, columns));
    expect_gm2l1_trap<fp8_e8m0_t>(
        MakeFrameLayout<NNLayoutPtn, AscendC::Std::Int<2>>(batch, rows, columns),
        MakeFrameLayout<NNLayoutPtn, AscendC::Std::Int<2>>(batch, rows, columns));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyGM2L1RejectsSmallerDestinationForRemainingPublicFormats)
{
    using namespace AscendC::Te;
    constexpr int SRC_ROWS = 64;
    constexpr int DST_ROWS = 16;
    constexpr int COLUMNS = 32;

    expect_gm2l1_size_trap<half>(
        MakeFrameLayout<NDExtLayoutPtn, half>(SRC_ROWS, COLUMNS),
        MakeFrameLayout<NZLayoutPtn, half>(DST_ROWS, COLUMNS));
    expect_gm2l1_size_trap<half>(
        MakeFrameLayout<NDExtLayoutPtn, half>(SRC_ROWS, COLUMNS),
        MakeFrameLayout<ZNLayoutPtn, half>(DST_ROWS, COLUMNS));
    expect_gm2l1_size_trap<half>(
        MakeFrameLayout<DNExtLayoutPtn, half>(SRC_ROWS, COLUMNS),
        MakeFrameLayout<NZLayoutPtn, half>(DST_ROWS, COLUMNS));
    expect_gm2l1_size_trap<half>(
        MakeFrameLayout<DNExtLayoutPtn, half>(SRC_ROWS, COLUMNS),
        MakeFrameLayout<ZNLayoutPtn, half>(DST_ROWS, COLUMNS));
    expect_gm2l1_size_trap<half>(
        MakeFrameLayout<NZLayoutPtn, half>(SRC_ROWS, COLUMNS), MakeFrameLayout<NZLayoutPtn, half>(DST_ROWS, COLUMNS));
    expect_gm2l1_size_trap<half>(
        MakeFrameLayout<ZNLayoutPtn, half>(SRC_ROWS, COLUMNS), MakeFrameLayout<ZNLayoutPtn, half>(DST_ROWS, COLUMNS));

    expect_gm2l1_size_trap<fp8_e8m0_t>(
        MakeFrameLayout<ScaleANDLayoutPtn, AscendC::Std::Int<2>>(SRC_ROWS, COLUMNS),
        MakeFrameLayout<ZZLayoutPtn, AscendC::Std::Int<2>>(DST_ROWS, COLUMNS));
    expect_gm2l1_size_trap<fp8_e8m0_t>(
        MakeFrameLayout<ScaleADNLayoutPtn, AscendC::Std::Int<2>>(SRC_ROWS, COLUMNS),
        MakeFrameLayout<ZZLayoutPtn, AscendC::Std::Int<2>>(DST_ROWS, COLUMNS));
    expect_gm2l1_size_trap<fp8_e8m0_t>(
        MakeFrameLayout<ZZLayoutPtn, AscendC::Std::Int<2>>(SRC_ROWS, COLUMNS),
        MakeFrameLayout<ZZLayoutPtn, AscendC::Std::Int<2>>(DST_ROWS, COLUMNS));
    expect_gm2l1_size_trap<fp8_e8m0_t>(
        MakeFrameLayout<ScaleBNDLayoutPtn, AscendC::Std::Int<2>>(SRC_ROWS, COLUMNS),
        MakeFrameLayout<NNLayoutPtn, AscendC::Std::Int<2>>(DST_ROWS, COLUMNS));
    expect_gm2l1_size_trap<fp8_e8m0_t>(
        MakeFrameLayout<ScaleBDNLayoutPtn, AscendC::Std::Int<2>>(SRC_ROWS, COLUMNS),
        MakeFrameLayout<NNLayoutPtn, AscendC::Std::Int<2>>(DST_ROWS, COLUMNS));
    expect_gm2l1_size_trap<fp8_e8m0_t>(
        MakeFrameLayout<NNLayoutPtn, AscendC::Std::Int<2>>(SRC_ROWS, COLUMNS),
        MakeFrameLayout<NNLayoutPtn, AscendC::Std::Int<2>>(DST_ROWS, COLUMNS));

    expect_gm2l1_size_trap<half>(
        MakeFrameLayout<NC1HWC0LayoutPtn>(1, 2, 4, 4, 16), MakeFrameLayout<NC1HWC0LayoutPtn>(1, 1, 2, 4, 16));
    expect_gm2l1_size_trap<half>(
        MakeFrameLayout<NHWCLayoutPtn>(1, 4, 4, 16), MakeFrameLayout<NC1HWC0LayoutPtn>(1, 1, 2, 4, 16));
    expect_gm2l1_size_trap<half>(
        MakeFrameLayout<NCHWLayoutPtn>(1, 16, 4, 4), MakeFrameLayout<NC1HWC0LayoutPtn>(1, 1, 2, 4, 16));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyGM2L1FlatNDAndDNRoutesRejectSmallerDestination)
{
    using namespace AscendC::Te;
    constexpr int SRC_ROWS = 64;
    constexpr int DST_ROWS = 16;
    constexpr int COLUMNS = 32;

    expect_gm2l1_size_trap<half>(
        MakeFrameLayout<NDLayoutPtn, half>(SRC_ROWS, COLUMNS), MakeFrameLayout<NDLayoutPtn, half>(DST_ROWS, COLUMNS));
    expect_gm2l1_size_trap<half>(
        MakeFrameLayout<NDLayoutPtn, half>(SRC_ROWS, COLUMNS), MakeFrameLayout<NZLayoutPtn, half>(DST_ROWS, COLUMNS));
    expect_gm2l1_size_trap<half>(
        MakeFrameLayout<NDLayoutPtn, half>(SRC_ROWS, COLUMNS), MakeFrameLayout<ZNLayoutPtn, half>(DST_ROWS, COLUMNS));
    expect_gm2l1_size_trap<half>(
        MakeFrameLayout<DNLayoutPtn, half>(SRC_ROWS, COLUMNS), MakeFrameLayout<NZLayoutPtn, half>(DST_ROWS, COLUMNS));
    expect_gm2l1_size_trap<half>(
        MakeFrameLayout<DNLayoutPtn, half>(SRC_ROWS, COLUMNS), MakeFrameLayout<ZNLayoutPtn, half>(DST_ROWS, COLUMNS));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyGM2L1BatchRoutesRejectSmallerDestination)
{
    using namespace AscendC::Te;
    constexpr int BATCH = 2;
    constexpr int SRC_ROWS = 64;
    constexpr int DST_ROWS = 16;
    constexpr int COLUMNS = 32;

    expect_gm2l1_size_trap<half>(
        MakeFrameLayout<NDExtLayoutPtn, half>(BATCH, SRC_ROWS, COLUMNS),
        MakeFrameLayout<NDExtLayoutPtn, half>(BATCH, DST_ROWS, COLUMNS));
    expect_gm2l1_size_trap<half>(
        MakeFrameLayout<NDExtLayoutPtn, half>(BATCH, SRC_ROWS, COLUMNS),
        MakeFrameLayout<NZLayoutPtn, half>(BATCH, DST_ROWS, COLUMNS));
    expect_gm2l1_size_trap<half>(
        MakeFrameLayout<DNExtLayoutPtn, half>(BATCH, SRC_ROWS, COLUMNS),
        MakeFrameLayout<NZLayoutPtn, half>(BATCH, DST_ROWS, COLUMNS));
    expect_gm2l1_size_trap<half>(
        MakeFrameLayout<NDExtLayoutPtn, half>(BATCH, SRC_ROWS, COLUMNS),
        MakeFrameLayout<ZNLayoutPtn, half>(BATCH, DST_ROWS, COLUMNS));
    expect_gm2l1_size_trap<half>(
        MakeFrameLayout<DNExtLayoutPtn, half>(BATCH, SRC_ROWS, COLUMNS),
        MakeFrameLayout<ZNLayoutPtn, half>(BATCH, DST_ROWS, COLUMNS));

    expect_gm2l1_size_trap<fp8_e8m0_t>(
        MakeFrameLayout<ScaleANDLayoutPtn, AscendC::Std::Int<2>>(BATCH, SRC_ROWS, COLUMNS),
        MakeFrameLayout<ZZLayoutPtn, AscendC::Std::Int<2>>(BATCH, DST_ROWS, COLUMNS));
    expect_gm2l1_size_trap<fp8_e8m0_t>(
        MakeFrameLayout<ScaleADNLayoutPtn, AscendC::Std::Int<2>>(BATCH, SRC_ROWS, COLUMNS),
        MakeFrameLayout<ZZLayoutPtn, AscendC::Std::Int<2>>(BATCH, DST_ROWS, COLUMNS));
    expect_gm2l1_size_trap<fp8_e8m0_t>(
        MakeFrameLayout<ZZLayoutPtn, AscendC::Std::Int<2>>(BATCH, SRC_ROWS, COLUMNS),
        MakeFrameLayout<ZZLayoutPtn, AscendC::Std::Int<2>>(BATCH, DST_ROWS, COLUMNS));
    expect_gm2l1_size_trap<fp8_e8m0_t>(
        MakeFrameLayout<ScaleBNDLayoutPtn, AscendC::Std::Int<2>>(BATCH, SRC_ROWS, COLUMNS),
        MakeFrameLayout<NNLayoutPtn, AscendC::Std::Int<2>>(BATCH, DST_ROWS, COLUMNS));
    expect_gm2l1_size_trap<fp8_e8m0_t>(
        MakeFrameLayout<ScaleBDNLayoutPtn, AscendC::Std::Int<2>>(BATCH, SRC_ROWS, COLUMNS),
        MakeFrameLayout<NNLayoutPtn, AscendC::Std::Int<2>>(BATCH, DST_ROWS, COLUMNS));
    expect_gm2l1_size_trap<fp8_e8m0_t>(
        MakeFrameLayout<NNLayoutPtn, AscendC::Std::Int<2>>(BATCH, SRC_ROWS, COLUMNS),
        MakeFrameLayout<NNLayoutPtn, AscendC::Std::Int<2>>(BATCH, DST_ROWS, COLUMNS));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12UBRejectsSmallerDestinationForEveryPublicFormat)
{
    using namespace AscendC::Te;
    constexpr int SRC_ROWS = 64;
    constexpr int DST_ROWS = 16;
    constexpr int COLUMNS = 32;

    expect_l12ub_size_trap<half>(
        MakeFrameLayout<NDLayoutPtn, half>(SRC_ROWS, COLUMNS), MakeFrameLayout<NDLayoutPtn, half>(DST_ROWS, COLUMNS));
    expect_l12ub_size_trap<half>(
        MakeFrameLayout<NDExtLayoutPtn, half>(SRC_ROWS, COLUMNS),
        MakeFrameLayout<NDExtLayoutPtn, half>(DST_ROWS, COLUMNS));
    expect_l12ub_size_trap<half>(
        MakeFrameLayout<DNLayoutPtn, half>(SRC_ROWS, COLUMNS), MakeFrameLayout<DNLayoutPtn, half>(DST_ROWS, COLUMNS));
    expect_l12ub_size_trap<half>(
        MakeFrameLayout<DNExtLayoutPtn, half>(SRC_ROWS, COLUMNS),
        MakeFrameLayout<DNExtLayoutPtn, half>(DST_ROWS, COLUMNS));
    expect_l12ub_size_trap<half>(
        MakeFrameLayout<NZLayoutPtn, half>(SRC_ROWS, COLUMNS), MakeFrameLayout<NZLayoutPtn, half>(DST_ROWS, COLUMNS));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12BTRejectsInvalidBatchInputs)
{
    using namespace AscendC::Te;
    int columns = 16;
    expect_l12bt_trap<half, float>(
        MakeFrameLayout<NDExtLayoutPtn>(1, 1, columns), MakeFrameLayout<NDExtLayoutPtn>(2, 1, columns));
    expect_l12bt_trap<half, float>(
        MakeFrameLayout<NDExtLayoutPtn>(4096, 1, columns), MakeFrameLayout<NDExtLayoutPtn>(4096, 1, columns));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12BTRejectsEachUnalignedBatchColumn)
{
    using namespace AscendC::Te;
    int batch = 2;
    expect_l12bt_trap<half, float>(
        MakeFrameLayout<NDExtLayoutPtn>(batch, 1, 17), MakeFrameLayout<NDExtLayoutPtn>(batch, 1, 32));
    expect_l12bt_trap<half, float>(
        MakeFrameLayout<NDExtLayoutPtn>(batch, 1, 16), MakeFrameLayout<NDExtLayoutPtn>(batch, 1, 17));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12BTRejectsBlockCountAboveLimitForEachNDLayout)
{
    using namespace AscendC::Te;
    int rows = 4096;
    int columns = 16;
    expect_l12bt_trap<float, float>(
        MakeFrameLayout<NDLayoutPtn>(rows, columns), MakeFrameLayout<NDLayoutPtn>(rows, columns));
    expect_l12bt_trap<float, float>(
        MakeFrameLayout<NDExtLayoutPtn>(rows, columns), MakeFrameLayout<NDExtLayoutPtn>(rows, columns));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12BTRejectsSmallerDestinationForNDLayout)
{
    using namespace AscendC::Te;
    expect_l12bt_size_trap<float, float>(
        MakeFrameLayout<NDLayoutPtn, float>(16, 64), MakeFrameLayout<NDLayoutPtn, float>(8, 64));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12BTBatchRejectsSmallerDestination)
{
    using namespace AscendC::Te;
    expect_l12bt_size_trap<half, float>(
        MakeFrameLayout<NDLayoutPtn, half>(2, 2, 16), MakeFrameLayout<NDLayoutPtn, float>(2, 1, 16));
    expect_l12bt_size_trap<half, float>(
        MakeFrameLayout<NDExtLayoutPtn, half>(2, 2, 16), MakeFrameLayout<NDExtLayoutPtn, float>(2, 1, 16));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12FBRejectsBlockCountAboveLimitForEachNDLayout)
{
    using namespace AscendC::Te;
    int rows = 4096;
    int columns = 16;
    expect_l12fb_trap(MakeFrameLayout<NDLayoutPtn>(rows, columns), MakeFrameLayout<NDLayoutPtn>(rows, columns));
    expect_l12fb_trap(MakeFrameLayout<NDExtLayoutPtn>(rows, columns), MakeFrameLayout<NDExtLayoutPtn>(rows, columns));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12FBRejectsSmallerDestinationForNDLayout)
{
    using namespace AscendC::Te;
    expect_l12fb_size_trap(
        MakeFrameLayout<NDLayoutPtn, uint64_t>(8, 64), MakeFrameLayout<NDLayoutPtn, uint64_t>(4, 64));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12L0ARejectsSmallerDestinationForTransposeRoute)
{
    using namespace AscendC::Te;
    expect_l12l0a_size_trap<half>(
        MakeFrameLayout<ZNLayoutPtn, half>(32, 32), MakeFrameLayout<NZLayoutPtn, half>(16, 32));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12L0ARejectsSmallerDestinationForB8B4TransposeRoutes)
{
    using namespace AscendC::Te;
    expect_l12l0a_size_trap<int8_t>(
        MakeFrameLayout<ZNLayoutPtn, int8_t>(32, 32), MakeFrameLayout<NZLayoutPtn, int8_t>(16, 32));
    expect_l12l0a_size_trap<fp4x2_e1m2_t>(
        MakeFrameLayout<ZNLayoutPtn, fp4x2_e1m2_t>(64, 64), MakeFrameLayout<NZLayoutPtn, fp4x2_e1m2_t>(16, 64));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12L0AImg2ColRejectsSmallerDestination)
{
    using namespace AscendC::Te;
    auto src_layout = MakeFrameLayout<NC1HWC0LayoutPtn>(1, 2, 5, 5, 16);
    auto dst_layout = MakeFrameLayout<NZLayoutPtn, int16_t>(16, 16);
    ASSERT_LT(static_cast<std::size_t>(dst_layout.Size()), static_cast<std::size_t>(src_layout.Size()));

    aligned_backing_buffer<int16_t> src(backing_buffer_elements(src_layout));
    aligned_backing_buffer<int16_t> dst(backing_buffer_elements(dst_layout));
    auto src_tensor = make_tensor_at<Location::L1>(src.data(), src_layout);
    auto dst_tensor = make_tensor_at<Location::L0A>(dst.data(), dst_layout);
    Img2ColParams<int16_t> params;
    params.mExtension = 16;
    params.kExtension = 16;

    EXPECT_THROW((copy_tensor_with<CopyL12L0A>(dst_tensor, src_tensor, params)), TrapException);
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12L0BRejectsSmallerDestinationForNormalRoute)
{
    using namespace AscendC::Te;
    expect_l12l0b_size_trap<float>(
        MakeFrameLayout<ZNLayoutPtn, float>(32, 32), MakeFrameLayout<ZNLayoutPtn, float>(16, 32));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12L0BRejectsSmallerDestinationForB8B4TransposeRoutes)
{
    using namespace AscendC::Te;
    expect_l12l0b_size_trap<int8_t>(
        MakeFrameLayout<NZLayoutPtn, int8_t>(32, 32), MakeFrameLayout<ZNLayoutPtn, int8_t>(32, 16));
    expect_l12l0b_size_trap<fp4x2_e1m2_t>(
        MakeFrameLayout<NZLayoutPtn, fp4x2_e1m2_t>(64, 64), MakeFrameLayout<ZNLayoutPtn, fp4x2_e1m2_t>(64, 16));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12L0ABatchRoutesRejectSmallerDestination)
{
    using namespace AscendC::Te;
    expect_l12l0a_size_trap<float>(
        MakeFrameLayout<NZLayoutPtn, float>(2, 32, 32), MakeFrameLayout<NZLayoutPtn, float>(2, 16, 32));
    expect_l12l0a_size_trap<half>(
        MakeFrameLayout<ZNLayoutPtn, half>(2, 32, 32), MakeFrameLayout<NZLayoutPtn, half>(2, 16, 32));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12L0BBatchRoutesRejectSmallerDestination)
{
    using namespace AscendC::Te;
    expect_l12l0b_size_trap<float>(
        MakeFrameLayout<ZNLayoutPtn, float>(2, 32, 32), MakeFrameLayout<ZNLayoutPtn, float>(2, 16, 32));
    expect_l12l0b_size_trap<half>(
        MakeFrameLayout<NZLayoutPtn, half>(2, 32, 32), MakeFrameLayout<ZNLayoutPtn, half>(2, 16, 32));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12L0ARejectsNormalRouteStepAboveLimit)
{
    using namespace AscendC::Te;
    expect_l12l0a_trap<float>(
        MakeFrameLayout<NZLayoutPtn, float>(4096, 8), MakeFrameLayout<NZLayoutPtn, float>(4096, 8));
    expect_l12l0a_trap<float>(
        MakeFrameLayout<NZLayoutPtn, float>(16, 2041), MakeFrameLayout<NZLayoutPtn, float>(16, 2041));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12L0ARejectsTransposeRouteStepAboveLimit)
{
    using namespace AscendC::Te;
    expect_l12l0a_trap<half>(
        MakeFrameLayout<ZNLayoutPtn, half>(16, 4096), MakeFrameLayout<NZLayoutPtn, half>(16, 4096));
    expect_l12l0a_trap<half>(
        MakeFrameLayout<ZNLayoutPtn, half>(4096, 16), MakeFrameLayout<NZLayoutPtn, half>(4096, 16));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12L0ARejectsB8B4TransposeStepAboveLimit)
{
    using namespace AscendC::Te;
    expect_l12l0a_trap<int8_t>(
        MakeFrameLayout<ZNLayoutPtn, int8_t>(32, 4096), MakeFrameLayout<NZLayoutPtn, int8_t>(32, 4096));
    expect_l12l0a_trap<fp4x2_e1m2_t>(
        MakeFrameLayout<ZNLayoutPtn, fp4x2_e1m2_t>(64, 4096), MakeFrameLayout<NZLayoutPtn, fp4x2_e1m2_t>(64, 4096));
    expect_l12l0a_trap<int8_t>(
        MakeFrameLayout<ZNLayoutPtn, int8_t>(8192, 32), MakeFrameLayout<NZLayoutPtn, int8_t>(8192, 32));
    expect_l12l0a_trap<fp4x2_e1m2_t>(
        MakeFrameLayout<ZNLayoutPtn, fp4x2_e1m2_t>(16384, 64), MakeFrameLayout<NZLayoutPtn, fp4x2_e1m2_t>(16384, 64));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12L0ARejectsBatchRouteStepAboveLimit)
{
    using namespace AscendC::Te;
    expect_l12l0a_trap<float>(
        MakeFrameLayout<NZLayoutPtn, float>(2, 4096, 8), MakeFrameLayout<NZLayoutPtn, float>(2, 4096, 8));
    expect_l12l0a_trap<float>(
        MakeFrameLayout<NZLayoutPtn, float>(256, 16, 8), MakeFrameLayout<NZLayoutPtn, float>(256, 16, 8));
    expect_l12l0a_trap<half>(
        MakeFrameLayout<ZNLayoutPtn, half>(2, 16, 4096), MakeFrameLayout<NZLayoutPtn, half>(2, 16, 4096));
    expect_l12l0a_trap<half>(
        MakeFrameLayout<ZNLayoutPtn, half>(2, 4096, 16), MakeFrameLayout<NZLayoutPtn, half>(2, 4096, 16));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12L0BRejectsNormalRouteStepAboveLimit)
{
    using namespace AscendC::Te;
    expect_l12l0b_trap<float>(
        MakeFrameLayout<ZNLayoutPtn, float>(8, 4096), MakeFrameLayout<ZNLayoutPtn, float>(8, 4096));
    expect_l12l0b_trap<float>(
        MakeFrameLayout<ZNLayoutPtn, float>(2041, 16), MakeFrameLayout<ZNLayoutPtn, float>(2041, 16));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12L0BRejectsTransposeRouteStepAboveLimit)
{
    using namespace AscendC::Te;
    expect_l12l0b_trap<half>(
        MakeFrameLayout<NZLayoutPtn, half>(4096, 16), MakeFrameLayout<ZNLayoutPtn, half>(4096, 16));
    expect_l12l0b_trap<half>(
        MakeFrameLayout<NZLayoutPtn, half>(16, 4096), MakeFrameLayout<ZNLayoutPtn, half>(16, 4096));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12L0BRejectsB8B4TransposeStepAboveLimit)
{
    using namespace AscendC::Te;
    expect_l12l0b_trap<int8_t>(
        MakeFrameLayout<NZLayoutPtn, int8_t>(4096, 32), MakeFrameLayout<ZNLayoutPtn, int8_t>(4096, 32));
    expect_l12l0b_trap<fp4x2_e1m2_t>(
        MakeFrameLayout<NZLayoutPtn, fp4x2_e1m2_t>(4096, 64), MakeFrameLayout<ZNLayoutPtn, fp4x2_e1m2_t>(4096, 64));
    expect_l12l0b_trap<int8_t>(
        MakeFrameLayout<NZLayoutPtn, int8_t>(32, 8192), MakeFrameLayout<ZNLayoutPtn, int8_t>(32, 8192));
    expect_l12l0b_trap<fp4x2_e1m2_t>(
        MakeFrameLayout<NZLayoutPtn, fp4x2_e1m2_t>(64, 16384), MakeFrameLayout<ZNLayoutPtn, fp4x2_e1m2_t>(64, 16384));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12L0BRejectsBatchRouteStepAboveLimit)
{
    using namespace AscendC::Te;
    expect_l12l0b_trap<float>(
        MakeFrameLayout<ZNLayoutPtn, float>(2, 8, 4096), MakeFrameLayout<ZNLayoutPtn, float>(2, 8, 4096));
    expect_l12l0b_trap<float>(
        MakeFrameLayout<ZNLayoutPtn, float>(256, 8, 16), MakeFrameLayout<ZNLayoutPtn, float>(256, 8, 16));
    expect_l12l0b_trap<half>(
        MakeFrameLayout<NZLayoutPtn, half>(2, 4096, 16), MakeFrameLayout<ZNLayoutPtn, half>(2, 4096, 16));
    expect_l12l0b_trap<half>(
        MakeFrameLayout<NZLayoutPtn, half>(2, 16, 4096), MakeFrameLayout<ZNLayoutPtn, half>(2, 16, 4096));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12L0ARejectsBatchMismatchForBothRoutes)
{
    using namespace AscendC::Te;
    expect_l12l0a_trap<float>(
        MakeFrameLayout<NZLayoutPtn, float>(1, 16, 8), MakeFrameLayout<NZLayoutPtn, float>(2, 16, 8));
    expect_l12l0a_trap<half>(
        MakeFrameLayout<ZNLayoutPtn, half>(1, 16, 16), MakeFrameLayout<NZLayoutPtn, half>(2, 16, 16));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12L0BRejectsBatchMismatchForBothRoutes)
{
    using namespace AscendC::Te;
    expect_l12l0b_trap<float>(
        MakeFrameLayout<ZNLayoutPtn, float>(1, 8, 16), MakeFrameLayout<ZNLayoutPtn, float>(2, 8, 16));
    expect_l12l0b_trap<half>(
        MakeFrameLayout<NZLayoutPtn, half>(1, 16, 16), MakeFrameLayout<ZNLayoutPtn, half>(2, 16, 16));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12L0ARejectsNonContinuousBatchLayouts)
{
    using namespace AscendC::Te;
    int batch = 2;
    auto nz_float = MakeFrameLayout<NZLayoutPtn, float>(16, 8);
    auto valid_nz_float = MakeBatchPatternLayout<NZLayoutPtn, GetLayoutTrait<decltype(nz_float)>>(batch, nz_float);
    auto invalid_nz_float_src = make_batch_layout<NZLayoutPtn>(nz_float, batch, nz_float.Capacity() + 1);
    auto invalid_nz_float_dst = make_batch_layout<NZLayoutPtn>(nz_float, batch, nz_float.Capacity() + 1);
    expect_l12l0a_trap<float>(invalid_nz_float_src, valid_nz_float);
    expect_l12l0a_trap<float>(valid_nz_float, invalid_nz_float_dst);

    auto zn_half = MakeFrameLayout<ZNLayoutPtn, half>(16, 16);
    auto nz_half = MakeFrameLayout<NZLayoutPtn, half>(16, 16);
    auto valid_zn_half = MakeBatchPatternLayout<ZNLayoutPtn, GetLayoutTrait<decltype(zn_half)>>(batch, zn_half);
    auto valid_nz_half = MakeBatchPatternLayout<NZLayoutPtn, GetLayoutTrait<decltype(nz_half)>>(batch, nz_half);
    auto invalid_zn_half_src = make_batch_layout<ZNLayoutPtn>(zn_half, batch, zn_half.Capacity() + 1);
    auto invalid_nz_half_dst = make_batch_layout<NZLayoutPtn>(nz_half, batch, nz_half.Capacity() + 1);
    expect_l12l0a_trap<half>(invalid_zn_half_src, valid_nz_half);
    expect_l12l0a_trap<half>(valid_zn_half, invalid_nz_half_dst);
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12L0BRejectsNonContinuousBatchLayouts)
{
    using namespace AscendC::Te;
    int batch = 2;
    auto zn_float = MakeFrameLayout<ZNLayoutPtn, float>(8, 16);
    auto valid_zn_float = MakeBatchPatternLayout<ZNLayoutPtn, GetLayoutTrait<decltype(zn_float)>>(batch, zn_float);
    auto invalid_zn_float_src = make_batch_layout<ZNLayoutPtn>(zn_float, batch, zn_float.Capacity() + 1);
    auto invalid_zn_float_dst = make_batch_layout<ZNLayoutPtn>(zn_float, batch, zn_float.Capacity() + 1);
    expect_l12l0b_trap<float>(invalid_zn_float_src, valid_zn_float);
    expect_l12l0b_trap<float>(valid_zn_float, invalid_zn_float_dst);

    auto nz_half = MakeFrameLayout<NZLayoutPtn, half>(16, 16);
    auto zn_half = MakeFrameLayout<ZNLayoutPtn, half>(16, 16);
    auto valid_nz_half = MakeBatchPatternLayout<NZLayoutPtn, GetLayoutTrait<decltype(nz_half)>>(batch, nz_half);
    auto valid_zn_half = MakeBatchPatternLayout<ZNLayoutPtn, GetLayoutTrait<decltype(zn_half)>>(batch, zn_half);
    auto invalid_nz_half_src = make_batch_layout<NZLayoutPtn>(nz_half, batch, nz_half.Capacity() + 1);
    auto invalid_zn_half_dst = make_batch_layout<ZNLayoutPtn>(zn_half, batch, zn_half.Capacity() + 1);
    expect_l12l0b_trap<half>(invalid_nz_half_src, valid_zn_half);
    expect_l12l0b_trap<half>(valid_nz_half, invalid_zn_half_dst);
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12L0ScaleARejectsEachStepAboveLimit)
{
    using namespace AscendC::Te;
    auto expect_trap = [&](const auto& layout) {
        aligned_backing_buffer<fp8_e8m0_t> src(backing_buffer_elements(layout));
        aligned_backing_buffer<fp8_e8m0_t> dst(backing_buffer_elements(layout));
        auto src_tensor = make_tensor_at<Location::L1>(src.data(), layout);
        auto dst_pointer = MakeMemPtr<Location::L0ScaleA, fp8_e8m0_t>(reinterpret_cast<uint64_t>(dst.data()) / 16);
        auto dst_tensor = MakeTensor(dst_pointer, layout);
        EXPECT_THROW(
            (copy_tensor<CopyL12L0ScaleA, CopyL12L0ScaleATraitDefault>(dst_tensor, src_tensor)), TrapException);
    };

    expect_trap(MakeFrameLayout<ZZLayoutPtn, AscendC::Std::Int<2>>(4096, 2));
    expect_trap(MakeFrameLayout<ZZLayoutPtn, AscendC::Std::Int<2>>(16, 511));
}

TEST_F(TensorApiCubeInputAdditionalValidation, CopyL12L0ScaleBRejectsEachStepAboveLimit)
{
    using namespace AscendC::Te;
    auto expect_trap = [&](const auto& layout) {
        aligned_backing_buffer<fp8_e8m0_t> src(backing_buffer_elements(layout));
        aligned_backing_buffer<fp8_e8m0_t> dst(backing_buffer_elements(layout));
        auto src_tensor = make_tensor_at<Location::L1>(src.data(), layout);
        auto dst_pointer = MakeMemPtr<Location::L0ScaleB, fp8_e8m0_t>(reinterpret_cast<uint64_t>(dst.data()) / 16);
        auto dst_tensor = MakeTensor(dst_pointer, layout);
        EXPECT_THROW(
            (copy_tensor<CopyL12L0ScaleB, CopyL12L0ScaleBTraitDefault>(dst_tensor, src_tensor)), TrapException);
    };

    expect_trap(MakeFrameLayout<NNLayoutPtn, AscendC::Std::Int<2>>(2, 4096));
    expect_trap(MakeFrameLayout<NNLayoutPtn, AscendC::Std::Int<2>>(512, 16));
}

} // namespace
