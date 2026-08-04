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

using namespace AscendC::Te;
using tensor_api_runtime_validation::TrapException;

class TensorApiCubeOutAdditionalRuntimeValidation : public testing::Test {
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

enum class QuantVariant { NONE, SCALAR, TENSOR };

enum class BatchQuantVariant { NONE, SCALAR, SHARED_TENSOR, BATCHED_TENSOR };

template <typename LocationType, typename PointerType, typename LayoutType>
auto make_tensor_at(PointerType pointer, const LayoutType& layout)
{
    return MakeTensor(MakeMemPtr<LocationType>(pointer), layout);
}

template <typename LocationType, typename PointerType, typename LayoutType>
auto make_tensor_directly(PointerType pointer, const LayoutType& layout)
{
    auto iterator = MakeMemPtr<LocationType>(pointer);
    using TensorType = decltype(MakeTensor(iterator, layout));
    using EngineType = typename TensorType::engineType;
    return TensorType{EngineType{iterator}, layout};
}

enum class InvalidLayoutKind { ZERO_SHAPE, NEGATIVE_SHAPE, STRIDE };

enum class InvalidTensorRole { DST, SRC, QUANT, FM, FILTER, BIAS };

template <typename ElementType, InvalidLayoutKind kind>
auto make_invalid_nd_layout()
{
    constexpr int M = 16;
    constexpr int N = 16;
    if constexpr (kind == InvalidLayoutKind::ZERO_SHAPE) {
        int invalid_n = 0;
        return MakeFrameLayout<NDLayoutPtn, ElementType>(M, invalid_n);
    } else if constexpr (kind == InvalidLayoutKind::NEGATIVE_SHAPE) {
        int invalid_n = -1;
        auto invalid_shape_layout = MakeFrameLayout<NDLayoutPtn, ElementType>(M, invalid_n);
        auto valid_layout = MakeFrameLayout<NDLayoutPtn, ElementType>(M, N);
        using LayoutTraitType = GetLayoutTrait<decltype(valid_layout)>;
        return MakePatternLayout<NDLayoutPtn, LayoutTraitType>(invalid_shape_layout.Shape(), valid_layout.Stride());
    } else {
        int invalid_stride = -1;
        auto valid_layout = MakeFrameLayout<NDLayoutPtn, ElementType>(M, N);
        using LayoutTraitType = GetLayoutTrait<decltype(valid_layout)>;
        return MakePatternLayout<NDLayoutPtn, LayoutTraitType>(valid_layout.Shape(), MakeStride(invalid_stride, _1{}));
    }
}

template <typename ElementType, InvalidLayoutKind kind>
auto make_invalid_ndext_layout()
{
    constexpr int M = 16;
    constexpr int N = 16;
    if constexpr (kind == InvalidLayoutKind::ZERO_SHAPE) {
        int invalid_n = 0;
        return MakeFrameLayout<NDExtLayoutPtn, ElementType>(M, invalid_n);
    } else if constexpr (kind == InvalidLayoutKind::NEGATIVE_SHAPE) {
        int invalid_n = -1;
        auto invalid_shape_layout = MakeFrameLayout<NDExtLayoutPtn, ElementType>(M, invalid_n);
        auto valid_layout = MakeFrameLayout<NDExtLayoutPtn, ElementType>(M, N);
        using LayoutTraitType = GetLayoutTrait<decltype(valid_layout)>;
        return MakePatternLayout<NDExtLayoutPtn, LayoutTraitType>(invalid_shape_layout.Shape(), valid_layout.Stride());
    } else {
        int invalid_stride = -1;
        auto valid_layout = MakeFrameLayout<NDExtLayoutPtn, ElementType>(M, N);
        using LayoutTraitType = GetLayoutTrait<decltype(valid_layout)>;
        auto stride = MakeStride(MakeStride(_0{}, invalid_stride), MakeStride(_0{}, _1{}));
        return MakePatternLayout<NDExtLayoutPtn, LayoutTraitType>(valid_layout.Shape(), stride);
    }
}

template <typename ElementType, InvalidLayoutKind kind, typename LayoutTraitType = LayoutTraitDefault<ElementType>>
auto make_invalid_nz_layout()
{
    constexpr int M = 16;
    constexpr int N = 16;
    if constexpr (kind == InvalidLayoutKind::ZERO_SHAPE) {
        int invalid_n = 0;
        return MakeFrameLayout<NZLayoutPtn, LayoutTraitType>(M, invalid_n);
    } else if constexpr (kind == InvalidLayoutKind::NEGATIVE_SHAPE) {
        int invalid_outer_extent = -1;
        auto valid_layout = MakeFrameLayout<NZLayoutPtn, LayoutTraitType>(M, N);
        using ActualLayoutTraitType = GetLayoutTrait<decltype(valid_layout)>;
        auto shape = MakeShape(MakeShape(Get<0>(Get<0>(valid_layout.Shape())), invalid_outer_extent),
                               Get<1>(valid_layout.Shape()));
        return MakePatternLayout<NZLayoutPtn, ActualLayoutTraitType>(shape, valid_layout.Stride());
    } else {
        int invalid_stride = -1;
        auto valid_layout = MakeFrameLayout<NZLayoutPtn, LayoutTraitType>(M, N);
        using ActualLayoutTraitType = GetLayoutTrait<decltype(valid_layout)>;
        auto stride = MakeStride(Get<0>(valid_layout.Stride()), MakeStride(_1{}, invalid_stride));
        return MakePatternLayout<NZLayoutPtn, ActualLayoutTraitType>(valid_layout.Shape(), stride);
    }
}

template <typename ElementType, InvalidLayoutKind kind>
auto make_invalid_zn_layout()
{
    constexpr int M = 16;
    constexpr int N = 16;
    if constexpr (kind == InvalidLayoutKind::ZERO_SHAPE) {
        int invalid_n = 0;
        return MakeFrameLayout<ZNLayoutPtn, ElementType>(M, invalid_n);
    } else if constexpr (kind == InvalidLayoutKind::NEGATIVE_SHAPE) {
        int invalid_outer_extent = -1;
        auto valid_layout = MakeFrameLayout<ZNLayoutPtn, ElementType>(M, N);
        using LayoutTraitType = GetLayoutTrait<decltype(valid_layout)>;
        auto shape = MakeShape(MakeShape(Get<0>(Get<0>(valid_layout.Shape())), invalid_outer_extent),
                               Get<1>(valid_layout.Shape()));
        return MakePatternLayout<ZNLayoutPtn, LayoutTraitType>(shape, valid_layout.Stride());
    } else {
        int invalid_stride = -1;
        auto valid_layout = MakeFrameLayout<ZNLayoutPtn, ElementType>(M, N);
        using LayoutTraitType = GetLayoutTrait<decltype(valid_layout)>;
        auto stride = MakeStride(MakeStride(_1{}, invalid_stride), Get<1>(valid_layout.Stride()));
        return MakePatternLayout<ZNLayoutPtn, LayoutTraitType>(valid_layout.Shape(), stride);
    }
}

template <typename LayoutPattern>
auto make_zero_row_stride_layout()
{
    constexpr int M = 16;
    constexpr int N = 16;
    if constexpr (AscendC::Std::is_same_v<LayoutPattern, NDLayoutPtn>) {
        auto valid_layout = MakeFrameLayout<NDLayoutPtn, float>(M, N);
        using LayoutTraitType = GetLayoutTrait<decltype(valid_layout)>;
        return MakePatternLayout<NDLayoutPtn, LayoutTraitType>(valid_layout.Shape(), MakeStride(0, _1{}));
    } else {
        auto valid_layout = MakeFrameLayout<NDExtLayoutPtn, float>(M, N);
        using LayoutTraitType = GetLayoutTrait<decltype(valid_layout)>;
        auto stride = MakeStride(MakeStride(_0{}, 0), MakeStride(_0{}, _1{}));
        return MakePatternLayout<NDExtLayoutPtn, LayoutTraitType>(valid_layout.Shape(), stride);
    }
}

template <typename ElementType, typename LayoutTraitType = LayoutTraitDefault<ElementType>>
auto make_zero_nz_column_stride_layout(int m, int n)
{
    auto valid_layout = MakeFrameLayout<NZLayoutPtn, LayoutTraitType>(m, n);
    using ActualLayoutTraitType = GetLayoutTrait<decltype(valid_layout)>;
    auto stride = MakeStride(Get<0>(valid_layout.Stride()), MakeStride(_1{}, 0));
    return MakePatternLayout<NZLayoutPtn, ActualLayoutTraitType>(valid_layout.Shape(), stride);
}

template <QuantVariant variant, typename CopyOperationType, typename CopyTraitType, typename DstTensorType,
          typename SrcTensorType, typename QuantTensorType>
void copy_with_quant_variant(const DstTensorType& dst, const SrcTensorType& src, const QuantTensorType& quant,
                             const FixpipeParams& params = FixpipeParams{})
{
    auto atom = MakeCopy(CopyOperationType{}, CopyTraitType{}).with(params);
    if constexpr (variant == QuantVariant::NONE) {
        Copy(atom, dst, src);
    } else if constexpr (variant == QuantVariant::SCALAR) {
        uint64_t scalar_quant = 1;
        Copy(atom, dst, src, scalar_quant);
    } else {
        Copy(atom, dst, src, quant);
    }
}

template <QuantVariant variant, typename CopyOperationType, typename CopyTraitType, typename DstTensorType,
          typename SrcTensorType, typename QuantTensorType>
void expect_copy_trap(const DstTensorType& dst, const SrcTensorType& src, const QuantTensorType& quant,
                      const FixpipeParams& params = FixpipeParams{})
{
    EXPECT_THROW((copy_with_quant_variant<variant, CopyOperationType, CopyTraitType>(dst, src, quant, params)),
                 TrapException);
}

template <BatchQuantVariant variant, typename CopyOperationType, typename CopyTraitType, typename DstTensorType,
          typename SrcTensorType, typename SharedQuantTensorType, typename BatchedQuantTensorType>
void expect_batch_copy_trap(const DstTensorType& dst, const SrcTensorType& src,
                            const SharedQuantTensorType& shared_quant, const BatchedQuantTensorType& batched_quant)
{
    auto atom = MakeCopy(CopyOperationType{}, CopyTraitType{}).with(FixpipeParams{});
    if constexpr (variant == BatchQuantVariant::NONE) {
        EXPECT_THROW(Copy(atom, dst, src), TrapException);
    } else if constexpr (variant == BatchQuantVariant::SCALAR) {
        uint64_t scalar_quant = 1;
        EXPECT_THROW(Copy(atom, dst, src, scalar_quant), TrapException);
    } else if constexpr (variant == BatchQuantVariant::SHARED_TENSOR) {
        EXPECT_THROW(Copy(atom, dst, src, shared_quant), TrapException);
    } else {
        EXPECT_THROW(Copy(atom, dst, src, batched_quant), TrapException);
    }
}

template <typename DstLayoutPattern, BatchQuantVariant variant>
void expect_gm_batch_capacity_trap()
{
    constexpr uint32_t SRC_BATCH = 2;
    constexpr uint32_t DST_BATCH = 1;
    constexpr uint32_t M = 32;
    constexpr uint32_t N = 32;
    alignas(512) static __cc__ float src_data[SRC_BATCH * M * N] = {};
    alignas(512) static __gm__ float dst_data[DST_BATCH * M * N] = {};
    alignas(512) static __cbuf__ uint64_t shared_quant_data[N] = {};
    alignas(512) static __cbuf__ uint64_t batched_quant_data[SRC_BATCH * N] = {};

    auto src = make_tensor_at<Location::L0C>(
        src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(SRC_BATCH, M, N));
    auto dst = make_tensor_at<Location::GM>(dst_data, MakeFrameLayout<DstLayoutPattern, float>(DST_BATCH, M, N));
    auto shared_quant =
        make_tensor_at<Location::L1>(shared_quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, N));
    auto batched_quant =
        make_tensor_at<Location::L1>(batched_quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(SRC_BATCH, _1{}, N));

    expect_batch_copy_trap<variant, CopyL0C2GM, CopyL0C2GMTraitDefault>(dst, src, shared_quant, batched_quant);
}

template <typename DstLayoutPattern, BatchQuantVariant variant>
void expect_ub_batch_capacity_trap()
{
    constexpr uint32_t SRC_BATCH = 2;
    constexpr uint32_t DST_BATCH = 1;
    constexpr uint32_t M = 32;
    constexpr uint32_t N = 32;
    alignas(512) static __cc__ float src_data[SRC_BATCH * M * N] = {};
    alignas(512) static __ubuf__ float dst_data[DST_BATCH * M * N] = {};
    alignas(512) static __cbuf__ uint64_t shared_quant_data[N] = {};
    alignas(512) static __cbuf__ uint64_t batched_quant_data[SRC_BATCH * N] = {};

    auto src = make_tensor_at<Location::L0C>(
        src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(SRC_BATCH, M, N));
    auto dst = make_tensor_at<Location::UB>(dst_data, MakeFrameLayout<DstLayoutPattern, float>(DST_BATCH, M, N));
    auto shared_quant =
        make_tensor_at<Location::L1>(shared_quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, N));
    auto batched_quant =
        make_tensor_at<Location::L1>(batched_quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(SRC_BATCH, _1{}, N));

    expect_batch_copy_trap<variant, CopyL0C2UB, CopyL0C2UBTraitDefault>(dst, src, shared_quant, batched_quant);
}

template <typename DstLocationType, typename CopyOperationType, typename CopyTraitType, QuantVariant variant,
          InvalidTensorRole role, InvalidLayoutKind kind, typename DstPointerType, typename SrcPointerType,
          typename QuantPointerType>
void expect_l0c_out_invalid_role_trap(DstPointerType dst_data, SrcPointerType src_data, QuantPointerType quant_data)
{
    constexpr int M = 16;
    constexpr int N = 16;
    using SrcLayoutTraitType = LayoutTraitDefault<float, _16>;
    auto valid_dst_layout = MakeFrameLayout<NDExtLayoutPtn, float>(M, N);
    auto valid_src_layout = MakeFrameLayout<NZLayoutPtn, SrcLayoutTraitType>(M, N);
    auto valid_quant_layout = MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, N);

    if constexpr (role == InvalidTensorRole::DST) {
        auto dst = make_tensor_directly<DstLocationType>(dst_data, make_invalid_ndext_layout<float, kind>());
        auto src = make_tensor_at<Location::L0C>(src_data, valid_src_layout);
        auto quant = make_tensor_at<Location::L1>(quant_data, valid_quant_layout);
        expect_copy_trap<variant, CopyOperationType, CopyTraitType>(dst, src, quant);
    } else if constexpr (role == InvalidTensorRole::SRC) {
        auto dst = make_tensor_at<DstLocationType>(dst_data, valid_dst_layout);
        auto src =
            make_tensor_directly<Location::L0C>(src_data, make_invalid_nz_layout<float, kind, SrcLayoutTraitType>());
        auto quant = make_tensor_at<Location::L1>(quant_data, valid_quant_layout);
        expect_copy_trap<variant, CopyOperationType, CopyTraitType>(dst, src, quant);
    } else {
        static_assert(role == InvalidTensorRole::QUANT && variant == QuantVariant::TENSOR);
        auto dst = make_tensor_at<DstLocationType>(dst_data, valid_dst_layout);
        auto src = make_tensor_at<Location::L0C>(src_data, valid_src_layout);
        auto quant = make_tensor_directly<Location::L1>(quant_data, make_invalid_ndext_layout<uint64_t, kind>());
        expect_copy_trap<variant, CopyOperationType, CopyTraitType>(dst, src, quant);
    }
}

template <QuantVariant variant, InvalidTensorRole role, InvalidLayoutKind kind>
void expect_gm_invalid_role_trap()
{
    constexpr int M = 16;
    constexpr int N = 16;
    alignas(512) static __gm__ float dst_data[M * N] = {};
    alignas(512) static __cc__ float src_data[M * N] = {};
    alignas(512) static __cbuf__ uint64_t quant_data[N] = {};
    expect_l0c_out_invalid_role_trap<Location::GM, CopyL0C2GM, CopyL0C2GMTraitDefault, variant, role, kind>(
        dst_data, src_data, quant_data);
}

template <QuantVariant variant, InvalidTensorRole role, InvalidLayoutKind kind>
void expect_ub_invalid_role_trap()
{
    constexpr int M = 16;
    constexpr int N = 16;
    alignas(512) static __ubuf__ float dst_data[M * N] = {};
    alignas(512) static __cc__ float src_data[M * N] = {};
    alignas(512) static __cbuf__ uint64_t quant_data[N] = {};
    expect_l0c_out_invalid_role_trap<Location::UB, CopyL0C2UB, CopyL0C2UBTraitDefault, variant, role, kind>(
        dst_data, src_data, quant_data);
}

template <QuantVariant variant>
void expect_gm_invalid_roles_for_overload()
{
    expect_gm_invalid_role_trap<variant, InvalidTensorRole::DST, InvalidLayoutKind::ZERO_SHAPE>();
    expect_gm_invalid_role_trap<variant, InvalidTensorRole::DST, InvalidLayoutKind::NEGATIVE_SHAPE>();
    expect_gm_invalid_role_trap<variant, InvalidTensorRole::DST, InvalidLayoutKind::STRIDE>();
    expect_gm_invalid_role_trap<variant, InvalidTensorRole::SRC, InvalidLayoutKind::ZERO_SHAPE>();
    expect_gm_invalid_role_trap<variant, InvalidTensorRole::SRC, InvalidLayoutKind::NEGATIVE_SHAPE>();
    expect_gm_invalid_role_trap<variant, InvalidTensorRole::SRC, InvalidLayoutKind::STRIDE>();
    if constexpr (variant == QuantVariant::TENSOR) {
        expect_gm_invalid_role_trap<variant, InvalidTensorRole::QUANT, InvalidLayoutKind::ZERO_SHAPE>();
        expect_gm_invalid_role_trap<variant, InvalidTensorRole::QUANT, InvalidLayoutKind::NEGATIVE_SHAPE>();
        expect_gm_invalid_role_trap<variant, InvalidTensorRole::QUANT, InvalidLayoutKind::STRIDE>();
    }
}

template <QuantVariant variant>
void expect_ub_invalid_roles_for_overload()
{
    expect_ub_invalid_role_trap<variant, InvalidTensorRole::DST, InvalidLayoutKind::ZERO_SHAPE>();
    expect_ub_invalid_role_trap<variant, InvalidTensorRole::DST, InvalidLayoutKind::NEGATIVE_SHAPE>();
    expect_ub_invalid_role_trap<variant, InvalidTensorRole::DST, InvalidLayoutKind::STRIDE>();
    expect_ub_invalid_role_trap<variant, InvalidTensorRole::SRC, InvalidLayoutKind::ZERO_SHAPE>();
    expect_ub_invalid_role_trap<variant, InvalidTensorRole::SRC, InvalidLayoutKind::NEGATIVE_SHAPE>();
    expect_ub_invalid_role_trap<variant, InvalidTensorRole::SRC, InvalidLayoutKind::STRIDE>();
    if constexpr (variant == QuantVariant::TENSOR) {
        expect_ub_invalid_role_trap<variant, InvalidTensorRole::QUANT, InvalidLayoutKind::ZERO_SHAPE>();
        expect_ub_invalid_role_trap<variant, InvalidTensorRole::QUANT, InvalidLayoutKind::NEGATIVE_SHAPE>();
        expect_ub_invalid_role_trap<variant, InvalidTensorRole::QUANT, InvalidLayoutKind::STRIDE>();
    }
}

template <typename DstLayoutPattern>
void expect_gm_zero_dst_stride_trap()
{
    constexpr int M = 16;
    constexpr int N = 16;
    alignas(512) static __cc__ float src_data[M * N] = {};
    alignas(512) static __gm__ float dst_data[M * N] = {};
    alignas(512) static __cbuf__ uint64_t quant_data[N] = {};

    auto src =
        make_tensor_at<Location::L0C>(src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(M, N));
    auto dst = make_tensor_at<Location::GM>(dst_data, make_zero_row_stride_layout<DstLayoutPattern>());
    auto quant = make_tensor_at<Location::L1>(quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, N));

    expect_copy_trap<QuantVariant::NONE, CopyL0C2GM, CopyL0C2GMTraitDefault>(dst, src, quant);
}

template <typename DstLayoutPattern>
void expect_ub_zero_dst_stride_trap()
{
    constexpr int M = 16;
    constexpr int N = 16;
    alignas(512) static __cc__ float src_data[M * N] = {};
    alignas(512) static __ubuf__ float dst_data[M * N] = {};
    alignas(512) static __cbuf__ uint64_t quant_data[N] = {};

    auto src =
        make_tensor_at<Location::L0C>(src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(M, N));
    auto dst = make_tensor_at<Location::UB>(dst_data, make_zero_row_stride_layout<DstLayoutPattern>());
    auto quant = make_tensor_at<Location::L1>(quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, N));

    expect_copy_trap<QuantVariant::NONE, CopyL0C2UB, CopyL0C2UBTraitDefault>(dst, src, quant);
}

template <QuantVariant variant>
void expect_l1_matrix_capacity_trap()
{
    constexpr int SRC_M = 32;
    constexpr int DST_M = 16;
    constexpr int N = 32;
    using LayoutTraitType = LayoutTraitDefault<float, _16>;
    alignas(512) static __cc__ float src_data[SRC_M * N] = {};
    alignas(512) static __cbuf__ float dst_data[DST_M * N] = {};
    alignas(512) static __cbuf__ uint64_t quant_data[N] = {};

    auto src = make_tensor_at<Location::L0C>(src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitType>(SRC_M, N));
    auto dst = make_tensor_at<Location::L1>(dst_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitType>(DST_M, N));
    auto quant = make_tensor_at<Location::L1>(quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, N));

    expect_copy_trap<variant, CopyL0C2L1, CopyL0C2L1TraitDefault>(dst, src, quant);
}

template <QuantVariant variant, InvalidTensorRole role, InvalidLayoutKind kind>
void expect_l1_invalid_role_trap()
{
    constexpr int M = 16;
    constexpr int N = 16;
    using LayoutTraitType = LayoutTraitDefault<float, _16>;
    alignas(512) static __cc__ float src_data[M * N] = {};
    alignas(512) static __cbuf__ float dst_data[M * N] = {};
    alignas(512) static __cbuf__ uint64_t quant_data[N] = {};
    auto valid_src_layout = MakeFrameLayout<NZLayoutPtn, LayoutTraitType>(M, N);
    auto valid_dst_layout = MakeFrameLayout<NZLayoutPtn, LayoutTraitType>(M, N);
    auto valid_quant_layout = MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, N);

    if constexpr (role == InvalidTensorRole::DST) {
        auto dst = make_tensor_directly<Location::L1>(dst_data, make_invalid_nz_layout<float, kind, LayoutTraitType>());
        auto src = make_tensor_at<Location::L0C>(src_data, valid_src_layout);
        auto quant = make_tensor_at<Location::L1>(quant_data, valid_quant_layout);
        expect_copy_trap<variant, CopyL0C2L1, CopyL0C2L1TraitDefault>(dst, src, quant);
    } else if constexpr (role == InvalidTensorRole::SRC) {
        auto dst = make_tensor_at<Location::L1>(dst_data, valid_dst_layout);
        auto src =
            make_tensor_directly<Location::L0C>(src_data, make_invalid_nz_layout<float, kind, LayoutTraitType>());
        auto quant = make_tensor_at<Location::L1>(quant_data, valid_quant_layout);
        expect_copy_trap<variant, CopyL0C2L1, CopyL0C2L1TraitDefault>(dst, src, quant);
    } else {
        static_assert(role == InvalidTensorRole::QUANT && variant == QuantVariant::TENSOR);
        auto dst = make_tensor_at<Location::L1>(dst_data, valid_dst_layout);
        auto src = make_tensor_at<Location::L0C>(src_data, valid_src_layout);
        auto quant = make_tensor_directly<Location::L1>(quant_data, make_invalid_ndext_layout<uint64_t, kind>());
        expect_copy_trap<variant, CopyL0C2L1, CopyL0C2L1TraitDefault>(dst, src, quant);
    }
}

template <QuantVariant variant>
void expect_l1_invalid_roles_for_overload()
{
    expect_l1_invalid_role_trap<variant, InvalidTensorRole::DST, InvalidLayoutKind::ZERO_SHAPE>();
    expect_l1_invalid_role_trap<variant, InvalidTensorRole::DST, InvalidLayoutKind::NEGATIVE_SHAPE>();
    expect_l1_invalid_role_trap<variant, InvalidTensorRole::DST, InvalidLayoutKind::STRIDE>();
    expect_l1_invalid_role_trap<variant, InvalidTensorRole::SRC, InvalidLayoutKind::ZERO_SHAPE>();
    expect_l1_invalid_role_trap<variant, InvalidTensorRole::SRC, InvalidLayoutKind::NEGATIVE_SHAPE>();
    expect_l1_invalid_role_trap<variant, InvalidTensorRole::SRC, InvalidLayoutKind::STRIDE>();
    if constexpr (variant == QuantVariant::TENSOR) {
        expect_l1_invalid_role_trap<variant, InvalidTensorRole::QUANT, InvalidLayoutKind::ZERO_SHAPE>();
        expect_l1_invalid_role_trap<variant, InvalidTensorRole::QUANT, InvalidLayoutKind::NEGATIVE_SHAPE>();
        expect_l1_invalid_role_trap<variant, InvalidTensorRole::QUANT, InvalidLayoutKind::STRIDE>();
    }
}

void expect_l1_n_limit_trap()
{
    constexpr int M = 16;
    constexpr int N = 4096;
    using LayoutTraitType = LayoutTraitDefault<float, _16>;
    alignas(512) static __cc__ float src_data[M * N] = {};
    alignas(512) static __cbuf__ float dst_data[M * N] = {};
    alignas(512) static __cbuf__ uint64_t quant_data[N] = {};
    auto src = make_tensor_at<Location::L0C>(src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitType>(M, N));
    auto dst = make_tensor_at<Location::L1>(dst_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitType>(M, N));
    auto quant = make_tensor_at<Location::L1>(quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, N));

    expect_copy_trap<QuantVariant::NONE, CopyL0C2L1, CopyL0C2L1TraitDefault>(dst, src, quant);
}

void expect_l1_m_limit_trap()
{
    constexpr int M = 65536;
    constexpr int N = 16;
    using LayoutTraitType = LayoutTraitDefault<float, _16>;
    alignas(512) static __cc__ float src_data[M * N] = {};
    alignas(512) static __cbuf__ float dst_data[M * N] = {};
    alignas(512) static __cbuf__ uint64_t quant_data[N] = {};
    auto src = make_tensor_at<Location::L0C>(src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitType>(M, N));
    auto dst = make_tensor_at<Location::L1>(dst_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitType>(M, N));
    auto quant = make_tensor_at<Location::L1>(quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, N));

    expect_copy_trap<QuantVariant::SCALAR, CopyL0C2L1, CopyL0C2L1TraitDefault>(dst, src, quant);
}

void expect_l1_src_stride_limit_trap()
{
    constexpr int BASE_M = 65536;
    constexpr int M = 16;
    constexpr int N = 16;
    using LayoutTraitType = LayoutTraitDefault<float, _16>;
    alignas(512) static __cc__ float src_data[BASE_M * N] = {};
    alignas(512) static __cbuf__ float dst_data[M * N] = {};
    alignas(512) static __cbuf__ uint64_t quant_data[N] = {};
    auto src_base = make_tensor_at<Location::L0C>(src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitType>(BASE_M, N));
    auto src = Slice(src_base, MakeCoord(_0{}, _0{}), MakeShape(M, N));
    auto dst = make_tensor_at<Location::L1>(dst_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitType>(M, N));
    auto quant = make_tensor_at<Location::L1>(quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, N));

    expect_copy_trap<QuantVariant::TENSOR, CopyL0C2L1, CopyL0C2L1TraitDefault>(dst, src, quant);
}

void expect_l1_zero_dst_stride_trap()
{
    constexpr int M = 16;
    constexpr int N = 16;
    using LayoutTraitType = LayoutTraitDefault<float, _16>;
    alignas(512) static __cc__ float src_data[M * N] = {};
    alignas(512) static __cbuf__ float dst_data[M * N] = {};
    alignas(512) static __cbuf__ uint64_t quant_data[N] = {};
    auto src = make_tensor_at<Location::L0C>(src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitType>(M, N));
    auto dst = make_tensor_at<Location::L1>(dst_data, make_zero_nz_column_stride_layout<float, LayoutTraitType>(M, N));
    auto quant = make_tensor_at<Location::L1>(quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, N));

    expect_copy_trap<QuantVariant::NONE, CopyL0C2L1, CopyL0C2L1TraitDefault>(dst, src, quant);
}

void expect_l1_invalid_unit_flag_trap()
{
    constexpr int M = 16;
    constexpr int N = 16;
    using LayoutTraitType = LayoutTraitDefault<float, _16>;
    alignas(512) static __cc__ float src_data[M * N] = {};
    alignas(512) static __cbuf__ float dst_data[M * N] = {};
    alignas(512) static __cbuf__ uint64_t quant_data[N] = {};
    auto src = make_tensor_at<Location::L0C>(src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitType>(M, N));
    auto dst = make_tensor_at<Location::L1>(dst_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitType>(M, N));
    auto quant = make_tensor_at<Location::L1>(quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, N));
    FixpipeParams params{1};

    expect_copy_trap<QuantVariant::NONE, CopyL0C2L1, CopyL0C2L1TraitDefault>(dst, src, quant, params);
}

template <typename DstLayoutPattern, QuantVariant variant>
void expect_gm_matrix_capacity_trap()
{
    constexpr uint32_t SRC_M = 32;
    constexpr uint32_t SRC_N = 32;
    constexpr uint32_t DST_M = 16;
    constexpr uint32_t DST_N = 32;
    alignas(512) static __cc__ float src_data[SRC_M * SRC_N] = {};
    alignas(512) static __gm__ float dst_data[DST_M * DST_N] = {};
    alignas(512) static __cbuf__ uint64_t quant_data[SRC_N] = {};

    auto src = make_tensor_at<Location::L0C>(
        src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(SRC_M, SRC_N));
    auto dst = make_tensor_at<Location::GM>(dst_data, MakeFrameLayout<DstLayoutPattern, float>(DST_M, DST_N));
    auto quant = make_tensor_at<Location::L1>(quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, SRC_N));

    expect_copy_trap<variant, CopyL0C2GM, CopyL0C2GMTraitDefault>(dst, src, quant);
}

template <typename DstLayoutPattern, QuantVariant variant>
void expect_ub_matrix_capacity_trap()
{
    constexpr uint32_t SRC_M = 32;
    constexpr uint32_t SRC_N = 32;
    constexpr uint32_t DST_M = 16;
    constexpr uint32_t DST_N = 32;
    alignas(512) static __cc__ float src_data[SRC_M * SRC_N] = {};
    alignas(512) static __ubuf__ float dst_data[DST_M * DST_N] = {};
    alignas(512) static __cbuf__ uint64_t quant_data[SRC_N] = {};

    auto src = make_tensor_at<Location::L0C>(
        src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(SRC_M, SRC_N));
    auto dst = make_tensor_at<Location::UB>(dst_data, MakeFrameLayout<DstLayoutPattern, float>(DST_M, DST_N));
    auto quant = make_tensor_at<Location::L1>(quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, SRC_N));

    expect_copy_trap<variant, CopyL0C2UB, CopyL0C2UBTraitDefault>(dst, src, quant);
}

template <QuantVariant variant>
void expect_gm_nc1hwc0_capacity_trap()
{
    constexpr uint32_t SRC_M = 32;
    constexpr uint32_t SRC_N = 32;
    constexpr uint32_t DST_C0 = 16;
    constexpr uint32_t DST_SIZE = 16 * DST_C0;
    alignas(512) static __cc__ float src_data[SRC_M * SRC_N] = {};
    alignas(512) static __gm__ float dst_data[DST_SIZE] = {};
    alignas(512) static __cbuf__ uint64_t quant_data[SRC_N] = {};

    auto src = make_tensor_at<Location::L0C>(
        src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(SRC_M, SRC_N));
    auto dst = make_tensor_at<Location::GM>(dst_data,
                                            MakeFrameLayout<NC1HWC0LayoutPtn, float>(_1{}, _1{}, _1{}, _16{}, DST_C0));
    auto quant = make_tensor_at<Location::L1>(quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, SRC_N));

    expect_copy_trap<variant, CopyL0C2GM, CopyL0C2GMTraitDefault>(dst, src, quant);
}

template <typename DstLayoutPattern>
void expect_gm_conv_capacity_trap()
{
    constexpr uint32_t SRC_M = 32;
    constexpr uint32_t SRC_N = 32;
    constexpr uint32_t DST_SIZE = 16 * 16;
    alignas(512) static __cc__ float src_data[SRC_M * SRC_N] = {};
    alignas(512) static __gm__ float dst_data[DST_SIZE] = {};
    alignas(512) static __cbuf__ uint64_t quant_data[SRC_N] = {};

    auto src = make_tensor_at<Location::L0C>(
        src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(SRC_M, SRC_N));
    auto dst =
        make_tensor_at<Location::GM>(dst_data, MakeFrameLayout<DstLayoutPattern, float>(_1{}, _1{}, _16{}, _16{}));
    auto quant = make_tensor_at<Location::L1>(quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, SRC_N));

    expect_copy_trap<QuantVariant::NONE, CopyL0C2GM, CopyL0C2GMTraitDefault>(dst, src, quant);
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2GMNDRejectsInsufficientCapacityForAllQuantVariants)
{
    expect_gm_matrix_capacity_trap<NDLayoutPtn, QuantVariant::NONE>();
    expect_gm_matrix_capacity_trap<NDLayoutPtn, QuantVariant::SCALAR>();
    expect_gm_matrix_capacity_trap<NDLayoutPtn, QuantVariant::TENSOR>();
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2GMNDExtRejectsInsufficientCapacityForAllQuantVariants)
{
    expect_gm_matrix_capacity_trap<NDExtLayoutPtn, QuantVariant::NONE>();
    expect_gm_matrix_capacity_trap<NDExtLayoutPtn, QuantVariant::SCALAR>();
    expect_gm_matrix_capacity_trap<NDExtLayoutPtn, QuantVariant::TENSOR>();
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2GMDNRejectsInsufficientCapacityForAllQuantVariants)
{
    expect_gm_matrix_capacity_trap<DNLayoutPtn, QuantVariant::NONE>();
    expect_gm_matrix_capacity_trap<DNLayoutPtn, QuantVariant::SCALAR>();
    expect_gm_matrix_capacity_trap<DNLayoutPtn, QuantVariant::TENSOR>();
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2GMDNExtRejectsInsufficientCapacityForAllQuantVariants)
{
    expect_gm_matrix_capacity_trap<DNExtLayoutPtn, QuantVariant::NONE>();
    expect_gm_matrix_capacity_trap<DNExtLayoutPtn, QuantVariant::SCALAR>();
    expect_gm_matrix_capacity_trap<DNExtLayoutPtn, QuantVariant::TENSOR>();
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2GMNZRejectsInsufficientCapacityForAllQuantVariants)
{
    expect_gm_matrix_capacity_trap<NZLayoutPtn, QuantVariant::NONE>();
    expect_gm_matrix_capacity_trap<NZLayoutPtn, QuantVariant::SCALAR>();
    expect_gm_matrix_capacity_trap<NZLayoutPtn, QuantVariant::TENSOR>();
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2GMNC1HWC0RejectsInsufficientCapacityForSupportedVariants)
{
    expect_gm_nc1hwc0_capacity_trap<QuantVariant::NONE>();
    expect_gm_nc1hwc0_capacity_trap<QuantVariant::SCALAR>();
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2GMNHWCRejectsInsufficientCapacity)
{
    expect_gm_conv_capacity_trap<NHWCLayoutPtn>();
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2GMNCHWRejectsInsufficientCapacity)
{
    expect_gm_conv_capacity_trap<NCHWLayoutPtn>();
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2UBNDRejectsInsufficientCapacityForAllQuantVariants)
{
    expect_ub_matrix_capacity_trap<NDLayoutPtn, QuantVariant::NONE>();
    expect_ub_matrix_capacity_trap<NDLayoutPtn, QuantVariant::SCALAR>();
    expect_ub_matrix_capacity_trap<NDLayoutPtn, QuantVariant::TENSOR>();
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2UBNDExtRejectsInsufficientCapacityForAllQuantVariants)
{
    expect_ub_matrix_capacity_trap<NDExtLayoutPtn, QuantVariant::NONE>();
    expect_ub_matrix_capacity_trap<NDExtLayoutPtn, QuantVariant::SCALAR>();
    expect_ub_matrix_capacity_trap<NDExtLayoutPtn, QuantVariant::TENSOR>();
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2UBDNRejectsInsufficientCapacityForAllQuantVariants)
{
    expect_ub_matrix_capacity_trap<DNLayoutPtn, QuantVariant::NONE>();
    expect_ub_matrix_capacity_trap<DNLayoutPtn, QuantVariant::SCALAR>();
    expect_ub_matrix_capacity_trap<DNLayoutPtn, QuantVariant::TENSOR>();
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2UBDNExtRejectsInsufficientCapacityForAllQuantVariants)
{
    expect_ub_matrix_capacity_trap<DNExtLayoutPtn, QuantVariant::NONE>();
    expect_ub_matrix_capacity_trap<DNExtLayoutPtn, QuantVariant::SCALAR>();
    expect_ub_matrix_capacity_trap<DNExtLayoutPtn, QuantVariant::TENSOR>();
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2UBNZRejectsInsufficientCapacityForAllQuantVariants)
{
    expect_ub_matrix_capacity_trap<NZLayoutPtn, QuantVariant::NONE>();
    expect_ub_matrix_capacity_trap<NZLayoutPtn, QuantVariant::SCALAR>();
    expect_ub_matrix_capacity_trap<NZLayoutPtn, QuantVariant::TENSOR>();
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2GMBatchRejectsInsufficientCapacityForEveryFormat)
{
    expect_gm_batch_capacity_trap<NDLayoutPtn, BatchQuantVariant::NONE>();
    expect_gm_batch_capacity_trap<NDLayoutPtn, BatchQuantVariant::SCALAR>();
    expect_gm_batch_capacity_trap<NDLayoutPtn, BatchQuantVariant::SHARED_TENSOR>();
    expect_gm_batch_capacity_trap<NDLayoutPtn, BatchQuantVariant::BATCHED_TENSOR>();
    expect_gm_batch_capacity_trap<NDExtLayoutPtn, BatchQuantVariant::NONE>();
    expect_gm_batch_capacity_trap<NDExtLayoutPtn, BatchQuantVariant::SCALAR>();
    expect_gm_batch_capacity_trap<NDExtLayoutPtn, BatchQuantVariant::SHARED_TENSOR>();
    expect_gm_batch_capacity_trap<NDExtLayoutPtn, BatchQuantVariant::BATCHED_TENSOR>();
    expect_gm_batch_capacity_trap<DNLayoutPtn, BatchQuantVariant::NONE>();
    expect_gm_batch_capacity_trap<DNLayoutPtn, BatchQuantVariant::SCALAR>();
    expect_gm_batch_capacity_trap<DNLayoutPtn, BatchQuantVariant::SHARED_TENSOR>();
    expect_gm_batch_capacity_trap<DNLayoutPtn, BatchQuantVariant::BATCHED_TENSOR>();
    expect_gm_batch_capacity_trap<DNExtLayoutPtn, BatchQuantVariant::NONE>();
    expect_gm_batch_capacity_trap<DNExtLayoutPtn, BatchQuantVariant::SCALAR>();
    expect_gm_batch_capacity_trap<DNExtLayoutPtn, BatchQuantVariant::SHARED_TENSOR>();
    expect_gm_batch_capacity_trap<DNExtLayoutPtn, BatchQuantVariant::BATCHED_TENSOR>();
    expect_gm_batch_capacity_trap<NZLayoutPtn, BatchQuantVariant::NONE>();
    expect_gm_batch_capacity_trap<NZLayoutPtn, BatchQuantVariant::SCALAR>();
    expect_gm_batch_capacity_trap<NZLayoutPtn, BatchQuantVariant::SHARED_TENSOR>();
    expect_gm_batch_capacity_trap<NZLayoutPtn, BatchQuantVariant::BATCHED_TENSOR>();
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2UBBatchRejectsInsufficientCapacityForEveryFormat)
{
    expect_ub_batch_capacity_trap<NDLayoutPtn, BatchQuantVariant::NONE>();
    expect_ub_batch_capacity_trap<NDLayoutPtn, BatchQuantVariant::SCALAR>();
    expect_ub_batch_capacity_trap<NDLayoutPtn, BatchQuantVariant::SHARED_TENSOR>();
    expect_ub_batch_capacity_trap<NDLayoutPtn, BatchQuantVariant::BATCHED_TENSOR>();
    expect_ub_batch_capacity_trap<NDExtLayoutPtn, BatchQuantVariant::NONE>();
    expect_ub_batch_capacity_trap<NDExtLayoutPtn, BatchQuantVariant::SCALAR>();
    expect_ub_batch_capacity_trap<NDExtLayoutPtn, BatchQuantVariant::SHARED_TENSOR>();
    expect_ub_batch_capacity_trap<NDExtLayoutPtn, BatchQuantVariant::BATCHED_TENSOR>();
    expect_ub_batch_capacity_trap<DNLayoutPtn, BatchQuantVariant::NONE>();
    expect_ub_batch_capacity_trap<DNLayoutPtn, BatchQuantVariant::SCALAR>();
    expect_ub_batch_capacity_trap<DNLayoutPtn, BatchQuantVariant::SHARED_TENSOR>();
    expect_ub_batch_capacity_trap<DNLayoutPtn, BatchQuantVariant::BATCHED_TENSOR>();
    expect_ub_batch_capacity_trap<DNExtLayoutPtn, BatchQuantVariant::NONE>();
    expect_ub_batch_capacity_trap<DNExtLayoutPtn, BatchQuantVariant::SCALAR>();
    expect_ub_batch_capacity_trap<DNExtLayoutPtn, BatchQuantVariant::SHARED_TENSOR>();
    expect_ub_batch_capacity_trap<DNExtLayoutPtn, BatchQuantVariant::BATCHED_TENSOR>();
    expect_ub_batch_capacity_trap<NZLayoutPtn, BatchQuantVariant::NONE>();
    expect_ub_batch_capacity_trap<NZLayoutPtn, BatchQuantVariant::SCALAR>();
    expect_ub_batch_capacity_trap<NZLayoutPtn, BatchQuantVariant::SHARED_TENSOR>();
    expect_ub_batch_capacity_trap<NZLayoutPtn, BatchQuantVariant::BATCHED_TENSOR>();
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2GMOverloadsRejectInvalidTensorRoles)
{
    expect_gm_invalid_roles_for_overload<QuantVariant::NONE>();
    expect_gm_invalid_roles_for_overload<QuantVariant::SCALAR>();
    expect_gm_invalid_roles_for_overload<QuantVariant::TENSOR>();
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2UBOverloadsRejectInvalidTensorRoles)
{
    expect_ub_invalid_roles_for_overload<QuantVariant::NONE>();
    expect_ub_invalid_roles_for_overload<QuantVariant::SCALAR>();
    expect_ub_invalid_roles_for_overload<QuantVariant::TENSOR>();
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2GMRejectsZeroDestinationStrideForPublicNDLayouts)
{
    expect_gm_zero_dst_stride_trap<NDLayoutPtn>();
    expect_gm_zero_dst_stride_trap<NDExtLayoutPtn>();
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2UBRejectsZeroDestinationStrideForPublicNDLayouts)
{
    expect_ub_zero_dst_stride_trap<NDLayoutPtn>();
    expect_ub_zero_dst_stride_trap<NDExtLayoutPtn>();
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2L1OverloadsRejectInsufficientCapacity)
{
    expect_l1_matrix_capacity_trap<QuantVariant::NONE>();
    expect_l1_matrix_capacity_trap<QuantVariant::SCALAR>();
    expect_l1_matrix_capacity_trap<QuantVariant::TENSOR>();
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2L1OverloadsRejectInvalidTensorRoles)
{
    expect_l1_invalid_roles_for_overload<QuantVariant::NONE>();
    expect_l1_invalid_roles_for_overload<QuantVariant::SCALAR>();
    expect_l1_invalid_roles_for_overload<QuantVariant::TENSOR>();
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2L1RejectsPublicNumericBoundaries)
{
    expect_l1_n_limit_trap();
    expect_l1_m_limit_trap();
    expect_l1_src_stride_limit_trap();
    expect_l1_zero_dst_stride_trap();
    expect_l1_invalid_unit_flag_trap();
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2GMRejectsNAboveInstructionLimitForDirectQuantForms)
{
    constexpr uint32_t M = 16;
    constexpr uint32_t N = 4096;
    alignas(512) static __cc__ float src_data[M * N] = {};
    alignas(512) static __gm__ float dst_data[M * N] = {};
    alignas(512) static __cbuf__ uint64_t quant_data[N] = {};

    auto src =
        make_tensor_at<Location::L0C>(src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(M, N));
    auto dst = make_tensor_at<Location::GM>(dst_data, MakeFrameLayout<NDExtLayoutPtn, float>(M, N));
    auto quant = make_tensor_at<Location::L1>(quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, N));

    expect_copy_trap<QuantVariant::NONE, CopyL0C2GM, CopyL0C2GMTraitDefault>(dst, src, quant);
    expect_copy_trap<QuantVariant::SCALAR, CopyL0C2GM, CopyL0C2GMTraitDefault>(dst, src, quant);
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2UBRejectsNAboveInstructionLimitForDirectQuantForms)
{
    constexpr uint32_t M = 16;
    constexpr uint32_t N = 4096;
    alignas(512) static __cc__ float src_data[M * N] = {};
    alignas(512) static __ubuf__ float dst_data[M * N] = {};
    alignas(512) static __cbuf__ uint64_t quant_data[N] = {};

    auto src =
        make_tensor_at<Location::L0C>(src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(M, N));
    auto dst = make_tensor_at<Location::UB>(dst_data, MakeFrameLayout<DNExtLayoutPtn, float>(M, N));
    auto quant = make_tensor_at<Location::L1>(quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, N));

    expect_copy_trap<QuantVariant::NONE, CopyL0C2UB, CopyL0C2UBTraitDefault>(dst, src, quant);
    expect_copy_trap<QuantVariant::SCALAR, CopyL0C2UB, CopyL0C2UBTraitDefault>(dst, src, quant);
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2GMRejectsNDMAboveLimitWithTensorQuant)
{
    constexpr uint32_t M = 8208;
    constexpr uint32_t N = 16;
    alignas(512) static __cc__ float src_data[M * N] = {};
    alignas(512) static __gm__ float dst_data[M * N] = {};
    alignas(512) static __cbuf__ uint64_t quant_data[N] = {};

    auto src =
        make_tensor_at<Location::L0C>(src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(M, N));
    auto dst = make_tensor_at<Location::GM>(dst_data, MakeFrameLayout<NDExtLayoutPtn, float>(M, N));
    auto quant = make_tensor_at<Location::L1>(quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, N));

    expect_copy_trap<QuantVariant::TENSOR, CopyL0C2GM, CopyL0C2GMTraitDefault>(dst, src, quant);
    expect_copy_trap<QuantVariant::SCALAR, CopyL0C2GM, CopyL0C2GMTraitDefault>(dst, src, quant);
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2UBRejectsNDMAboveLimitWithoutQuant)
{
    constexpr uint32_t M = 8208;
    constexpr uint32_t N = 16;
    alignas(512) static __cc__ float src_data[M * N] = {};
    alignas(512) static __ubuf__ float dst_data[M * N] = {};
    alignas(512) static __cbuf__ uint64_t quant_data[N] = {};

    auto src =
        make_tensor_at<Location::L0C>(src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(M, N));
    auto dst = make_tensor_at<Location::UB>(dst_data, MakeFrameLayout<NDLayoutPtn, float>(M, N));
    auto quant = make_tensor_at<Location::L1>(quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, N));

    expect_copy_trap<QuantVariant::NONE, CopyL0C2UB, CopyL0C2UBTraitDefault>(dst, src, quant);
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2GMRejectsNZMAboveLimitWithScalarQuant)
{
    constexpr uint32_t M = 65536;
    constexpr uint32_t N = 16;
    alignas(512) static __cc__ float src_data[M * N] = {};
    alignas(512) static __gm__ float dst_data[M * N] = {};
    alignas(512) static __cbuf__ uint64_t quant_data[N] = {};

    auto src =
        make_tensor_at<Location::L0C>(src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(M, N));
    auto dst =
        make_tensor_at<Location::GM>(dst_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(M, N));
    auto quant = make_tensor_at<Location::L1>(quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, N));

    expect_copy_trap<QuantVariant::SCALAR, CopyL0C2GM, CopyL0C2GMTraitDefault>(dst, src, quant);
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2UBRejectsNZMAboveLimitWithTensorQuant)
{
    constexpr uint32_t M = 65536;
    constexpr uint32_t N = 16;
    alignas(512) static __cc__ float src_data[M * N] = {};
    alignas(512) static __ubuf__ float dst_data[M * N] = {};
    alignas(512) static __cbuf__ uint64_t quant_data[N] = {};

    auto src =
        make_tensor_at<Location::L0C>(src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(M, N));
    auto dst =
        make_tensor_at<Location::UB>(dst_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(M, N));
    auto quant = make_tensor_at<Location::L1>(quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, N));

    expect_copy_trap<QuantVariant::TENSOR, CopyL0C2UB, CopyL0C2UBTraitDefault>(dst, src, quant);
    expect_copy_trap<QuantVariant::SCALAR, CopyL0C2UB, CopyL0C2UBTraitDefault>(dst, src, quant);
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2GMRejectsNZNNotDivisibleBy16)
{
    constexpr uint32_t SRC_M = 16;
    constexpr uint32_t SRC_N = 16;
    constexpr uint32_t DST_M = 32;
    constexpr uint32_t DST_N = 8;
    alignas(512) __cc__ float src_data[SRC_M * SRC_N] = {};
    alignas(512) __gm__ float dst_data[DST_M * DST_N] = {};
    alignas(512) __cbuf__ uint64_t quant_data[DST_N] = {};

    auto src = make_tensor_at<Location::L0C>(
        src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(SRC_M, SRC_N));
    auto dst = make_tensor_at<Location::GM>(dst_data, MakeFrameLayout<NZLayoutPtn, float>(DST_M, DST_N));
    auto quant = make_tensor_at<Location::L1>(quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, DST_N));

    expect_copy_trap<QuantVariant::NONE, CopyL0C2GM, CopyL0C2GMTraitDefault>(dst, src, quant);
    expect_copy_trap<QuantVariant::SCALAR, CopyL0C2GM, CopyL0C2GMTraitDefault>(dst, src, quant);
    expect_copy_trap<QuantVariant::TENSOR, CopyL0C2GM, CopyL0C2GMTraitDefault>(dst, src, quant);
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2UBRejectsNZNNotDivisibleBy16)
{
    constexpr uint32_t SRC_M = 16;
    constexpr uint32_t SRC_N = 16;
    constexpr uint32_t DST_M = 32;
    constexpr uint32_t DST_N = 8;
    alignas(512) __cc__ float src_data[SRC_M * SRC_N] = {};
    alignas(512) __ubuf__ float dst_data[DST_M * DST_N] = {};
    alignas(512) __cbuf__ uint64_t quant_data[DST_N] = {};

    auto src = make_tensor_at<Location::L0C>(
        src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(SRC_M, SRC_N));
    auto dst = make_tensor_at<Location::UB>(dst_data, MakeFrameLayout<NZLayoutPtn, float>(DST_M, DST_N));
    auto quant = make_tensor_at<Location::L1>(quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, DST_N));

    expect_copy_trap<QuantVariant::NONE, CopyL0C2UB, CopyL0C2UBTraitDefault>(dst, src, quant);
    expect_copy_trap<QuantVariant::SCALAR, CopyL0C2UB, CopyL0C2UBTraitDefault>(dst, src, quant);
    expect_copy_trap<QuantVariant::TENSOR, CopyL0C2UB, CopyL0C2UBTraitDefault>(dst, src, quant);
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2GMRejectsSourceStrideAboveLimit)
{
    constexpr uint32_t BASE_M = 65536;
    constexpr uint32_t M = 16;
    constexpr uint32_t N = 16;
    alignas(512) static __cc__ float src_data[BASE_M * N] = {};
    alignas(512) static __gm__ float dst_data[M * N] = {};
    alignas(512) static __cbuf__ uint64_t quant_data[N] = {};

    auto src_base = make_tensor_at<Location::L0C>(
        src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(BASE_M, N));
    auto src = Slice(src_base, MakeCoord(_0{}, _0{}), MakeShape(M, N));
    auto dst = make_tensor_at<Location::GM>(dst_data, MakeFrameLayout<NDExtLayoutPtn, float>(M, N));
    auto quant = make_tensor_at<Location::L1>(quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, N));

    expect_copy_trap<QuantVariant::SCALAR, CopyL0C2GM, CopyL0C2GMTraitDefault>(dst, src, quant);
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2UBRejectsSourceStrideAboveLimit)
{
    constexpr uint32_t BASE_M = 65536;
    constexpr uint32_t M = 16;
    constexpr uint32_t N = 16;
    alignas(512) static __cc__ float src_data[BASE_M * N] = {};
    alignas(512) static __ubuf__ float dst_data[M * N] = {};
    alignas(512) static __cbuf__ uint64_t quant_data[N] = {};

    auto src_base = make_tensor_at<Location::L0C>(
        src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(BASE_M, N));
    auto src = Slice(src_base, MakeCoord(_0{}, _0{}), MakeShape(M, N));
    auto dst = make_tensor_at<Location::UB>(dst_data, MakeFrameLayout<NDExtLayoutPtn, float>(M, N));
    auto quant = make_tensor_at<Location::L1>(quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, N));

    expect_copy_trap<QuantVariant::TENSOR, CopyL0C2UB, CopyL0C2UBTraitDefault>(dst, src, quant);
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2GMQuantOverloadsRejectInvalidUnitFlag)
{
    constexpr uint32_t M = 16;
    constexpr uint32_t N = 16;
    alignas(512) __cc__ float src_data[M * N] = {};
    alignas(512) __gm__ float dst_data[M * N] = {};
    alignas(512) __cbuf__ uint64_t quant_data[N] = {};
    FixpipeParams params{1};

    auto src =
        make_tensor_at<Location::L0C>(src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(M, N));
    auto dst = make_tensor_at<Location::GM>(dst_data, MakeFrameLayout<NDExtLayoutPtn, float>(M, N));
    auto quant = make_tensor_at<Location::L1>(quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, N));

    expect_copy_trap<QuantVariant::SCALAR, CopyL0C2GM, CopyL0C2GMTraitDefault>(dst, src, quant, params);
    expect_copy_trap<QuantVariant::TENSOR, CopyL0C2GM, CopyL0C2GMTraitDefault>(dst, src, quant, params);
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, CopyL0C2UBQuantOverloadsRejectInvalidUnitFlag)
{
    constexpr uint32_t M = 16;
    constexpr uint32_t N = 16;
    alignas(512) __cc__ float src_data[M * N] = {};
    alignas(512) __ubuf__ float dst_data[M * N] = {};
    alignas(512) __cbuf__ uint64_t quant_data[N] = {};
    FixpipeParams params{1};

    auto src =
        make_tensor_at<Location::L0C>(src_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(M, N));
    auto dst = make_tensor_at<Location::UB>(dst_data, MakeFrameLayout<NDExtLayoutPtn, float>(M, N));
    auto quant = make_tensor_at<Location::L1>(quant_data, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(_1{}, N));

    expect_copy_trap<QuantVariant::SCALAR, CopyL0C2UB, CopyL0C2UBTraitDefault>(dst, src, quant, params);
    expect_copy_trap<QuantVariant::TENSOR, CopyL0C2UB, CopyL0C2UBTraitDefault>(dst, src, quant, params);
}

constexpr MmadTrait GEMV_ENABLED_MMAD_TRAIT = {0, false, false, false, MmadType::NORMAL};

constexpr MmadTrait MX_MMAD_TRAIT = {0, false, false, true, MmadType::MX};

struct GemvEnabledMmadTrait {
    using TraitType = MmadTrait;
    static constexpr const TraitType value = GEMV_ENABLED_MMAD_TRAIT;
};

struct MxMmadTrait {
    using TraitType = MmadTrait;
    static constexpr const TraitType value = MX_MMAD_TRAIT;
};

template <typename BiasLocationType, typename L0CPointerType, typename BiasPointerType>
auto make_valid_mmad_bias(L0CPointerType l0c_pointer, BiasPointerType bias_pointer, int m, int n)
{
    if constexpr (AscendC::Std::is_same_v<BiasLocationType, Location::L0C>) {
        return make_tensor_at<Location::L0C>(l0c_pointer,
                                             MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(m, n));
    } else {
        return make_tensor_at<Location::BIAS>(bias_pointer, MakeFrameLayout<NDLayoutPtn, float>(m, n));
    }
}

template <typename BiasLocationType, InvalidLayoutKind kind, typename L0CPointerType, typename BiasPointerType>
auto make_invalid_mmad_bias(L0CPointerType l0c_pointer, BiasPointerType bias_pointer)
{
    if constexpr (AscendC::Std::is_same_v<BiasLocationType, Location::L0C>) {
        return make_tensor_directly<Location::L0C>(
            l0c_pointer, make_invalid_nz_layout<float, kind, LayoutTraitDefault<float, _16>>());
    } else {
        return make_tensor_directly<Location::BIAS>(bias_pointer, make_invalid_nd_layout<float, kind>());
    }
}

template <typename BiasLocationType, typename MmadTraitType>
void run_mmad_with_bias_at(const MmadParams& params, const MmadTraitType& trait)
{
    constexpr size_t BUFFER_ELEMENTS = 4096 * 16;
    alignas(512) static __ca__ half fm_data[BUFFER_ELEMENTS] = {};
    alignas(512) static __cb__ half filter_data[BUFFER_ELEMENTS] = {};
    alignas(512) static __cc__ float dst_data[BUFFER_ELEMENTS] = {};
    alignas(512) static __cc__ float l0c_bias_data[BUFFER_ELEMENTS] = {};
    alignas(512) static __biasbuf__ float bias_data[BUFFER_ELEMENTS] = {};

    auto fm = make_tensor_at<Location::L0A>(fm_data, MakeFrameLayout<NZLayoutPtn, half>(params.m, params.k));
    auto filter = make_tensor_at<Location::L0B>(filter_data, MakeFrameLayout<ZNLayoutPtn, half>(params.k, params.n));
    auto dst = make_tensor_at<Location::L0C>(
        dst_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(params.m, params.n));
    auto bias = make_valid_mmad_bias<BiasLocationType>(l0c_bias_data, bias_data, params.m, params.n);
    auto atom = MakeMmad(MmadOperation{}, trait).with(params);

    Mmad(atom, dst, fm, filter, bias);
}

void run_normal_mmad_with_default_atom(const MmadParams& params)
{
    constexpr size_t BUFFER_ELEMENTS = 16 * 16;
    alignas(512) static __ca__ half fm_data[BUFFER_ELEMENTS] = {};
    alignas(512) static __cb__ half filter_data[BUFFER_ELEMENTS] = {};
    alignas(512) static __cc__ float dst_data[BUFFER_ELEMENTS] = {};

    auto fm = make_tensor_at<Location::L0A>(fm_data, MakeFrameLayout<NZLayoutPtn, half>(16, 16));
    auto filter = make_tensor_at<Location::L0B>(filter_data, MakeFrameLayout<ZNLayoutPtn, half>(16, 16));
    auto dst =
        make_tensor_at<Location::L0C>(dst_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(16, 16));
    auto atom = MakeMmad(MmadOperation{}).with(params);

    Mmad(atom, dst, fm, filter);
}

void run_mx_mmad(const MmadParams& params)
{
    constexpr size_t BUFFER_ELEMENTS = 4096 * 16;
    alignas(512) static __ca__ fp8_e4m3fn_t fm_data[BUFFER_ELEMENTS] = {};
    alignas(512) static __cb__ fp8_e4m3fn_t filter_data[BUFFER_ELEMENTS] = {};
    alignas(512) static __cc__ float dst_data[BUFFER_ELEMENTS] = {};

    auto fm = make_tensor_at<Location::L0A>(fm_data, MakeFrameLayout<NZLayoutPtn, fp8_e4m3fn_t>(params.m, params.k));
    auto filter =
        make_tensor_at<Location::L0B>(filter_data, MakeFrameLayout<ZNLayoutPtn, fp8_e4m3fn_t>(params.k, params.n));
    auto dst = make_tensor_at<Location::L0C>(
        dst_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(params.m, params.n));
    auto atom = MakeMmad(MmadOperation{}, MxMmadTrait{}).with(params);

    Mmad(atom, dst, fm, filter);
}

template <typename InputType, typename MmadTraitType, InvalidTensorRole role, InvalidLayoutKind kind>
void expect_mmad_no_bias_invalid_role_trap(const MmadTraitType& trait)
{
    constexpr int M = 16;
    constexpr int N = 16;
    constexpr int K = 16;
    alignas(512) static __ca__ InputType fm_data[M * K] = {};
    alignas(512) static __cb__ InputType filter_data[K * N] = {};
    alignas(512) static __cc__ float dst_data[M * N] = {};
    MmadParams params{M, N, K, 0, true};
    auto atom = MakeMmad(MmadOperation{}, trait).with(params);

    if constexpr (role == InvalidTensorRole::DST) {
        auto dst = make_tensor_directly<Location::L0C>(
            dst_data, make_invalid_nz_layout<float, kind, LayoutTraitDefault<float, _16>>());
        auto fm = make_tensor_at<Location::L0A>(fm_data, MakeFrameLayout<NZLayoutPtn, InputType>(M, K));
        auto filter = make_tensor_at<Location::L0B>(filter_data, MakeFrameLayout<ZNLayoutPtn, InputType>(K, N));
        EXPECT_THROW((Mmad(atom, dst, fm, filter)), TrapException);
    } else if constexpr (role == InvalidTensorRole::FM) {
        auto dst =
            make_tensor_at<Location::L0C>(dst_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(M, N));
        auto fm = make_tensor_directly<Location::L0A>(fm_data, make_invalid_nz_layout<InputType, kind>());
        auto filter = make_tensor_at<Location::L0B>(filter_data, MakeFrameLayout<ZNLayoutPtn, InputType>(K, N));
        EXPECT_THROW((Mmad(atom, dst, fm, filter)), TrapException);
    } else {
        static_assert(role == InvalidTensorRole::FILTER);
        auto dst =
            make_tensor_at<Location::L0C>(dst_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(M, N));
        auto fm = make_tensor_at<Location::L0A>(fm_data, MakeFrameLayout<NZLayoutPtn, InputType>(M, K));
        auto filter = make_tensor_directly<Location::L0B>(filter_data, make_invalid_zn_layout<InputType, kind>());
        EXPECT_THROW((Mmad(atom, dst, fm, filter)), TrapException);
    }
}

template <typename InputType, typename MmadTraitType>
void expect_mmad_no_bias_invalid_roles(const MmadTraitType& trait)
{
    expect_mmad_no_bias_invalid_role_trap<InputType, MmadTraitType, InvalidTensorRole::DST,
                                          InvalidLayoutKind::ZERO_SHAPE>(trait);
    expect_mmad_no_bias_invalid_role_trap<InputType, MmadTraitType, InvalidTensorRole::DST,
                                          InvalidLayoutKind::NEGATIVE_SHAPE>(trait);
    expect_mmad_no_bias_invalid_role_trap<InputType, MmadTraitType, InvalidTensorRole::DST, InvalidLayoutKind::STRIDE>(
        trait);
    expect_mmad_no_bias_invalid_role_trap<InputType, MmadTraitType, InvalidTensorRole::FM,
                                          InvalidLayoutKind::ZERO_SHAPE>(trait);
    expect_mmad_no_bias_invalid_role_trap<InputType, MmadTraitType, InvalidTensorRole::FM,
                                          InvalidLayoutKind::NEGATIVE_SHAPE>(trait);
    expect_mmad_no_bias_invalid_role_trap<InputType, MmadTraitType, InvalidTensorRole::FM, InvalidLayoutKind::STRIDE>(
        trait);
    expect_mmad_no_bias_invalid_role_trap<InputType, MmadTraitType, InvalidTensorRole::FILTER,
                                          InvalidLayoutKind::ZERO_SHAPE>(trait);
    expect_mmad_no_bias_invalid_role_trap<InputType, MmadTraitType, InvalidTensorRole::FILTER,
                                          InvalidLayoutKind::NEGATIVE_SHAPE>(trait);
    expect_mmad_no_bias_invalid_role_trap<InputType, MmadTraitType, InvalidTensorRole::FILTER,
                                          InvalidLayoutKind::STRIDE>(trait);
}

template <typename BiasLocationType, InvalidTensorRole role, InvalidLayoutKind kind>
void expect_mmad_with_bias_invalid_role_trap()
{
    constexpr int M = 16;
    constexpr int N = 16;
    constexpr int K = 16;
    alignas(512) static __ca__ half fm_data[M * K] = {};
    alignas(512) static __cb__ half filter_data[K * N] = {};
    alignas(512) static __cc__ float dst_data[M * N] = {};
    alignas(512) static __cc__ float l0c_bias_data[M * N] = {};
    alignas(512) static __biasbuf__ float bias_data[M * N] = {};
    MmadParams params{M, N, K, 0, false};
    auto atom = MakeMmad(MmadOperation{}, MmadTraitDefault{}).with(params);

    if constexpr (role == InvalidTensorRole::DST) {
        auto dst = make_tensor_directly<Location::L0C>(
            dst_data, make_invalid_nz_layout<float, kind, LayoutTraitDefault<float, _16>>());
        auto fm = make_tensor_at<Location::L0A>(fm_data, MakeFrameLayout<NZLayoutPtn, half>(M, K));
        auto filter = make_tensor_at<Location::L0B>(filter_data, MakeFrameLayout<ZNLayoutPtn, half>(K, N));
        auto bias = make_valid_mmad_bias<BiasLocationType>(l0c_bias_data, bias_data, M, N);
        EXPECT_THROW((Mmad(atom, dst, fm, filter, bias)), TrapException);
    } else if constexpr (role == InvalidTensorRole::FM) {
        auto dst =
            make_tensor_at<Location::L0C>(dst_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(M, N));
        auto fm = make_tensor_directly<Location::L0A>(fm_data, make_invalid_nz_layout<half, kind>());
        auto filter = make_tensor_at<Location::L0B>(filter_data, MakeFrameLayout<ZNLayoutPtn, half>(K, N));
        auto bias = make_valid_mmad_bias<BiasLocationType>(l0c_bias_data, bias_data, M, N);
        EXPECT_THROW((Mmad(atom, dst, fm, filter, bias)), TrapException);
    } else if constexpr (role == InvalidTensorRole::FILTER) {
        auto dst =
            make_tensor_at<Location::L0C>(dst_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(M, N));
        auto fm = make_tensor_at<Location::L0A>(fm_data, MakeFrameLayout<NZLayoutPtn, half>(M, K));
        auto filter = make_tensor_directly<Location::L0B>(filter_data, make_invalid_zn_layout<half, kind>());
        auto bias = make_valid_mmad_bias<BiasLocationType>(l0c_bias_data, bias_data, M, N);
        EXPECT_THROW((Mmad(atom, dst, fm, filter, bias)), TrapException);
    } else {
        static_assert(role == InvalidTensorRole::BIAS);
        auto dst =
            make_tensor_at<Location::L0C>(dst_data, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(M, N));
        auto fm = make_tensor_at<Location::L0A>(fm_data, MakeFrameLayout<NZLayoutPtn, half>(M, K));
        auto filter = make_tensor_at<Location::L0B>(filter_data, MakeFrameLayout<ZNLayoutPtn, half>(K, N));
        auto bias = make_invalid_mmad_bias<BiasLocationType, kind>(l0c_bias_data, bias_data);
        EXPECT_THROW((Mmad(atom, dst, fm, filter, bias)), TrapException);
    }
}

template <typename BiasLocationType>
void expect_mmad_with_bias_invalid_roles()
{
    expect_mmad_with_bias_invalid_role_trap<BiasLocationType, InvalidTensorRole::DST, InvalidLayoutKind::ZERO_SHAPE>();
    expect_mmad_with_bias_invalid_role_trap<BiasLocationType, InvalidTensorRole::DST,
                                            InvalidLayoutKind::NEGATIVE_SHAPE>();
    expect_mmad_with_bias_invalid_role_trap<BiasLocationType, InvalidTensorRole::DST, InvalidLayoutKind::STRIDE>();
    expect_mmad_with_bias_invalid_role_trap<BiasLocationType, InvalidTensorRole::FM, InvalidLayoutKind::ZERO_SHAPE>();
    expect_mmad_with_bias_invalid_role_trap<BiasLocationType, InvalidTensorRole::FM,
                                            InvalidLayoutKind::NEGATIVE_SHAPE>();
    expect_mmad_with_bias_invalid_role_trap<BiasLocationType, InvalidTensorRole::FM, InvalidLayoutKind::STRIDE>();
    expect_mmad_with_bias_invalid_role_trap<BiasLocationType, InvalidTensorRole::FILTER,
                                            InvalidLayoutKind::ZERO_SHAPE>();
    expect_mmad_with_bias_invalid_role_trap<BiasLocationType, InvalidTensorRole::FILTER,
                                            InvalidLayoutKind::NEGATIVE_SHAPE>();
    expect_mmad_with_bias_invalid_role_trap<BiasLocationType, InvalidTensorRole::FILTER, InvalidLayoutKind::STRIDE>();
    expect_mmad_with_bias_invalid_role_trap<BiasLocationType, InvalidTensorRole::BIAS, InvalidLayoutKind::ZERO_SHAPE>();
    expect_mmad_with_bias_invalid_role_trap<BiasLocationType, InvalidTensorRole::BIAS,
                                            InvalidLayoutKind::NEGATIVE_SHAPE>();
    expect_mmad_with_bias_invalid_role_trap<BiasLocationType, InvalidTensorRole::BIAS, InvalidLayoutKind::STRIDE>();
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, MmadNormalDefaultAtomRejectsInvalidUnitFlag)
{
    MmadParams params{16, 16, 16, 1, true};

    EXPECT_THROW((run_normal_mmad_with_default_atom(params)), TrapException);
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, MmadMxRejectsDimensionsAboveLimit)
{
    MmadParams invalid_m{4096, 16, 16, 0, true};
    MmadParams invalid_n{16, 4096, 16, 0, true};
    MmadParams invalid_k{16, 16, 4096, 0, true};

    EXPECT_THROW((run_mx_mmad(invalid_m)), TrapException);
    EXPECT_THROW((run_mx_mmad(invalid_n)), TrapException);
    EXPECT_THROW((run_mx_mmad(invalid_k)), TrapException);
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, MmadMxRejectsInvalidUnitFlag)
{
    MmadParams params{16, 16, 16, 1, true};

    EXPECT_THROW((run_mx_mmad(params)), TrapException);
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, MmadNormalAndMxRejectInvalidOperandLayouts)
{
    expect_mmad_no_bias_invalid_roles<half>(MmadTraitDefault{});
    expect_mmad_no_bias_invalid_roles<fp8_e4m3fn_t>(MxMmadTrait{});
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, MmadWithBiasRejectsInvalidOperandLayouts)
{
    expect_mmad_with_bias_invalid_roles<Location::BIAS>();
    expect_mmad_with_bias_invalid_roles<Location::L0C>();
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, MmadWithBiasRejectsDimensionAboveLimit)
{
    MmadParams invalid_m{4096, 16, 16, 0, false};
    MmadParams invalid_n{16, 4096, 16, 0, false};
    MmadParams invalid_k{16, 16, 4096, 0, false};

    EXPECT_THROW((run_mmad_with_bias_at<Location::BIAS>(invalid_m, MmadTraitDefault{})), TrapException);
    EXPECT_THROW((run_mmad_with_bias_at<Location::BIAS>(invalid_n, MmadTraitDefault{})), TrapException);
    EXPECT_THROW((run_mmad_with_bias_at<Location::BIAS>(invalid_k, MmadTraitDefault{})), TrapException);
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, MmadWithBiasRejectsInvalidUnitFlag)
{
    MmadParams params{16, 16, 16, 1, false};

    EXPECT_THROW((run_mmad_with_bias_at<Location::BIAS>(params, MmadTraitDefault{})), TrapException);
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, MmadWithBiasRejectsMatrixMAboveOneInGemvMode)
{
    MmadParams params{16, 16, 16, 0, false};

    EXPECT_THROW((run_mmad_with_bias_at<Location::BIAS>(params, GemvEnabledMmadTrait{})), TrapException);
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, MmadWithL0CBiasRejectsDimensionAboveLimit)
{
    MmadParams invalid_m{4096, 16, 16, 0, false};
    MmadParams invalid_n{16, 4096, 16, 0, false};
    MmadParams invalid_k{16, 16, 4096, 0, false};

    EXPECT_THROW((run_mmad_with_bias_at<Location::L0C>(invalid_m, MmadTraitDefault{})), TrapException);
    EXPECT_THROW((run_mmad_with_bias_at<Location::L0C>(invalid_n, MmadTraitDefault{})), TrapException);
    EXPECT_THROW((run_mmad_with_bias_at<Location::L0C>(invalid_k, MmadTraitDefault{})), TrapException);
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, MmadWithL0CBiasRejectsInvalidUnitFlag)
{
    MmadParams params{16, 16, 16, 1, false};

    EXPECT_THROW((run_mmad_with_bias_at<Location::L0C>(params, MmadTraitDefault{})), TrapException);
}

TEST_F(TensorApiCubeOutAdditionalRuntimeValidation, MmadWithL0CBiasRejectsMatrixMAboveOneInGemvMode)
{
    MmadParams params{16, 16, 16, 0, false};

    EXPECT_THROW((run_mmad_with_bias_at<Location::L0C>(params, GemvEnabledMmadTrait{})), TrapException);
}

} // namespace
