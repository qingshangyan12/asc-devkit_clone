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
constexpr std::size_t OPERAND_BUFFER_ELEMENTS = 4096;

template <typename ElementType>
class aligned_backing_buffer {
public:
    aligned_backing_buffer() : storage_(OPERAND_BUFFER_ELEMENTS + BACKING_BUFFER_ALIGNMENT / sizeof(ElementType)) {}

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

class TensorApiCubeInputOperandLayoutValidation : public testing::Test {
protected:
    void SetUp() override
    {
        AscendC::SetGCoreType(1);
    }

    void TearDown() override
    {
        AscendC::SetGCoreType(0);
    }
};

template <typename IteratorType, typename LayoutType>
auto make_tensor_directly(const IteratorType& iterator, const LayoutType& layout)
{
    using namespace AscendC::Te;
    using TensorType = decltype(MakeTensor(iterator, layout));
    using EngineType = typename TensorType::engineType;
    return TensorType{EngineType{iterator}, layout};
}

template <typename CopyOperationType, typename CopyTraitType, typename DstTensorType, typename SrcTensorType>
void copy_tensor(const DstTensorType& dst, const SrcTensorType& src)
{
    using namespace AscendC::Te;
    Copy(MakeCopy(CopyOperationType{}, CopyTraitType{}), dst, src);
}

template <typename CopyOperationType, typename CopyTraitType, typename SrcIteratorType, typename SrcLayoutType,
          typename DstIteratorType, typename DstLayoutType>
void expect_invalid_src_layout(const SrcIteratorType& src_iterator, const SrcLayoutType& src_layout,
                               const DstIteratorType& dst_iterator, const DstLayoutType& dst_layout)
{
    using namespace AscendC::Te;
    auto src = make_tensor_directly(src_iterator, src_layout);
    auto dst = MakeTensor(dst_iterator, dst_layout);
    EXPECT_THROW((copy_tensor<CopyOperationType, CopyTraitType>(dst, src)), TrapException);
}

template <typename CopyOperationType, typename CopyTraitType, typename SrcIteratorType, typename SrcLayoutType,
          typename DstIteratorType, typename DstLayoutType>
void expect_invalid_dst_layout(const SrcIteratorType& src_iterator, const SrcLayoutType& src_layout,
                               const DstIteratorType& dst_iterator, const DstLayoutType& dst_layout)
{
    using namespace AscendC::Te;
    auto src = MakeTensor(src_iterator, src_layout);
    auto dst = make_tensor_directly(dst_iterator, dst_layout);
    EXPECT_THROW((copy_tensor<CopyOperationType, CopyTraitType>(dst, src)), TrapException);
}

template <typename CopyOperationType, typename SrcIteratorType, typename SrcLayoutType, typename DstIteratorType,
          typename DstLayoutType, typename ParamType>
void expect_invalid_src_layout_with(const SrcIteratorType& src_iterator, const SrcLayoutType& src_layout,
                                    const DstIteratorType& dst_iterator, const DstLayoutType& dst_layout,
                                    const ParamType& params)
{
    using namespace AscendC::Te;
    auto src = make_tensor_directly(src_iterator, src_layout);
    auto dst = MakeTensor(dst_iterator, dst_layout);
    EXPECT_THROW(Copy(MakeCopy(CopyOperationType{}).with(params), dst, src), TrapException);
}

template <typename CopyOperationType, typename SrcIteratorType, typename SrcLayoutType, typename DstIteratorType,
          typename DstLayoutType, typename ParamType>
void expect_invalid_dst_layout_with(const SrcIteratorType& src_iterator, const SrcLayoutType& src_layout,
                                    const DstIteratorType& dst_iterator, const DstLayoutType& dst_layout,
                                    const ParamType& params)
{
    using namespace AscendC::Te;
    auto src = MakeTensor(src_iterator, src_layout);
    auto dst = make_tensor_directly(dst_iterator, dst_layout);
    EXPECT_THROW(Copy(MakeCopy(CopyOperationType{}).with(params), dst, src), TrapException);
}

template <typename ElementType>
auto make_nd_shape_error(int invalid_shape = 0)
{
    using namespace AscendC::Te;
    return MakePatternLayout<NDLayoutPtn, LayoutTraitDefault<ElementType>>(MakeShape(invalid_shape, 16),
                                                                           MakeStride(16, _1{}));
}

template <typename ElementType>
auto make_nd_stride_error()
{
    using namespace AscendC::Te;
    return MakePatternLayout<NDLayoutPtn, LayoutTraitDefault<ElementType>>(MakeShape(16, 16), MakeStride(-16, _1{}));
}

template <typename ElementType>
auto make_nz_shape_error(int invalid_shape = 0)
{
    using namespace AscendC::Te;
    constexpr auto C0 = C0_ELEMENT<ElementType>;
    using C0Type = AscendC::Std::Int<C0>;
    using FractalStrideType = AscendC::Std::Int<C0 * FRACTAL_FIXED>;
    return MakePatternLayout<NZLayoutPtn, LayoutTraitDefault<ElementType>>(
        MakeShape(MakeShape(_16{}, invalid_shape), MakeShape(C0Type{}, 1)),
        MakeStride(MakeStride(C0Type{}, FractalStrideType{}), MakeStride(_1{}, C0 * FRACTAL_FIXED)));
}

template <typename ElementType>
auto make_nz_stride_error()
{
    using namespace AscendC::Te;
    constexpr auto C0 = C0_ELEMENT<ElementType>;
    using C0Type = AscendC::Std::Int<C0>;
    using FractalStrideType = AscendC::Std::Int<C0 * FRACTAL_FIXED>;
    return MakePatternLayout<NZLayoutPtn, LayoutTraitDefault<ElementType>>(
        MakeShape(MakeShape(_16{}, 1), MakeShape(C0Type{}, 1)),
        MakeStride(MakeStride(C0Type{}, FractalStrideType{}), MakeStride(_1{}, -1)));
}

template <typename ElementType>
auto make_zn_shape_error(int invalid_shape = 0)
{
    using namespace AscendC::Te;
    constexpr auto C0 = C0_ELEMENT<ElementType>;
    using C0Type = AscendC::Std::Int<C0>;
    using FractalStrideType = AscendC::Std::Int<C0 * FRACTAL_FIXED>;
    return MakePatternLayout<ZNLayoutPtn, LayoutTraitDefault<ElementType>>(
        MakeShape(MakeShape(C0Type{}, invalid_shape), MakeShape(_16{}, 1)),
        MakeStride(MakeStride(_1{}, C0 * FRACTAL_FIXED), MakeStride(C0Type{}, FractalStrideType{})));
}

template <typename ElementType>
auto make_zn_stride_error()
{
    using namespace AscendC::Te;
    constexpr auto C0 = C0_ELEMENT<ElementType>;
    using C0Type = AscendC::Std::Int<C0>;
    using FractalStrideType = AscendC::Std::Int<C0 * FRACTAL_FIXED>;
    return MakePatternLayout<ZNLayoutPtn, LayoutTraitDefault<ElementType>>(
        MakeShape(MakeShape(C0Type{}, 1), MakeShape(_16{}, 1)),
        MakeStride(MakeStride(_1{}, -1), MakeStride(C0Type{}, FractalStrideType{})));
}

auto make_nc1hwc0_shape_error(int invalid_shape = 0)
{
    using namespace AscendC::Te;
    using TraitType = GetLayoutTrait<decltype(MakeFrameLayout<NC1HWC0LayoutPtn>(1, 2, 5, 5, 16))>;
    return MakePatternLayout<NC1HWC0LayoutPtn, TraitType>(MakeShape(1, 2, invalid_shape, 5, 16),
                                                          MakeStride(800, 400, 80, 16, _1{}));
}

auto make_nc1hwc0_stride_error()
{
    using namespace AscendC::Te;
    using TraitType = GetLayoutTrait<decltype(MakeFrameLayout<NC1HWC0LayoutPtn>(1, 2, 5, 5, 16))>;
    return MakePatternLayout<NC1HWC0LayoutPtn, TraitType>(MakeShape(1, 2, 5, 5, 16),
                                                          MakeStride(-800, 400, 80, 16, _1{}));
}

auto make_zz_shape_error(int invalid_shape = 0)
{
    using namespace AscendC::Te;
    using TraitType = GetLayoutTrait<decltype(MakeFrameLayout<ZZLayoutPtn, _2>(16, 16))>;
    return MakePatternLayout<ZZLayoutPtn, TraitType>(MakeShape(MakeShape(_16{}, invalid_shape), MakeShape(_2{}, 8)),
                                                     MakeStride(MakeStride(_2{}, 256), MakeStride(_1{}, 32)));
}

auto make_zz_stride_error()
{
    using namespace AscendC::Te;
    using TraitType = GetLayoutTrait<decltype(MakeFrameLayout<ZZLayoutPtn, _2>(16, 16))>;
    return MakePatternLayout<ZZLayoutPtn, TraitType>(MakeShape(MakeShape(_16{}, 1), MakeShape(_2{}, 8)),
                                                     MakeStride(MakeStride(_2{}, -1), MakeStride(_1{}, 32)));
}

auto make_nn_shape_error(int invalid_shape = 0)
{
    using namespace AscendC::Te;
    using TraitType = GetLayoutTrait<decltype(MakeFrameLayout<NNLayoutPtn, _2>(16, 16))>;
    return MakePatternLayout<NNLayoutPtn, TraitType>(MakeShape(MakeShape(_2{}, invalid_shape), MakeShape(_16{}, 1)),
                                                     MakeStride(MakeStride(_1{}, _32{}), MakeStride(_2{}, 256)));
}

auto make_nn_stride_error()
{
    using namespace AscendC::Te;
    using TraitType = GetLayoutTrait<decltype(MakeFrameLayout<NNLayoutPtn, _2>(16, 16))>;
    return MakePatternLayout<NNLayoutPtn, TraitType>(MakeShape(MakeShape(_2{}, 8), MakeShape(_16{}, 1)),
                                                     MakeStride(MakeStride(_1{}, _32{}), MakeStride(_2{}, -1)));
}

TEST_F(TensorApiCubeInputOperandLayoutValidation, CopyGM2L1RejectsInvalidSourceAndDestinationLayouts)
{
    using namespace AscendC::Te;
    aligned_backing_buffer<half> src_buffer;
    aligned_backing_buffer<half> dst_buffer;
    auto src_iterator = MakeMemPtr<Location::GM>(src_buffer.data());
    auto dst_iterator = MakeMemPtr<Location::L1>(dst_buffer.data());
    auto valid_layout = MakeFrameLayout<NDLayoutPtn, half>(16, 16);

    expect_invalid_src_layout<CopyGM2L1, CopyGM2L1TraitDefault>(src_iterator, make_nd_shape_error<half>(), dst_iterator,
                                                                valid_layout);
    expect_invalid_src_layout<CopyGM2L1, CopyGM2L1TraitDefault>(src_iterator, make_nd_shape_error<half>(-1),
                                                                dst_iterator, valid_layout);
    expect_invalid_src_layout<CopyGM2L1, CopyGM2L1TraitDefault>(src_iterator, make_nd_stride_error<half>(),
                                                                dst_iterator, valid_layout);
    expect_invalid_dst_layout<CopyGM2L1, CopyGM2L1TraitDefault>(src_iterator, valid_layout, dst_iterator,
                                                                make_nd_shape_error<half>());
    expect_invalid_dst_layout<CopyGM2L1, CopyGM2L1TraitDefault>(src_iterator, valid_layout, dst_iterator,
                                                                make_nd_shape_error<half>(-1));
    expect_invalid_dst_layout<CopyGM2L1, CopyGM2L1TraitDefault>(src_iterator, valid_layout, dst_iterator,
                                                                make_nd_stride_error<half>());
}

TEST_F(TensorApiCubeInputOperandLayoutValidation, CopyL12UBRejectsInvalidSourceAndDestinationLayouts)
{
    using namespace AscendC::Te;
    aligned_backing_buffer<half> src_buffer;
    aligned_backing_buffer<half> dst_buffer;
    auto src_iterator = MakeMemPtr<Location::L1>(src_buffer.data());
    auto dst_iterator = MakeMemPtr<Location::UB>(dst_buffer.data());
    auto valid_layout = MakeFrameLayout<NDLayoutPtn, half>(16, 16);

    expect_invalid_src_layout<CopyL12UB, CopyL12UBTraitDefault>(src_iterator, make_nd_shape_error<half>(), dst_iterator,
                                                                valid_layout);
    expect_invalid_src_layout<CopyL12UB, CopyL12UBTraitDefault>(src_iterator, make_nd_shape_error<half>(-1),
                                                                dst_iterator, valid_layout);
    expect_invalid_src_layout<CopyL12UB, CopyL12UBTraitDefault>(src_iterator, make_nd_stride_error<half>(),
                                                                dst_iterator, valid_layout);
    expect_invalid_dst_layout<CopyL12UB, CopyL12UBTraitDefault>(src_iterator, valid_layout, dst_iterator,
                                                                make_nd_shape_error<half>());
    expect_invalid_dst_layout<CopyL12UB, CopyL12UBTraitDefault>(src_iterator, valid_layout, dst_iterator,
                                                                make_nd_shape_error<half>(-1));
    expect_invalid_dst_layout<CopyL12UB, CopyL12UBTraitDefault>(src_iterator, valid_layout, dst_iterator,
                                                                make_nd_stride_error<half>());
}

TEST_F(TensorApiCubeInputOperandLayoutValidation, CopyL12BTRejectsInvalidSourceAndDestinationLayouts)
{
    using namespace AscendC::Te;
    aligned_backing_buffer<half> src_buffer;
    aligned_backing_buffer<float> dst_buffer;
    auto src_iterator = MakeMemPtr<Location::L1>(src_buffer.data());
    auto dst_iterator = MakeMemPtr<Location::BIAS>(dst_buffer.data());
    auto valid_src_layout = MakeFrameLayout<NDLayoutPtn, half>(16, 16);
    auto valid_dst_layout = MakeFrameLayout<NDLayoutPtn, float>(16, 16);

    expect_invalid_src_layout<CopyL12BT, CopyL12BTTraitDefault>(src_iterator, make_nd_shape_error<half>(), dst_iterator,
                                                                valid_dst_layout);
    expect_invalid_src_layout<CopyL12BT, CopyL12BTTraitDefault>(src_iterator, make_nd_shape_error<half>(-1),
                                                                dst_iterator, valid_dst_layout);
    expect_invalid_src_layout<CopyL12BT, CopyL12BTTraitDefault>(src_iterator, make_nd_stride_error<half>(),
                                                                dst_iterator, valid_dst_layout);
    expect_invalid_dst_layout<CopyL12BT, CopyL12BTTraitDefault>(src_iterator, valid_src_layout, dst_iterator,
                                                                make_nd_shape_error<float>());
    expect_invalid_dst_layout<CopyL12BT, CopyL12BTTraitDefault>(src_iterator, valid_src_layout, dst_iterator,
                                                                make_nd_shape_error<float>(-1));
    expect_invalid_dst_layout<CopyL12BT, CopyL12BTTraitDefault>(src_iterator, valid_src_layout, dst_iterator,
                                                                make_nd_stride_error<float>());
}

TEST_F(TensorApiCubeInputOperandLayoutValidation, CopyL12FBRejectsInvalidSourceAndDestinationLayouts)
{
    using namespace AscendC::Te;
    aligned_backing_buffer<uint64_t> src_buffer;
    aligned_backing_buffer<uint64_t> dst_buffer;
    auto src_iterator = MakeMemPtr<Location::L1>(src_buffer.data());
    auto dst_iterator = MakeMemPtr<Location::FIXBUF>(dst_buffer.data());
    auto valid_layout = MakeFrameLayout<NDLayoutPtn, uint64_t>(16, 16);

    expect_invalid_src_layout<CopyL12FB, CopyL12FBTraitDefault>(src_iterator, make_nd_shape_error<uint64_t>(),
                                                                dst_iterator, valid_layout);
    expect_invalid_src_layout<CopyL12FB, CopyL12FBTraitDefault>(src_iterator, make_nd_shape_error<uint64_t>(-1),
                                                                dst_iterator, valid_layout);
    expect_invalid_src_layout<CopyL12FB, CopyL12FBTraitDefault>(src_iterator, make_nd_stride_error<uint64_t>(),
                                                                dst_iterator, valid_layout);
    expect_invalid_dst_layout<CopyL12FB, CopyL12FBTraitDefault>(src_iterator, valid_layout, dst_iterator,
                                                                make_nd_shape_error<uint64_t>());
    expect_invalid_dst_layout<CopyL12FB, CopyL12FBTraitDefault>(src_iterator, valid_layout, dst_iterator,
                                                                make_nd_shape_error<uint64_t>(-1));
    expect_invalid_dst_layout<CopyL12FB, CopyL12FBTraitDefault>(src_iterator, valid_layout, dst_iterator,
                                                                make_nd_stride_error<uint64_t>());
}

TEST_F(TensorApiCubeInputOperandLayoutValidation, CopyL12L0ANormalRejectsInvalidSourceAndDestinationLayouts)
{
    using namespace AscendC::Te;
    aligned_backing_buffer<half> src_buffer;
    aligned_backing_buffer<half> dst_buffer;
    auto src_iterator = MakeMemPtr<Location::L1>(src_buffer.data());
    auto dst_iterator = MakeMemPtr<Location::L0A>(dst_buffer.data());
    auto valid_layout = MakeFrameLayout<NZLayoutPtn, half>(16, 16);

    expect_invalid_src_layout<CopyL12L0A, CopyL12L0ATraitDefault>(src_iterator, make_nz_shape_error<half>(),
                                                                  dst_iterator, valid_layout);
    expect_invalid_src_layout<CopyL12L0A, CopyL12L0ATraitDefault>(src_iterator, make_nz_shape_error<half>(-1),
                                                                  dst_iterator, valid_layout);
    expect_invalid_src_layout<CopyL12L0A, CopyL12L0ATraitDefault>(src_iterator, make_nz_stride_error<half>(),
                                                                  dst_iterator, valid_layout);
    expect_invalid_dst_layout<CopyL12L0A, CopyL12L0ATraitDefault>(src_iterator, valid_layout, dst_iterator,
                                                                  make_nz_shape_error<half>());
    expect_invalid_dst_layout<CopyL12L0A, CopyL12L0ATraitDefault>(src_iterator, valid_layout, dst_iterator,
                                                                  make_nz_shape_error<half>(-1));
    expect_invalid_dst_layout<CopyL12L0A, CopyL12L0ATraitDefault>(src_iterator, valid_layout, dst_iterator,
                                                                  make_nz_stride_error<half>());
}

TEST_F(TensorApiCubeInputOperandLayoutValidation, CopyL12L0ATransposeRejectsInvalidSourceAndDestinationLayouts)
{
    using namespace AscendC::Te;
    aligned_backing_buffer<half> src_buffer;
    aligned_backing_buffer<half> dst_buffer;
    auto src_iterator = MakeMemPtr<Location::L1>(src_buffer.data());
    auto dst_iterator = MakeMemPtr<Location::L0A>(dst_buffer.data());
    auto valid_src_layout = MakeFrameLayout<ZNLayoutPtn, half>(16, 16);
    auto valid_dst_layout = MakeFrameLayout<NZLayoutPtn, half>(16, 16);

    expect_invalid_src_layout<CopyL12L0A, CopyL12L0ATraitDefault>(src_iterator, make_zn_shape_error<half>(),
                                                                  dst_iterator, valid_dst_layout);
    expect_invalid_src_layout<CopyL12L0A, CopyL12L0ATraitDefault>(src_iterator, make_zn_shape_error<half>(-1),
                                                                  dst_iterator, valid_dst_layout);
    expect_invalid_src_layout<CopyL12L0A, CopyL12L0ATraitDefault>(src_iterator, make_zn_stride_error<half>(),
                                                                  dst_iterator, valid_dst_layout);
    expect_invalid_dst_layout<CopyL12L0A, CopyL12L0ATraitDefault>(src_iterator, valid_src_layout, dst_iterator,
                                                                  make_nz_shape_error<half>());
    expect_invalid_dst_layout<CopyL12L0A, CopyL12L0ATraitDefault>(src_iterator, valid_src_layout, dst_iterator,
                                                                  make_nz_shape_error<half>(-1));
    expect_invalid_dst_layout<CopyL12L0A, CopyL12L0ATraitDefault>(src_iterator, valid_src_layout, dst_iterator,
                                                                  make_nz_stride_error<half>());
}

TEST_F(TensorApiCubeInputOperandLayoutValidation, CopyL12L0AImg2ColRejectsInvalidSourceAndDestinationLayouts)
{
    using namespace AscendC::Te;
    aligned_backing_buffer<int16_t> src_buffer;
    aligned_backing_buffer<int16_t> dst_buffer;
    auto src_iterator = MakeMemPtr<Location::L1>(src_buffer.data());
    auto dst_iterator = MakeMemPtr<Location::L0A>(dst_buffer.data());
    auto valid_src_layout = MakeFrameLayout<NC1HWC0LayoutPtn>(1, 2, 5, 5, 16);
    auto valid_dst_layout = MakeFrameLayout<NZLayoutPtn, int16_t>(32, 32);
    Img2ColParams<int16_t> params;
    params.mExtension = 32;
    params.kExtension = 32;

    expect_invalid_src_layout_with<CopyL12L0A>(src_iterator, make_nc1hwc0_shape_error(), dst_iterator, valid_dst_layout,
                                               params);
    expect_invalid_src_layout_with<CopyL12L0A>(src_iterator, make_nc1hwc0_shape_error(-1), dst_iterator,
                                               valid_dst_layout, params);
    expect_invalid_src_layout_with<CopyL12L0A>(src_iterator, make_nc1hwc0_stride_error(), dst_iterator,
                                               valid_dst_layout, params);
    expect_invalid_dst_layout_with<CopyL12L0A>(src_iterator, valid_src_layout, dst_iterator,
                                               make_nz_shape_error<int16_t>(), params);
    expect_invalid_dst_layout_with<CopyL12L0A>(src_iterator, valid_src_layout, dst_iterator,
                                               make_nz_shape_error<int16_t>(-1), params);
    expect_invalid_dst_layout_with<CopyL12L0A>(src_iterator, valid_src_layout, dst_iterator,
                                               make_nz_stride_error<int16_t>(), params);
}

TEST_F(TensorApiCubeInputOperandLayoutValidation, CopyL12L0BNormalRejectsInvalidSourceAndDestinationLayouts)
{
    using namespace AscendC::Te;
    aligned_backing_buffer<half> src_buffer;
    aligned_backing_buffer<half> dst_buffer;
    auto src_iterator = MakeMemPtr<Location::L1>(src_buffer.data());
    auto dst_iterator = MakeMemPtr<Location::L0B>(dst_buffer.data());
    auto valid_layout = MakeFrameLayout<ZNLayoutPtn, half>(16, 16);

    expect_invalid_src_layout<CopyL12L0B, CopyL12L0BTraitDefault>(src_iterator, make_zn_shape_error<half>(),
                                                                  dst_iterator, valid_layout);
    expect_invalid_src_layout<CopyL12L0B, CopyL12L0BTraitDefault>(src_iterator, make_zn_shape_error<half>(-1),
                                                                  dst_iterator, valid_layout);
    expect_invalid_src_layout<CopyL12L0B, CopyL12L0BTraitDefault>(src_iterator, make_zn_stride_error<half>(),
                                                                  dst_iterator, valid_layout);
    expect_invalid_dst_layout<CopyL12L0B, CopyL12L0BTraitDefault>(src_iterator, valid_layout, dst_iterator,
                                                                  make_zn_shape_error<half>());
    expect_invalid_dst_layout<CopyL12L0B, CopyL12L0BTraitDefault>(src_iterator, valid_layout, dst_iterator,
                                                                  make_zn_shape_error<half>(-1));
    expect_invalid_dst_layout<CopyL12L0B, CopyL12L0BTraitDefault>(src_iterator, valid_layout, dst_iterator,
                                                                  make_zn_stride_error<half>());
}

TEST_F(TensorApiCubeInputOperandLayoutValidation, CopyL12L0BTransposeRejectsInvalidSourceAndDestinationLayouts)
{
    using namespace AscendC::Te;
    aligned_backing_buffer<half> src_buffer;
    aligned_backing_buffer<half> dst_buffer;
    auto src_iterator = MakeMemPtr<Location::L1>(src_buffer.data());
    auto dst_iterator = MakeMemPtr<Location::L0B>(dst_buffer.data());
    auto valid_src_layout = MakeFrameLayout<NZLayoutPtn, half>(16, 16);
    auto valid_dst_layout = MakeFrameLayout<ZNLayoutPtn, half>(16, 16);

    expect_invalid_src_layout<CopyL12L0B, CopyL12L0BTraitDefault>(src_iterator, make_nz_shape_error<half>(),
                                                                  dst_iterator, valid_dst_layout);
    expect_invalid_src_layout<CopyL12L0B, CopyL12L0BTraitDefault>(src_iterator, make_nz_shape_error<half>(-1),
                                                                  dst_iterator, valid_dst_layout);
    expect_invalid_src_layout<CopyL12L0B, CopyL12L0BTraitDefault>(src_iterator, make_nz_stride_error<half>(),
                                                                  dst_iterator, valid_dst_layout);
    expect_invalid_dst_layout<CopyL12L0B, CopyL12L0BTraitDefault>(src_iterator, valid_src_layout, dst_iterator,
                                                                  make_zn_shape_error<half>());
    expect_invalid_dst_layout<CopyL12L0B, CopyL12L0BTraitDefault>(src_iterator, valid_src_layout, dst_iterator,
                                                                  make_zn_shape_error<half>(-1));
    expect_invalid_dst_layout<CopyL12L0B, CopyL12L0BTraitDefault>(src_iterator, valid_src_layout, dst_iterator,
                                                                  make_zn_stride_error<half>());
}

TEST_F(TensorApiCubeInputOperandLayoutValidation, CopyL12L0ScaleARejectsInvalidSourceAndDestinationLayouts)
{
    using namespace AscendC::Te;
    aligned_backing_buffer<fp8_e8m0_t> src_buffer;
    aligned_backing_buffer<fp8_e8m0_t> dst_buffer;
    auto src_iterator = MakeMemPtr<Location::L1>(src_buffer.data());
    auto dst_iterator = MakeMemPtr<Location::L0ScaleA, fp8_e8m0_t>(reinterpret_cast<uint64_t>(dst_buffer.data()) / 16);
    auto valid_layout = MakeFrameLayout<ZZLayoutPtn, _2>(16, 16);

    expect_invalid_src_layout<CopyL12L0ScaleA, CopyL12L0ScaleATraitDefault>(src_iterator, make_zz_shape_error(),
                                                                            dst_iterator, valid_layout);
    expect_invalid_src_layout<CopyL12L0ScaleA, CopyL12L0ScaleATraitDefault>(src_iterator, make_zz_shape_error(-1),
                                                                            dst_iterator, valid_layout);
    expect_invalid_src_layout<CopyL12L0ScaleA, CopyL12L0ScaleATraitDefault>(src_iterator, make_zz_stride_error(),
                                                                            dst_iterator, valid_layout);
    expect_invalid_dst_layout<CopyL12L0ScaleA, CopyL12L0ScaleATraitDefault>(src_iterator, valid_layout, dst_iterator,
                                                                            make_zz_shape_error());
    expect_invalid_dst_layout<CopyL12L0ScaleA, CopyL12L0ScaleATraitDefault>(src_iterator, valid_layout, dst_iterator,
                                                                            make_zz_shape_error(-1));
    expect_invalid_dst_layout<CopyL12L0ScaleA, CopyL12L0ScaleATraitDefault>(src_iterator, valid_layout, dst_iterator,
                                                                            make_zz_stride_error());
}

TEST_F(TensorApiCubeInputOperandLayoutValidation, CopyL12L0ScaleBRejectsInvalidSourceAndDestinationLayouts)
{
    using namespace AscendC::Te;
    aligned_backing_buffer<fp8_e8m0_t> src_buffer;
    aligned_backing_buffer<fp8_e8m0_t> dst_buffer;
    auto src_iterator = MakeMemPtr<Location::L1>(src_buffer.data());
    auto dst_iterator = MakeMemPtr<Location::L0ScaleB, fp8_e8m0_t>(reinterpret_cast<uint64_t>(dst_buffer.data()) / 16);
    auto valid_layout = MakeFrameLayout<NNLayoutPtn, _2>(16, 16);

    expect_invalid_src_layout<CopyL12L0ScaleB, CopyL12L0ScaleBTraitDefault>(src_iterator, make_nn_shape_error(),
                                                                            dst_iterator, valid_layout);
    expect_invalid_src_layout<CopyL12L0ScaleB, CopyL12L0ScaleBTraitDefault>(src_iterator, make_nn_shape_error(-1),
                                                                            dst_iterator, valid_layout);
    expect_invalid_src_layout<CopyL12L0ScaleB, CopyL12L0ScaleBTraitDefault>(src_iterator, make_nn_stride_error(),
                                                                            dst_iterator, valid_layout);
    expect_invalid_dst_layout<CopyL12L0ScaleB, CopyL12L0ScaleBTraitDefault>(src_iterator, valid_layout, dst_iterator,
                                                                            make_nn_shape_error());
    expect_invalid_dst_layout<CopyL12L0ScaleB, CopyL12L0ScaleBTraitDefault>(src_iterator, valid_layout, dst_iterator,
                                                                            make_nn_shape_error(-1));
    expect_invalid_dst_layout<CopyL12L0ScaleB, CopyL12L0ScaleBTraitDefault>(src_iterator, valid_layout, dst_iterator,
                                                                            make_nn_stride_error());
}

} // namespace
