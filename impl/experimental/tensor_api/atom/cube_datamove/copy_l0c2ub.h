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
 * \file copy_l0c2ub.h
 * \brief
*/
#ifndef IMPL_TENSOR_API_ATOM_CUBE_DATAMOVE_COPY_L0C2UB_H
#define IMPL_TENSOR_API_ATOM_CUBE_DATAMOVE_COPY_L0C2UB_H

#include "impl/experimental/tensor_api/utils/utils_impl.h"

#include "impl/experimental/tensor_api/arch/cube_datamove/fixpipe/fixpipe_impl.h"
#include "impl/experimental/tensor_api/atom/cube_datamove/copy_fixpipe_traits.h"
#include "impl/experimental/tensor_api/atom/copy_traits_impl.h"


namespace AscendC {
namespace Te {

struct CopyL0C2UB {
    template <typename Tp, const Tp& traits, typename... Args>
    __aicore__ inline static void Copy(const Args& ...args)
    {
        Fixpipe<traits, Args...>(args...);
    }
};

struct CopyL0C2UBWith {
    template <typename Tp, const Tp& traits, typename... Args>
    __aicore__ inline static void Copy(const Args& ...args, uint8_t unitFlag)
    {
        // custom function Fixpipe<traits, Args...>(args..., unitFlag)
    }
};

template <typename TraitStruct>
struct CopyTraits<CopyL0C2UBWith, TraitStruct>
{
    using TraitType = typename TraitStruct::TraitType;
    static constexpr const TraitType defaultTrait = TraitStruct::value;

    template <const TraitType& trait = defaultTrait, typename... Args>
    __aicore__ inline void CopyUnpack(const Args& ...args) const {
      CopyL0C2UBWith::Copy<TraitType, trait, Args...>(args..., unitFlag);
    }

    uint8_t unitFlag = 0;
};

template <typename Traits>
struct CopyTraits<CopyL0C2UB, Traits> : public CopyTraits<CopyL0C2UB, Traits, CopyL0C2UBWith, FixpipeTraitDefault> {};

template <>
struct CopyTraits<CopyL0C2UB> : public CopyTraits<CopyL0C2UB, FixpipeTraitDefault> {};

}
}

#endif // IMPL_TENSOR_API_ATOM_CUBE_DATAMOVE_COPY_L0C2UB_H
