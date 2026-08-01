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

class Tensor_Api_Frame_Layout : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    void TearDown() {}
};

TEST_F(Tensor_Api_Frame_Layout, NDAndDNGroupDefaultTrait)
{
    using namespace AscendC::Te;

    auto ndLayout = make_frame_layout<NDLayoutPtn>(8, 16);
    auto dnLayout = make_frame_layout<DNLayoutPtn>(8, 16);
    auto ndExtLayout = make_frame_layout<NDExtLayoutPtn>(8, 16);
    auto dnExtLayout = make_frame_layout<DNExtLayoutPtn>(8, 16);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape(ndLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape(ndLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(ndLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_stride(ndLayout)), 1);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape(dnLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape(dnLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(dnLayout)), 1);
    EXPECT_EQ(AscendC::Std::get<1>(get_stride(dnLayout)), 8);

    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<0>(get_shape(ndExtLayout))), 1);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_shape(ndExtLayout))), 8);
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<1>(get_shape(ndExtLayout))), 1);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_shape(ndExtLayout))), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_stride(ndExtLayout))), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_stride(ndExtLayout))), 1);

    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<0>(get_shape(dnExtLayout))), 1);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_shape(dnExtLayout))), 8);
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<1>(get_shape(dnExtLayout))), 1);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_shape(dnExtLayout))), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_stride(dnExtLayout))), 1);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_stride(dnExtLayout))), 8);
}

TEST_F(Tensor_Api_Frame_Layout, BatchFrameLayoutDefaultTrait)
{
    using namespace AscendC::Te;

    auto ndLayout = make_frame_layout<NDLayoutPtn>(2, 8, 16);
    auto dnLayout = make_frame_layout<DNLayoutPtn>(2, 8, 16);
    auto ndExtLayout = make_frame_layout<NDExtLayoutPtn>(2, 8, 16);
    auto dnExtLayout = make_frame_layout<DNExtLayoutPtn>(2, 8, 16);
    auto nzLayout = make_frame_layout<NZLayoutPtn>(2, 32, 64);
    auto znLayout = make_frame_layout<ZNLayoutPtn>(2, 32, 64);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape(ndLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(ndLayout)), 128);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(ndLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(ndLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride<1>(ndLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_stride<1>(ndLayout)), 1);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape(dnLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(dnLayout)), 128);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(dnLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(dnLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride<1>(dnLayout)), 1);
    EXPECT_EQ(AscendC::Std::get<1>(get_stride<1>(dnLayout)), 8);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape(ndExtLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(ndExtLayout)), 128);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_shape<1>(ndExtLayout))), 8);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_shape<1>(ndExtLayout))), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_stride<1>(ndExtLayout))), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_stride<1>(ndExtLayout))), 1);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape(dnExtLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(dnExtLayout)), 128);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_shape<1>(dnExtLayout))), 8);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_shape<1>(dnExtLayout))), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_stride<1>(dnExtLayout))), 1);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_stride<1>(dnExtLayout))), 8);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape(nzLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(nzLayout)), 2048);
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<0>(get_shape<1>(nzLayout))), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_shape<1>(nzLayout))), 2);
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<1>(get_shape<1>(nzLayout))), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_shape<1>(nzLayout))), 4);
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<0>(get_stride<1>(nzLayout))), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_stride<1>(nzLayout))), 256);
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<1>(get_stride<1>(nzLayout))), 1);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_stride<1>(nzLayout))), 512);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape(znLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(znLayout)), 2048);
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<0>(get_shape<1>(znLayout))), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_shape<1>(znLayout))), 2);
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<1>(get_shape<1>(znLayout))), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_shape<1>(znLayout))), 4);
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<0>(get_stride<1>(znLayout))), 1);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_stride<1>(znLayout))), 1024);
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<1>(get_stride<1>(znLayout))), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_stride<1>(znLayout))), 256);
}

TEST_F(Tensor_Api_Frame_Layout, BatchFrameLayoutTraitForms)
{
    using namespace AscendC::Te;

    auto ndLayout = make_frame_layout<NDLayoutPtn, LayoutTraitDefault<float>>(2, 8, 16);
    auto dnLayout = make_frame_layout<DNLayoutPtn, float>(2, 8, 16);
    auto ndExtLayout = make_frame_layout<NDExtLayoutPtn, 8>(2, 8, 16);
    auto dnExtLayout = make_frame_layout<DNExtLayoutPtn, LayoutTraitDefault<float>>(2, 8, 16);
    auto nzLayoutWithTrait = make_frame_layout<NZLayoutPtn, LayoutTraitDefault<float>>(2, 32, 64);
    auto nzLayoutWithDataType = make_frame_layout<NZLayoutPtn, float>(2, 32, 64);
    auto nzLayoutWithCompatTrait = make_frame_layout<NZLayoutPtn, LayoutTrait<float, _8>>(2, 32, 64);
    auto znLayoutWithC0 = make_frame_layout<ZNLayoutPtn, 8>(2, 32, 64);
    using NzTraitWithDefault = GetLayoutTrait<decltype(nzLayoutWithTrait)>;
    using NzTraitWithDataType = GetLayoutTrait<decltype(nzLayoutWithDataType)>;
    using NzCompatTrait = GetLayoutTrait<decltype(nzLayoutWithCompatTrait)>;
    static_assert(NzTraitWithDefault::C0_ELEMENT == _8{}, "LayoutTraitDefault<T> should only be used to infer C0.");
    static_assert(NzTraitWithDataType::C0_ELEMENT == _8{}, "Data type argument should only be used to infer C0.");
    static_assert(AscendC::Std::is_same_v<NzCompatTrait, LayoutTrait<float, _8>>,
        "LayoutTrait<T, C0> should remain source-compatible.");
    static_assert(NzCompatTrait::C0_ELEMENT == _8{}, "LayoutTrait<T, C0> should use C0 as layout trait.");

    EXPECT_EQ(AscendC::Std::get<0>(get_shape(ndLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(ndLayout)), 128);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape(dnLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(dnLayout)), 128);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape(ndExtLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(ndExtLayout)), 128);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape(dnExtLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(dnExtLayout)), 128);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape(nzLayoutWithTrait)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(nzLayoutWithTrait)), 2048);
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<1>(get_shape<1>(nzLayoutWithTrait))), 8);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_shape<1>(nzLayoutWithTrait))), 8);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape(nzLayoutWithDataType)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(nzLayoutWithDataType)), 2048);
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<1>(get_shape<1>(nzLayoutWithDataType))), 8);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_shape<1>(nzLayoutWithDataType))), 8);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape(nzLayoutWithCompatTrait)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(nzLayoutWithCompatTrait)), 2048);
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<1>(get_shape<1>(nzLayoutWithCompatTrait))), 8);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_shape<1>(nzLayoutWithCompatTrait))), 8);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape(znLayoutWithC0)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(znLayoutWithC0)), 2048);
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<0>(get_shape<1>(znLayoutWithC0))), 8);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_shape<1>(znLayoutWithC0))), 4);
}

TEST_F(Tensor_Api_Frame_Layout, NDAndDNGroupTraitWithType)
{
    using namespace AscendC::Te;

    auto ndLayout = make_frame_layout<NDLayoutPtn, LayoutTraitDefault<float>>(8, 16);
    auto dnLayout = make_frame_layout<DNLayoutPtn, LayoutTraitDefault<float>>(8, 16);
    auto ndExtLayout = make_frame_layout<NDExtLayoutPtn, LayoutTraitDefault<float>>(8, 16);
    auto dnExtLayout = make_frame_layout<DNExtLayoutPtn, LayoutTraitDefault<float>>(8, 16);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape(ndLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape(ndLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(ndLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_stride(ndLayout)), 1);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape(dnLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape(dnLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(dnLayout)), 1);
    EXPECT_EQ(AscendC::Std::get<1>(get_stride(dnLayout)), 8);

    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_shape(ndExtLayout))), 8);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_shape(ndExtLayout))), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_stride(ndExtLayout))), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_stride(ndExtLayout))), 1);

    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_shape(dnExtLayout))), 8);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_shape(dnExtLayout))), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_stride(dnExtLayout))), 1);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_stride(dnExtLayout))), 8);
}

TEST_F(Tensor_Api_Frame_Layout, NDAndDNGroupTraitWithTypeAndC0Element)
{
    using namespace AscendC::Te;

    auto ndLayout = make_frame_layout<NDLayoutPtn, LayoutTraitDefault<float>>(8, 16);
    auto dnLayout = make_frame_layout<DNLayoutPtn, LayoutTraitDefault<float>>(8, 16);
    auto ndExtLayout = make_frame_layout<NDExtLayoutPtn, LayoutTraitDefault<float>>(8, 16);
    auto dnExtLayout = make_frame_layout<DNExtLayoutPtn, LayoutTraitDefault<float>>(8, 16);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape(ndLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape(ndLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(ndLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_stride(ndLayout)), 1);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape(dnLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape(dnLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(dnLayout)), 1);
    EXPECT_EQ(AscendC::Std::get<1>(get_stride(dnLayout)), 8);

    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_shape(ndExtLayout))), 8);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_shape(ndExtLayout))), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_stride(ndExtLayout))), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_stride(ndExtLayout))), 1);

    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_shape(dnExtLayout))), 8);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_shape(dnExtLayout))), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_stride(dnExtLayout))), 1);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_stride(dnExtLayout))), 8);
}

TEST_F(Tensor_Api_Frame_Layout, NDAndDNGroupIntegralConstantTrait)
{
    using namespace AscendC::Te;

    auto ndLayout = make_frame_layout<NDLayoutPtn, _16>(8, 16);
    auto dnLayout = make_frame_layout<DNLayoutPtn, _16>(8, 16);
    auto ndExtLayout = make_frame_layout<NDExtLayoutPtn, _16>(8, 16);
    auto dnExtLayout = make_frame_layout<DNExtLayoutPtn, _16>(8, 16);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape(ndLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape(ndLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(ndLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_stride(ndLayout)), 1);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape(dnLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape(dnLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(dnLayout)), 1);
    EXPECT_EQ(AscendC::Std::get<1>(get_stride(dnLayout)), 8);

    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_shape(ndExtLayout))), 8);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_shape(ndExtLayout))), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_stride(ndExtLayout))), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_stride(ndExtLayout))), 1);

    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_shape(dnExtLayout))), 8);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_shape(dnExtLayout))), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_stride(dnExtLayout))), 1);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_stride(dnExtLayout))), 8);
}


TEST_F(Tensor_Api_Frame_Layout, ScaleGroupTraitWithTypeAndC0Element)
{
    using namespace AscendC::Te;

    auto andLayout = make_frame_layout<ScaleANDLayoutPtn>(32, 16);
    auto adnLayout = make_frame_layout<ScaleADNLayoutPtn>(32, 16);
    auto bndLayout = make_frame_layout<ScaleBNDLayoutPtn>(16, 32);
    auto bdnLayout = make_frame_layout<ScaleBDNLayoutPtn>(16, 32);

    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(andLayout)), 32);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(andLayout)), 16);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(adnLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(adnLayout)), 8);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(bndLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(bndLayout)), 8);

    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(bdnLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(bdnLayout)), 32);
}

TEST_F(Tensor_Api_Frame_Layout, ScaleGroupIntegralConstantTrait)
{
    using namespace AscendC::Te;

    auto andLayout = make_frame_layout<ScaleANDLayoutPtn>(32, 16);
    auto adnLayout = make_frame_layout<ScaleADNLayoutPtn>(32, 16);
    auto bndLayout = make_frame_layout<ScaleBNDLayoutPtn>(16, 32);
    auto bdnLayout = make_frame_layout<ScaleBDNLayoutPtn>(16, 32);

    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(andLayout)), 32);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(andLayout)), 16);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(adnLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(adnLayout)), 8);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(bndLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(bndLayout)), 8);

    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(bdnLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(bdnLayout)), 32);
}

TEST_F(Tensor_Api_Frame_Layout, OtherGroupDefaultTrait)
{
    using namespace AscendC::Te;

    auto nzLayout = make_frame_layout<NZLayoutPtn>(32, 64);
    auto znLayout = make_frame_layout<ZNLayoutPtn>(32, 64);
    auto zzLayout = make_frame_layout<ZZLayoutPtn>(32, 64);
    auto nnLayout = make_frame_layout<NNLayoutPtn, LayoutTrait<AscendC::Std::ignore_t, _2>>(16, 32);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(nzLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(nzLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(nzLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(nzLayout)), 4);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(znLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(znLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(znLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(znLayout)), 4);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(zzLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(zzLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(zzLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(zzLayout)), 4);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(nnLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(nnLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(nnLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(nnLayout)), 2);
}

TEST_F(Tensor_Api_Frame_Layout, OtherGroupTraitWithType)
{
    using namespace AscendC::Te;

    auto nzLayout = make_frame_layout<NZLayoutPtn, LayoutTraitDefault<float>>(32, 64);
    auto znLayout = make_frame_layout<ZNLayoutPtn, LayoutTraitDefault<float>>(32, 64);
    auto zzLayout = make_frame_layout<ZZLayoutPtn, LayoutTraitDefault<float>>(32, 64);
    auto nnLayout = make_frame_layout<NNLayoutPtn>(16, 32);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(nzLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(nzLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(nzLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(nzLayout)), 8);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(znLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(znLayout)), 4);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(znLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(znLayout)), 4);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(zzLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(zzLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(zzLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(zzLayout)), 8);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(nnLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(nnLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(nnLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(nnLayout)), 2);
}


TEST_F(Tensor_Api_Frame_Layout, OtherGroupWithDataType)
{
    using namespace AscendC::Te;

    auto nzLayout = make_frame_layout<NZLayoutPtn, float>(32, 64);
    auto znLayout = make_frame_layout<ZNLayoutPtn, float>(32, 64);
    auto zzLayout = make_frame_layout<ZZLayoutPtn, float>(32, 64);
    auto nnLayout = make_frame_layout<NNLayoutPtn>(16, 32);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(nzLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(nzLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(nzLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(nzLayout)), 8);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(znLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(znLayout)), 4);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(znLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(znLayout)), 4);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(zzLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(zzLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(zzLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(zzLayout)), 8);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(nnLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(nnLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(nnLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(nnLayout)), 2);
}


TEST_F(Tensor_Api_Frame_Layout, OtherGroupWithC0Element)
{
    using namespace AscendC::Te;

    auto nzLayout = make_frame_layout<NZLayoutPtn, 8>(32, 64);
    auto znLayout = make_frame_layout<ZNLayoutPtn, 8>(32, 64);
    auto zzLayout = make_frame_layout<ZZLayoutPtn, 8>(32, 64);
    auto nnLayout = make_frame_layout<NNLayoutPtn, 2>(16, 32);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(nzLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(nzLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(nzLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(nzLayout)), 8);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(znLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(znLayout)), 4);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(znLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(znLayout)), 4);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(zzLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(zzLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(zzLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(zzLayout)), 8);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(nnLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(nnLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(nnLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(nnLayout)), 2);
}

TEST_F(Tensor_Api_Frame_Layout, OtherGroupTraitWithTypeAndC0Element)
{
    using namespace AscendC::Te;

    auto nzLayout = make_frame_layout<NZLayoutPtn, LayoutTraitDefault<float>>(32, 64);
    auto znLayout = make_frame_layout<ZNLayoutPtn, LayoutTraitDefault<float>>(32, 64);
    auto zzLayout = make_frame_layout<ZZLayoutPtn, LayoutTraitDefault<float>>(32, 64);
    auto nnLayout = make_frame_layout<NNLayoutPtn>(16, 32);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(nzLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(nzLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(nzLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(nzLayout)), 8);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(znLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(znLayout)), 4);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(znLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(znLayout)), 4);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(zzLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(zzLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(zzLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(zzLayout)), 8);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(nnLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(nnLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(nnLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(nnLayout)), 2);
}

TEST_F(Tensor_Api_Frame_Layout, OtherGroupIntegralConstantTrait)
{
    using namespace AscendC::Te;

    auto nzLayout = make_frame_layout<NZLayoutPtn, _16>(32, 64);
    auto znLayout = make_frame_layout<ZNLayoutPtn, _16>(32, 64);
    auto zzLayout = make_frame_layout<ZZLayoutPtn, _16>(32, 64);
    auto nnLayout = make_frame_layout<NNLayoutPtn, _2>(16, 32);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(nzLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(nzLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(nzLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(nzLayout)), 4);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(znLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(znLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(znLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(znLayout)), 4);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(zzLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(zzLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(zzLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(zzLayout)), 4);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape<0>(nnLayout)), 2);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<0>(nnLayout)), 8);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape<1>(nnLayout)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape<1>(nnLayout)), 2);
}

TEST_F(Tensor_Api_Frame_Layout, ScaleAndZzNnBatchFrameLayout)
{
    using namespace AscendC::Te;

    constexpr int batch = 2;
    auto zz = make_frame_layout<ZZLayoutPtn, 2>(batch, 32, 64);
    auto nn = make_frame_layout<NNLayoutPtn, 2>(batch, 16, 32);
    auto sand = make_frame_layout<ScaleANDLayoutPtn, 2>(batch, 32, 16);
    auto sadn = make_frame_layout<ScaleADNLayoutPtn, 2>(batch, 32, 16);
    auto sbnd = make_frame_layout<ScaleBNDLayoutPtn, 2>(batch, 16, 32);
    auto sbdn = make_frame_layout<ScaleBDNLayoutPtn, 2>(batch, 16, 32);

    auto zzBase = make_frame_layout<ZZLayoutPtn, 2>(32, 64);
    auto nnBase = make_frame_layout<NNLayoutPtn, 2>(16, 32);
    auto sandBase = make_frame_layout<ScaleANDLayoutPtn, 2>(32, 16);
    auto sadnBase = make_frame_layout<ScaleADNLayoutPtn, 2>(32, 16);
    auto sbndBase = make_frame_layout<ScaleBNDLayoutPtn, 2>(16, 32);
    auto sbdnBase = make_frame_layout<ScaleBDNLayoutPtn, 2>(16, 32);

    // Batch dim shape == batch, batch stride == single-matrix Capacity, for every pattern.
    EXPECT_EQ(AscendC::Std::get<0>(get_shape(zz)), batch);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(zz)), capacity(zzBase));
    EXPECT_EQ(AscendC::Std::get<0>(get_shape(nn)), batch);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(nn)), capacity(nnBase));
    EXPECT_EQ(AscendC::Std::get<0>(get_shape(sand)), batch);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(sand)), capacity(sandBase));
    EXPECT_EQ(AscendC::Std::get<0>(get_shape(sadn)), batch);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(sadn)), capacity(sadnBase));
    EXPECT_EQ(AscendC::Std::get<0>(get_shape(sbnd)), batch);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(sbnd)), capacity(sbndBase));
    EXPECT_EQ(AscendC::Std::get<0>(get_shape(sbdn)), batch);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(sbdn)), capacity(sbdnBase));

    // Inner fractal shape unchanged vs the non-batch ZZ(32,64,c0=2) layout: ((16,2),(2,32)).
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<0>(get_shape<1>(zz))), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(get_shape<1>(zz))), 2);
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<1>(get_shape<1>(zz))), 2);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(get_shape<1>(zz))), 32);
}

TEST_F(Tensor_Api_Frame_Layout, ConvFeatureMapLayouts)
{
    using namespace AscendC::Te;

    // NCHW(N,C,H,W): row-major contiguous stride (C*H*W, H*W, W, 1).
    auto nchw = make_frame_layout<NCHWLayoutPtn>(2, 3, 4, 5);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape(nchw)), 2);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape(nchw)), 3);
    EXPECT_EQ(AscendC::Std::get<2>(get_shape(nchw)), 4);
    EXPECT_EQ(AscendC::Std::get<3>(get_shape(nchw)), 5);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(nchw)), 60);
    EXPECT_EQ(AscendC::Std::get<1>(get_stride(nchw)), 20);
    EXPECT_EQ(AscendC::Std::get<2>(get_stride(nchw)), 5);
    EXPECT_EQ(AscendC::Std::get<3>(get_stride(nchw)), 1);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(nchw)>, NCHWLayoutPtn>);

    // NHWC(N,H,W,C): row-major contiguous stride (H*W*C, W*C, C, 1).
    auto nhwc = make_frame_layout<NHWCLayoutPtn>(2, 4, 5, 3);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape(nhwc)), 2);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape(nhwc)), 4);
    EXPECT_EQ(AscendC::Std::get<2>(get_shape(nhwc)), 5);
    EXPECT_EQ(AscendC::Std::get<3>(get_shape(nhwc)), 3);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(nhwc)), 60);
    EXPECT_EQ(AscendC::Std::get<1>(get_stride(nhwc)), 15);
    EXPECT_EQ(AscendC::Std::get<2>(get_stride(nhwc)), 3);
    EXPECT_EQ(AscendC::Std::get<3>(get_stride(nhwc)), 1);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(nhwc)>, NHWCLayoutPtn>);

    // NC1HWC0(N,C1,H,W,C0): C0 supplied by caller, row-major contiguous stride.
    auto nc1hwc0 = make_frame_layout<NC1HWC0LayoutPtn>(2, 3, 4, 5, 16);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape(nc1hwc0)), 2);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape(nc1hwc0)), 3);
    EXPECT_EQ(AscendC::Std::get<2>(get_shape(nc1hwc0)), 4);
    EXPECT_EQ(AscendC::Std::get<3>(get_shape(nc1hwc0)), 5);
    EXPECT_EQ(AscendC::Std::get<4>(get_shape(nc1hwc0)), 16);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(nc1hwc0)), 960);
    EXPECT_EQ(AscendC::Std::get<1>(get_stride(nc1hwc0)), 320);
    EXPECT_EQ(AscendC::Std::get<2>(get_stride(nc1hwc0)), 80);
    EXPECT_EQ(AscendC::Std::get<3>(get_stride(nc1hwc0)), 16);
    EXPECT_EQ(AscendC::Std::get<4>(get_stride(nc1hwc0)), 1);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(nc1hwc0)>, NC1HWC0LayoutPtn>);
}

TEST_F(Tensor_Api_Frame_Layout, Conv3DFeatureMapLayouts)
{
    using namespace AscendC::Te;

    // NCDHW(N,C,D,H,W): row-major contiguous stride (C*D*H*W, D*H*W, H*W, W, 1).
    auto ncdhw = MakeFrameLayout<NCDHWLayoutPtn>(2, 3, 4, 5, 6);
    EXPECT_EQ(AscendC::Std::get<0>(GetShape(ncdhw)), 2);
    EXPECT_EQ(AscendC::Std::get<1>(GetShape(ncdhw)), 3);
    EXPECT_EQ(AscendC::Std::get<2>(GetShape(ncdhw)), 4);
    EXPECT_EQ(AscendC::Std::get<3>(GetShape(ncdhw)), 5);
    EXPECT_EQ(AscendC::Std::get<4>(GetShape(ncdhw)), 6);
    EXPECT_EQ(AscendC::Std::get<0>(GetStride(ncdhw)), 360); // 3*4*5*6
    EXPECT_EQ(AscendC::Std::get<1>(GetStride(ncdhw)), 120); // 4*5*6
    EXPECT_EQ(AscendC::Std::get<2>(GetStride(ncdhw)), 30);  // 5*6
    EXPECT_EQ(AscendC::Std::get<3>(GetStride(ncdhw)), 6);   // 6
    EXPECT_EQ(AscendC::Std::get<4>(GetStride(ncdhw)), 1);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(ncdhw)>, NCDHWLayoutPtn>);

    // NDC1HWC0(N,D,C1,H,W,C0): C0 supplied by caller, row-major contiguous stride.
    auto ndc1hwc0 = MakeFrameLayout<NDC1HWC0LayoutPtn>(2, 4, 3, 5, 6, 16);
    EXPECT_EQ(AscendC::Std::get<0>(GetShape(ndc1hwc0)), 2);
    EXPECT_EQ(AscendC::Std::get<1>(GetShape(ndc1hwc0)), 4);
    EXPECT_EQ(AscendC::Std::get<2>(GetShape(ndc1hwc0)), 3);
    EXPECT_EQ(AscendC::Std::get<3>(GetShape(ndc1hwc0)), 5);
    EXPECT_EQ(AscendC::Std::get<4>(GetShape(ndc1hwc0)), 6);
    EXPECT_EQ(AscendC::Std::get<5>(GetShape(ndc1hwc0)), 16);
    EXPECT_EQ(AscendC::Std::get<0>(GetStride(ndc1hwc0)), 5760); // 4*3*5*6*16
    EXPECT_EQ(AscendC::Std::get<1>(GetStride(ndc1hwc0)), 1440); // 3*5*6*16
    EXPECT_EQ(AscendC::Std::get<2>(GetStride(ndc1hwc0)), 480);  // 5*6*16
    EXPECT_EQ(AscendC::Std::get<3>(GetStride(ndc1hwc0)), 96);   // 6*16
    EXPECT_EQ(AscendC::Std::get<4>(GetStride(ndc1hwc0)), 16);   // 16
    EXPECT_EQ(AscendC::Std::get<5>(GetStride(ndc1hwc0)), 1);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(ndc1hwc0)>, NDC1HWC0LayoutPtn>);
}

// make_frame_layout(batch0, ..., batchN, row, col): the batch axes are flat -- they sit side by side in
// the outermost tuple with the base (row, col) block as the last element, giving rank batchNum + 1:
//   (batch0, batch1, (row, col))
// Batch strides are row-major over the base block: the last batch axis steps by the base capacity and
// each axis to its left multiplies in the extents to its right.
TEST_F(Tensor_Api_Frame_Layout, MultiBatchFrameLayoutTwoBatches)
{
    using namespace AscendC::Te;

    // ND base (8,16): capacity 128. Flat shape (2, 3, (8,16)), strides (384, 128, (16,1)).
    auto layout = make_frame_layout<NDLayoutPtn>(2, 3, 8, 16);

    static_assert(decltype(layout)::rank == 3, "two batch axes + base block are flat");

    EXPECT_EQ(AscendC::Std::get<0>(get_shape(layout)), 2);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape(layout)), 3);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(layout)), 384); // 3 * 128
    EXPECT_EQ(AscendC::Std::get<1>(get_stride(layout)), 128); // base capacity 8*16

    // Last element is the plain ND base block, untouched.
    auto innerShape = AscendC::Std::get<2>(get_shape(layout));
    auto innerStride = AscendC::Std::get<2>(get_stride(layout));
    EXPECT_EQ(AscendC::Std::get<0>(innerShape), 8);
    EXPECT_EQ(AscendC::Std::get<1>(innerShape), 16);
    EXPECT_EQ(AscendC::Std::get<0>(innerStride), 16);
    EXPECT_EQ(AscendC::Std::get<1>(innerStride), 1);

    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(layout)>, NDLayoutPtn>);
}

TEST_F(Tensor_Api_Frame_Layout, MultiBatchFrameLayoutThreeBatches)
{
    using namespace AscendC::Te;

    // base (8,16) capacity 128. Flat shape (2, 3, 4, (8,16)), strides (1536, 512, 128, (16,1)).
    auto layout = make_frame_layout<NDLayoutPtn>(2, 3, 4, 8, 16);

    static_assert(decltype(layout)::rank == 4, "three batch axes + base block are flat");

    EXPECT_EQ(AscendC::Std::get<0>(get_shape(layout)), 2);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape(layout)), 3);
    EXPECT_EQ(AscendC::Std::get<2>(get_shape(layout)), 4);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(layout)), 1536); // 3 * 4 * 128
    EXPECT_EQ(AscendC::Std::get<1>(get_stride(layout)), 512);  // 4 * 128
    EXPECT_EQ(AscendC::Std::get<2>(get_stride(layout)), 128);  // base capacity
}

// Flat batch axes with a fractal base: the last element stays the untouched NZ block
// ((row0,row1),(col0,col1)), so the result is (batch0, batch1, ((16,2),(16,4))).
TEST_F(Tensor_Api_Frame_Layout, MultiBatchFrameLayoutFractal)
{
    using namespace AscendC::Te;

    // NZ(32,64) with default trait: single-matrix capacity 2048 (see BatchFrameLayoutDefaultTrait).
    auto layout = make_frame_layout<NZLayoutPtn>(2, 3, 32, 64);

    static_assert(decltype(layout)::rank == 3);

    EXPECT_EQ(AscendC::Std::get<0>(get_shape(layout)), 2);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape(layout)), 3);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(layout)), 6144); // 3 * 2048
    EXPECT_EQ(AscendC::Std::get<1>(get_stride(layout)), 2048); // base capacity

    // Base block keeps the NZ fractal nesting ((16, M/16), (C0, N/C0)).
    auto baseShape = AscendC::Std::get<2>(get_shape(layout));
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<0>(baseShape)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<0>(baseShape)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(AscendC::Std::get<1>(baseShape)), 16);
    EXPECT_EQ(AscendC::Std::get<1>(AscendC::Std::get<1>(baseShape)), 4);

    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(layout)>, NZLayoutPtn>);
}

// Regression guard: adding the multi-batch overload must not change the existing arities -- 3 args is
// still single-batch, and the conv feature-map patterns keep their fixed positional meaning (their
// 4/5-arg Make is not a batch form).
TEST_F(Tensor_Api_Frame_Layout, MultiBatchFrameLayoutKeepsExistingArities)
{
    using namespace AscendC::Te;

    // 3 args = single batch, unchanged.
    auto single = make_frame_layout<NDLayoutPtn>(2, 8, 16);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape(single)), 2);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(single)), 128);

    // 4 args on NCHW stays (N, C, H, W), not (batch0, batch1, row, col).
    auto nchwKeep = make_frame_layout<NCHWLayoutPtn>(2, 3, 4, 5);
    EXPECT_EQ(AscendC::Std::get<0>(get_shape(nchwKeep)), 2);
    EXPECT_EQ(AscendC::Std::get<1>(get_shape(nchwKeep)), 3);
    EXPECT_EQ(AscendC::Std::get<2>(get_shape(nchwKeep)), 4);
    EXPECT_EQ(AscendC::Std::get<3>(get_shape(nchwKeep)), 5);
    EXPECT_EQ(AscendC::Std::get<0>(get_stride(nchwKeep)), 60);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(nchwKeep)>, NCHWLayoutPtn>);

    // 5 args on NC1HWC0 stays (N, C1, H, W, C0).
    auto nc1hwc0Keep = make_frame_layout<NC1HWC0LayoutPtn>(2, 3, 4, 5, 16);
    EXPECT_EQ(AscendC::Std::get<4>(get_shape(nc1hwc0Keep)), 16);
    static_assert(AscendC::Std::is_same_v<GetLayoutPattern<decltype(nc1hwc0Keep)>, NC1HWC0LayoutPtn>);
}
