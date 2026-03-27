/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/*!
* \file copy_gm2l1.h
* \brief
*/
#ifndef IMPL_TENSOR_API_ATOM_CUBE_DATAMOVE_COPY_GM2L1_H
#define IMPL_TENSOR_API_ATOM_CUBE_DATAMOVE_COPY_GM2L1_H

#include "impl/experimental/tensor_api/utils/utils_impl.h"

#include "impl/experimental/tensor_api/arch/data_copy_impl.h"
#include "impl/experimental/tensor_api/atom/copy_traits_impl.h"

namespace AscendC {
namespace Te {

struct DataCopyTraitDefaultUB {
    using TraitType = DataCopyTrait;
    static constexpr const TraitType value = DEFAULT_DATA_COPY_TRAIT;
};

struct CopyOperation {
    template <typename Tp, const Tp& traits, typename... Args>
    __aicore__ inline static void Copy(const Args& ...args)
    {
        DataCopy<traits, Args...>(args...);
    }
};

template <typename Traits>
struct CopyTraits<CopyOperation, Traits> : public CopyTraits<CopyOperation, Traits, CopyOperation, DataCopyTraitDefaultUB> {};

template <>
struct CopyTraits<CopyOperation> : public CopyTraits<CopyOperation, DataCopyTraitDefaultUB> {};

using CopyGM2UB = CopyOperation;
using CopyUB2GM = CopyOperation;
using CopyUB2L1 = CopyOperation;
using CopyL12UB = CopyOperation;

}
}

#endif // IMPL_TENSOR_API_ATOM_CUBE_DATAMOVE_COPY_GM2L1_H