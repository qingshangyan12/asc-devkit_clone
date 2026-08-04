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

class TensorApiCubeRuntimeValidation : public testing::Test {
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

constexpr AscendC::Te::MmadTrait GEMV_ENABLED_MMAD_TRAIT = {0, false, false, false, AscendC::Te::MmadType::NORMAL};

struct GemvEnabledMmadTrait {
    using TraitType = AscendC::Te::MmadTrait;
    static constexpr const TraitType value = GEMV_ENABLED_MMAD_TRAIT;
};

template <typename MmadTraitType>
void run_mmad(const AscendC::Te::MmadParams& params, const MmadTraitType& trait)
{
    using namespace AscendC::Te;

    constexpr size_t MMAD_TEST_BUFFER_ELEMENTS = 4096 * 16;
    alignas(512) static __ca__ half fm_data[MMAD_TEST_BUFFER_ELEMENTS] = {};
    alignas(512) static __cb__ half filter_data[MMAD_TEST_BUFFER_ELEMENTS] = {};
    alignas(512) static __cc__ float dst_data[MMAD_TEST_BUFFER_ELEMENTS] = {};
    auto fm = make_tensor_at<Location::L0A>(fm_data, MakeFrameLayout<NZLayoutPtn, half>(params.m, params.k));
    auto filter = make_tensor_at<Location::L0B>(filter_data, MakeFrameLayout<ZNLayoutPtn, half>(params.k, params.n));
    auto dst = make_tensor_at<Location::L0C>(dst_data, MakeFrameLayout<NZLayoutPtn, float>(params.m, params.n));
    auto atom = MakeMmad(MmadOperation{}, trait).with(params);

    Mmad(atom, dst, fm, filter);
}

void run_img2col(const AscendC::Te::Img2ColParams<int16_t>& params)
{
    using namespace AscendC::Te;

    constexpr int C0 = 16;
    alignas(512) static __cbuf__ int16_t src[1 * 2 * 5 * 5 * C0] = {};
    alignas(512) static __ca__ int16_t dst[32 * 288] = {};
    auto src_tensor = make_tensor_at<Location::L1>(src, MakeFrameLayout<NC1HWC0LayoutPtn>(1, 2, 5, 5, C0));
    auto dst_tensor = make_tensor_at<Location::L0A>(dst, MakeFrameLayout<NZLayoutPtn, int16_t>(32, 288));

    copy_tensor_with<CopyL12L0A>(dst_tensor, src_tensor, params);
}

TEST_F(TensorApiCubeRuntimeValidation, CopyGM2L1RejectsDestinationSmallerThanCopyData)
{
    using namespace AscendC::Te;

    alignas(512) __gm__ int8_t src[32 * 32] = {};
    alignas(512) __cbuf__ int8_t dst[16 * 32] = {};
    auto src_tensor = make_tensor_at<Location::GM>(src, MakeFrameLayout<NDExtLayoutPtn, int8_t>(32, 32));
    auto dst_tensor = make_tensor_at<Location::L1>(dst, MakeFrameLayout<NDExtLayoutPtn, int8_t>(16, 32));

    EXPECT_THROW((copy_tensor<CopyGM2L1, CopyGM2L1TraitDefault>(dst_tensor, src_tensor)), TrapException);
}

TEST_F(TensorApiCubeRuntimeValidation, CopyL12BTRejectsDestinationSmallerThanCopyData)
{
    using namespace AscendC::Te;

    alignas(512) __cbuf__ float src[16 * 64] = {};
    alignas(512) __biasbuf__ float dst[8 * 64] = {};
    auto src_tensor = make_tensor_at<Location::L1>(src, MakeFrameLayout<NDExtLayoutPtn, float>(16, 64));
    auto dst_tensor = make_tensor_at<Location::BIAS>(dst, MakeFrameLayout<NDExtLayoutPtn, float>(8, 64));

    EXPECT_THROW((copy_tensor<CopyL12BT, CopyL12BTTraitDefault>(dst_tensor, src_tensor)), TrapException);
}

TEST_F(TensorApiCubeRuntimeValidation, CopyL12FBRejectsDestinationSmallerThanCopyData)
{
    using namespace AscendC::Te;

    alignas(512) __cbuf__ uint64_t src[8 * 64] = {};
    alignas(512) __fbuf__ uint64_t dst[4 * 64] = {};
    auto src_tensor = make_tensor_at<Location::L1>(src, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(8, 64));
    auto dst_tensor = make_tensor_at<Location::FIXBUF>(dst, MakeFrameLayout<NDExtLayoutPtn, uint64_t>(4, 64));

    EXPECT_THROW((copy_tensor<CopyL12FB, CopyL12FBTraitDefault>(dst_tensor, src_tensor)), TrapException);
}

TEST_F(TensorApiCubeRuntimeValidation, CopyL12L0ARejectsDestinationSmallerThanCopyData)
{
    using namespace AscendC::Te;

    alignas(512) __cbuf__ float src[32 * 32] = {};
    alignas(512) __ca__ float dst[16 * 32] = {};
    auto src_tensor = make_tensor_at<Location::L1>(src, MakeFrameLayout<NZLayoutPtn, float>(32, 32));
    auto dst_tensor = make_tensor_at<Location::L0A>(dst, MakeFrameLayout<NZLayoutPtn, float>(16, 32));

    EXPECT_THROW((copy_tensor<CopyL12L0A, CopyL12L0ATraitDefault>(dst_tensor, src_tensor)), TrapException);
}

TEST_F(TensorApiCubeRuntimeValidation, CopyL12L0BRejectsDestinationSmallerThanCopyData)
{
    using namespace AscendC::Te;

    alignas(512) __cbuf__ float src[32 * 32] = {};
    alignas(512) __cb__ float dst[16 * 32] = {};
    auto src_tensor = make_tensor_at<Location::L1>(src, MakeFrameLayout<NZLayoutPtn, float>(32, 32));
    auto dst_tensor = make_tensor_at<Location::L0B>(dst, MakeFrameLayout<ZNLayoutPtn, float>(16, 32));

    EXPECT_THROW((copy_tensor<CopyL12L0B, CopyL12L0BTraitDefault>(dst_tensor, src_tensor)), TrapException);
}

TEST_F(TensorApiCubeRuntimeValidation, CopyL12L0ScaleARejectsDestinationSmallerThanCopyData)
{
    using namespace AscendC::Te;

    alignas(512) __cbuf__ fp8_e8m0_t src[32 * 32] = {};
    alignas(512) __ca__ fp8_e8m0_t dst[16 * 32] = {};
    auto src_tensor = make_tensor_at<Location::L1>(src, MakeFrameLayout<ZZLayoutPtn, AscendC::Std::Int<2>>(32, 32));
    auto dst_pointer = MakeMemPtr<Location::L0ScaleA, fp8_e8m0_t>(reinterpret_cast<uint64_t>(dst) / 16);
    auto dst_tensor = MakeTensor(dst_pointer, MakeFrameLayout<ZZLayoutPtn, AscendC::Std::Int<2>>(16, 32));

    EXPECT_THROW((copy_tensor<CopyL12L0ScaleA, CopyL12L0ScaleATraitDefault>(dst_tensor, src_tensor)), TrapException);
}

TEST_F(TensorApiCubeRuntimeValidation, CopyL12L0ScaleBRejectsDestinationSmallerThanCopyData)
{
    using namespace AscendC::Te;

    alignas(512) __cbuf__ fp8_e8m0_t src[32 * 32] = {};
    alignas(512) __cb__ fp8_e8m0_t dst[16 * 32] = {};
    auto src_tensor = make_tensor_at<Location::L1>(src, MakeFrameLayout<NNLayoutPtn, AscendC::Std::Int<2>>(32, 32));
    auto dst_pointer = MakeMemPtr<Location::L0ScaleB, fp8_e8m0_t>(reinterpret_cast<uint64_t>(dst) / 16);
    auto dst_tensor = MakeTensor(dst_pointer, MakeFrameLayout<NNLayoutPtn, AscendC::Std::Int<2>>(16, 32));

    EXPECT_THROW((copy_tensor<CopyL12L0ScaleB, CopyL12L0ScaleBTraitDefault>(dst_tensor, src_tensor)), TrapException);
}

TEST_F(TensorApiCubeRuntimeValidation, CopyL0C2GMRejectsDestinationSmallerThanCopyData)
{
    using namespace AscendC::Te;

    alignas(512) __cc__ float src[32 * 32] = {};
    alignas(512) __gm__ float dst[16 * 32] = {};
    auto src_tensor =
        make_tensor_at<Location::L0C>(src, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(32, 32));
    auto dst_tensor = make_tensor_at<Location::GM>(dst, MakeFrameLayout<NDExtLayoutPtn, float>(16, 32));

    EXPECT_THROW((copy_tensor<CopyL0C2GM, CopyL0C2GMTraitDefault>(dst_tensor, src_tensor)), TrapException);
}

TEST_F(TensorApiCubeRuntimeValidation, CopyL0C2UBRejectsDestinationSmallerThanCopyData)
{
    using namespace AscendC::Te;

    alignas(512) __cc__ float src[32 * 32] = {};
    alignas(512) __ubuf__ float dst[16 * 32] = {};
    auto src_tensor =
        make_tensor_at<Location::L0C>(src, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(32, 32));
    auto dst_tensor = make_tensor_at<Location::UB>(dst, MakeFrameLayout<NDExtLayoutPtn, float>(16, 32));

    EXPECT_THROW((copy_tensor<CopyL0C2UB, CopyL0C2UBTraitDefault>(dst_tensor, src_tensor)), TrapException);
}

TEST_F(TensorApiCubeRuntimeValidation, CopyGM2L1RejectsBatchCountAboveLimit)
{
    using namespace AscendC::Te;

    int batch = 4096;
    alignas(512) static __gm__ int8_t src[4096 * 1 * 32] = {};
    alignas(512) static __cbuf__ int8_t dst[4096 * 1 * 32] = {};
    auto src_tensor = make_tensor_at<Location::GM>(src, MakeFrameLayout<NDExtLayoutPtn, int8_t>(batch, 1, 32));
    auto dst_tensor = make_tensor_at<Location::L1>(dst, MakeFrameLayout<NDExtLayoutPtn, int8_t>(batch, 1, 32));

    EXPECT_THROW((copy_tensor<CopyGM2L1, CopyGM2L1TraitDefault>(dst_tensor, src_tensor)), TrapException);
}

TEST_F(TensorApiCubeRuntimeValidation, CopyL12L0ARejectsMismatchedBatchCounts)
{
    using namespace AscendC::Te;

    int src_batch = 1;
    int dst_batch = 2;
    alignas(512) __cbuf__ float src[1 * 32 * 32] = {};
    alignas(512) __ca__ float dst[2 * 32 * 32] = {};
    auto src_tensor = make_tensor_at<Location::L1>(src, MakeFrameLayout<NZLayoutPtn, float>(src_batch, 32, 32));
    auto dst_tensor = make_tensor_at<Location::L0A>(dst, MakeFrameLayout<NZLayoutPtn, float>(dst_batch, 32, 32));

    EXPECT_THROW((copy_tensor<CopyL12L0A, CopyL12L0ATraitDefault>(dst_tensor, src_tensor)), TrapException);
}

TEST_F(TensorApiCubeRuntimeValidation, CopyL12L0BRejectsMismatchedBatchCounts)
{
    using namespace AscendC::Te;

    int src_batch = 1;
    int dst_batch = 2;
    alignas(512) __cbuf__ float src[1 * 32 * 32] = {};
    alignas(512) __cb__ float dst[2 * 32 * 32] = {};
    auto src_tensor = make_tensor_at<Location::L1>(src, MakeFrameLayout<ZNLayoutPtn, float>(src_batch, 32, 32));
    auto dst_tensor = make_tensor_at<Location::L0B>(dst, MakeFrameLayout<ZNLayoutPtn, float>(dst_batch, 32, 32));

    EXPECT_THROW((copy_tensor<CopyL12L0B, CopyL12L0BTraitDefault>(dst_tensor, src_tensor)), TrapException);
}

TEST_F(TensorApiCubeRuntimeValidation, CopyL12BTRejectsUnalignedBatchColumnSize)
{
    using namespace AscendC::Te;

    int batch = 8;
    int columns = 62;
    alignas(512) __cbuf__ float src[8 * 1 * 62] = {};
    alignas(512) __biasbuf__ float dst[8 * 1 * 62] = {};
    auto src_tensor = make_tensor_at<Location::L1>(src, MakeFrameLayout<NDLayoutPtn, float>(batch, 1, columns));
    auto dst_tensor = make_tensor_at<Location::BIAS>(dst, MakeFrameLayout<NDLayoutPtn, float>(batch, 1, columns));

    EXPECT_THROW((copy_tensor<CopyL12BT, CopyL12BTTraitDefault>(dst_tensor, src_tensor)), TrapException);
}

TEST_F(TensorApiCubeRuntimeValidation, CopyL12L0ARejectsInvalidImg2ColStartPoint)
{
    using namespace AscendC::Te;

    Img2ColParams<int16_t> params;
    params.mExtension = 32;
    params.kExtension = 288;
    params.mStartPt = 32768;

    EXPECT_THROW((run_img2col(params)), TrapException);
}

TEST_F(TensorApiCubeRuntimeValidation, CopyL12L0ARejectsMisalignedImg2ColKStartPoint)
{
    using namespace AscendC::Te;

    Img2ColParams<int16_t> params;
    params.mExtension = 32;
    params.kExtension = 288;
    params.kStartPt = 8;

    EXPECT_THROW((run_img2col(params)), TrapException);
}

TEST_F(TensorApiCubeRuntimeValidation, CopyL0C2GMRejectsInvalidUnitFlag)
{
    using namespace AscendC::Te;

    alignas(512) __cc__ float src[32 * 32] = {};
    alignas(512) __gm__ float dst[32 * 32] = {};
    auto src_tensor =
        make_tensor_at<Location::L0C>(src, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(32, 32));
    auto dst_tensor = make_tensor_at<Location::GM>(dst, MakeFrameLayout<NDExtLayoutPtn, float>(32, 32));
    FixpipeParams params;
    params.unitFlag = 1;

    EXPECT_THROW((copy_tensor_with<CopyL0C2GM>(dst_tensor, src_tensor, params)), TrapException);
}

TEST_F(TensorApiCubeRuntimeValidation, CopyL0C2UBRejectsInvalidUnitFlag)
{
    using namespace AscendC::Te;

    alignas(512) __cc__ float src[32 * 32] = {};
    alignas(512) __ubuf__ float dst[32 * 32] = {};
    auto src_tensor =
        make_tensor_at<Location::L0C>(src, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(32, 32));
    auto dst_tensor = make_tensor_at<Location::UB>(dst, MakeFrameLayout<NDExtLayoutPtn, float>(32, 32));
    FixpipeParams params;
    params.unitFlag = 1;

    EXPECT_THROW((copy_tensor_with<CopyL0C2UB>(dst_tensor, src_tensor, params)), TrapException);
}

TEST_F(TensorApiCubeRuntimeValidation, MmadRejectsDimensionAboveLimit)
{
    using namespace AscendC::Te;

    MmadParams invalid_m{4096, 16, 16, 0, true};
    MmadParams invalid_n{16, 4096, 16, 0, true};
    MmadParams invalid_k{16, 16, 4096, 0, true};

    EXPECT_THROW((run_mmad(invalid_m, MmadTraitDefault{})), TrapException);
    EXPECT_THROW((run_mmad(invalid_n, MmadTraitDefault{})), TrapException);
    EXPECT_THROW((run_mmad(invalid_k, MmadTraitDefault{})), TrapException);
}

TEST_F(TensorApiCubeRuntimeValidation, MmadRejectsInvalidUnitFlag)
{
    using namespace AscendC::Te;

    MmadParams params{16, 16, 16, 1, true};

    EXPECT_THROW((run_mmad(params, MmadTraitDefault{})), TrapException);
}

TEST_F(TensorApiCubeRuntimeValidation, MmadRejectsMatrixMAboveOneInGemvMode)
{
    using namespace AscendC::Te;

    MmadParams params{16, 16, 16, 0, true};

    EXPECT_THROW((run_mmad(params, GemvEnabledMmadTrait{})), TrapException);
}

} // namespace
