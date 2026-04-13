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

namespace Hardware {
    struct EmptyTrait {};
    struct GM {};
    struct UB {};
    struct L1 {};
    struct L0A {};
    struct L0B {};
    struct L0C {};
    struct Bias {};
    struct Fixbuf {};
}

using PtrMemSet = TupleMap<
    Std::tuple<Hardware::UB, MakeUBMemPtr>,
    Std::tuple<Hardware::L1, MakeL1MemPtr>,
    Std::tuple<Hardware::L0A, MakeL0AMemPtr>,
    Std::tuple<Hardware::L0B, MakeL0BMemPtr>,
    Std::tuple<Hardware::L0C, MakeL0CMemPtr>,
    Std::tuple<Hardware::Bias, MakeBiasMemPtr>,
    Std::tuple<Hardware::Fixbuf, MakeFixbufMemPtr>>;

template <typename PtrPattern, typename TraitType, typename... Args>
__aicore__ inline constexpr auto MakeMemPtr(Args... args)
{
    using Arg = typename Std::tuple_element<0, Std::tuple<Args...>>::type;
    static_assert(!IsMemPtrIterator<Arg>::value, "MakeMemPtr expects a byteOffset. ");
    
    using GetPtrMakeFun = typename PtrMemSet::template Get<PtrPattern>;
    return GetPtrMakeFun::template Make<PtrPattern, TraitType>(args...);
}

template <typename PtrPattern, typename... Args>
__aicore__ inline constexpr auto MakeMemPtr(Args... args)
{
    using Arg = typename Std::tuple_element<0, Std::tuple<Args...>>::type;
    static_assert(IsMemPtrIterator<Arg>::value, "MakeMemPtr<PtrPattern>(arg) expects an iterator/pointer");
    return MakeHardwareMemPtr<PtrPattern>(args...);
}

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_TENSOR_POINTER_PATTERN_H
