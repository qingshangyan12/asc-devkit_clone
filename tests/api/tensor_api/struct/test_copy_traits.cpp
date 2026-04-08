/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to License for details. You may not use this file except in compliance with License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <gtest/gtest.h>
#include "tensor_api/stub/cce_stub.h"
#include "include/experimental/tensor_api/tensor.h"

class Tensor_Api_Copy_Traitsaripe : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    void TearDown() {}
};

TEST_F(Tensor_Api_Copy_Traitsaripe, CopyTraitsDefault)
{
    using namespace AscendC::Std;

    using CopyTraitsType = CopyTraits<DataCopyOperation, DataCopyTrait>;
    using TraitType = typename CopyTraitsType::TraitType;

    EXPECT_TRUE(is_same_v<TraitType, DataCopyTrait>);
}

TEST_F(Tensor_Api_Copy_Traitsaripe, CopyTraitsWithMultiple)
{
    using namespace AscendC::Std;

    using CopyTraitsType = CopyTraits<DataCopyOperation, DataCopyTrait, DataCopyTrait>;
    using TraitType = typename CopyTraitsType::TraitType;

    EXPECT_TRUE(is_same_v<TraitType, DataCopyTrait>);
}

TEST_F(Tensor_Api_Copy_Traitsaripe, CopyTraitsDefaultTrait)
{
    using namespace AscendC::Std;

    using CopyTraitsType = CopyTraits<DataCopyOperation>;
    constexpr auto defaultTrait = CopyTraitsType::defaultTrait;

    EXPECT_TRUE(is_same_v<decltype(defaultTrait), const DataCopyTrait&>);
}

TEST_F(Tensor_Api_Copy_Traitsaripe, CopyTraitsWithMethod)
{
    using namespace AscendC::Std;

    using CopyTraitsType = CopyTraits<DataCopyOperation>;
    auto traits = CopyTraitsType::with(DataCopyTrait{});

    EXPECT_TRUE(is_same_v<decltype(traits), CopyTraits<DataCopyOperation, DataCopyTrait>>);
}

TEST_F(Tensor_Api_Copy_Traitsaripe, CopyTraitsMultipleWithMethod)
{
    using namespace AscendC::Std;

    using CopyTraitsType = CopyTraits<DataCopyOperation>;
    auto traits = CopyTraitsType::with(DataCopyTrait{}, DataCopyTrait{});

    EXPECT_TRUE(is_same_v<decltype(traits), CopyTraits<DataCopyOperation, DataCopyTrait, DataCopyTrait>>);
}

TEST_F(Tensor_Api_Copy_Traits, CopyTraitsWithMultiple)
{
    using CopyTraitsType = CopyTraits<DataCopyOperation, DataCopyTrait, DataCopyTrait>;
    using TraitType = typename CopyTraitsType::TraitType;

    EXPECT_TRUE(std::is_same_v<TraitType, DataCopyTrait>);
}

TEST_F(Tensor_Api_Copy_Traits, CopyTraitsDefaultTrait)
{
    using CopyTraitsType = CopyTraits<DataCopyOperation>;
    constexpr auto defaultTrait = CopyTraitsType::defaultTrait;

    EXPECT_TRUE(std::is_same_v<decltype(defaultTrait), const DataCopyTrait&>);
}

TEST_F(Tensor_Api_Copy_Traits, CopyTraitsWithMethod)
{
    using CopyTraitsType = CopyTraits<DataCopyOperation>;
    auto traits = CopyTraitsType::with(DataCopyTrait{});

    EXPECT_TRUE(std::is_same_v<decltype(traits), CopyTraits<DataCopyOperation, DataCopyTrait>>);
}

TEST_F(Tensor_Api_Copy_Traits, CopyTraitsMultipleWithMethod)
{
    using CopyTraitsType = CopyTraits<DataCopyOperation>;
    auto traits = CopyTraitsType::with(DataCopyTrait{}, DataCopyTrait{});

    EXPECT_TRUE(std::is_same_v<decltype(traits), CopyTraits<DataCopyOperation, DataCopyTrait, DataCopyTrait>>);
}
