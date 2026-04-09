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
 * \file pointer_pattern.h
* \brief
*/
#ifndef IMPL_TENSOR_API_TENSOR_POINTER_PATTERN_H
#define IMPL_TENSOR_API_TENSOR_POINTER_PATTERN_H

#include "impl/experimental/tensor_api/utils/map_impl.h"
#include "impl/experimental/tensor_api/tensor/pointer_pattern_impl.h"

namespace AscendC {
namespace Te {

struct GMMemPtrPattern {};
struct UBMemPtrPattern {};
struct L1MemPtrPattern {};
struct L0AMemPtrPattern {};
struct L0BMemPtrPattern {};
struct L0CMemPtrPattern {};
struct BiasMemPtrPattern {};
struct FixbufMemPtrPattern {};

using PtrMemSet = TupleMap<
    Std::tuple<GMMemPtrPattern, MakeGMMemPtr>,
    Std::tuple<UBMemPtrPattern, MakeUBMemPtr>,
    Std::tuple<L1MemPtrPattern, MakeL1MemPtr>,
    Std::tuple<L0AMemPtrPattern, MakeL0AMemPtr>,
    Std::tuple<L0BMemPtrPattern, MakeL0BMemPtr>,
    Std::tuple<L0CMemPtrPattern, MakeL0CMemPtr>,
    Std::tuple<BiasMemPtrPattern, MakeBiasMemPtr>,
    Std::tuple<FixbufMemPtrPattern, MakeFixbufMemPtr>>;

template <typename PtrPattern, typename TraitType, typename... Args>
__aicore__ inline constexpr auto MakeMemPatternPtr(Args... args)
{
    using GetPtrMakeFun = typename PtrMemSet::template Get<PtrPattern>;
    static_assert(!Std::is_same_v<GetPtrMakeFun, EmptyValue>, "Unsupported pointer pattern.");
    return GetPtrMakeFun::template Make<TraitType>(args...);
}

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_TENSOR_POINTER_PATTERN_H
