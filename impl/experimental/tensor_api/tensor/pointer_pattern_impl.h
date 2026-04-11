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
 * \file pointer_pattern_impl.h
* \brief
*/
#ifndef IMPL_TENSOR_API_TENSOR_POINTER_PATTERN_IMPL_H
#define IMPL_TENSOR_API_TENSOR_POINTER_PATTERN_IMPL_H

#include "impl/experimental/tensor_api/tensor/pointer_mem_impl.h"

namespace AscendC {
namespace Te {

template <typename T, typename = void>
struct IsMemPtrIterator : Std::false_type {};

template <typename T>
struct IsMemPtrIterator<T, void_t<decltype(*Std::declval<T&>())>> : Std::true_type {};

template <typename T, typename = void>
struct HasMemPtrTraitDataType : Std::false_type {};

template <typename T>
struct HasMemPtrTraitDataType<T, void_t<typename T::type>> : Std::true_type {};

template <typename PtrPattern, typename Iterator>
__aicore__ inline auto MakeHardwareMemPtr(Iterator iter)
{
    return HardwareMemPtrV2<PtrPattern, Iterator>{iter};
}

template <typename PtrPattern, typename Iterator>
__aicore__ inline auto MakeHardwareMemPtrV2(Iterator iter)
{
    return HardwareMemPtrV2<PtrPattern, Iterator>{iter};
}

struct MakeGMMemPtr {
    template <typename PtrPattern, typename TraitType, typename Arg>
    __aicore__ inline static auto Make(const Arg& arg) {
        static_assert(sizeof(Arg) == 0, "GMMemPtr does not support byteOffset construction.");
    }
};

struct MakeUBMemPtr {
    template <typename PtrPattern, typename TraitType, typename Arg>
    __aicore__ inline static auto Make(const Arg& arg) {
        using T = typename TraitType::type;
        return MakeHardwareMemPtrV2<PtrPattern>(reinterpret_cast<__ubuf__ T*>(get_imm(0) + arg));
    }
};

struct MakeL1MemPtr {
    template <typename PtrPattern, typename TraitType, typename Arg>
    __aicore__ inline static auto Make(const Arg& arg) {
        using T = typename TraitType::type;
        return MakeHardwareMemPtrV2<PtrPattern>(reinterpret_cast<__cbuf__ T*>(get_imm(0) + arg));
    }
};

struct MakeL0AMemPtr {
    template <typename PtrPattern, typename TraitType, typename Arg>
    __aicore__ inline static auto Make(const Arg& arg) {
        using T = typename TraitType::type;
        return MakeHardwareMemPtrV2<PtrPattern>(reinterpret_cast<__ca__ T*>(get_imm(0) + arg));
    }
};

struct MakeL0BMemPtr {
    template <typename PtrPattern, typename TraitType, typename Arg>
    __aicore__ inline static auto Make(const Arg& arg) {
        using T = typename TraitType::type;
        return MakeHardwareMemPtrV2<PtrPattern>(reinterpret_cast<__cb__ T*>(get_imm(0) + arg));
    }
};

struct MakeL0CMemPtr {
    template <typename PtrPattern, typename TraitType, typename Arg>
    __aicore__ inline static auto Make(const Arg& arg) {
        using T = typename TraitType::type;
        return MakeHardwareMemPtrV2<PtrPattern>(reinterpret_cast<__cc__ T*>(get_imm(0) + arg));
    }
};

struct MakeBiasMemPtr {
    template <typename PtrPattern, typename TraitType, typename Arg>
    __aicore__ inline static auto Make(const Arg& arg) {
        using T = typename TraitType::type;
        return MakeHardwareMemPtrV2<PtrPattern>(reinterpret_cast<__biasbuf__ T*>(get_imm(0) + arg));
    }
};

struct MakeFixbufMemPtr {
    template <typename PtrPattern, typename TraitType, typename Arg>
    __aicore__ inline static auto Make(const Arg& arg) {
        using T = typename TraitType::type;
        return MakeHardwareMemPtrV2<PtrPattern>(reinterpret_cast<__fbuf__ T*>(get_imm(0) + arg));
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_TENSOR_POINTER_PATTERN_IMPL_H
