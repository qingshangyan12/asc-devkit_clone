/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif

/*!
* \file load_trait.h
* \brief Compile-time traits for register-level UB load/store (design contract).
*
* Orthogonal dimensions (do not encode in function overload names):
*   - Op tag (load::UbAlign, load::MaskAlign, ...)     what instruction family
*   - Trait (this file)                                how (dist, post, width, addr)
*   - Verb (Load / Store / LoadMask / StoreMask)       eliminate LoadAlign homonymy
*   - Operand factories (MakeRegTensor, MakeMaskReg)     who
*
* MVP implemented: load::UbAlign + load::UbStoreAlign with default traits.
* Planned: RegLoadTrait::{post, width, addr}, BlockStrideTrait, MaskMoveTrait, UnalignTrait.
*/
#ifndef INCLUDE_TENSOR_API_ARCH_VECTOR_LOAD_TRAIT_H
#define INCLUDE_TENSOR_API_ARCH_VECTOR_LOAD_TRAIT_H

#include "basic_api/reg_compute/kernel_reg_compute_utils.h"

namespace AscendC {
namespace Te {

enum class PostMode : uint8_t { NORMAL = 0, POST_UPDATE = 1 };
enum class RegWidth : uint8_t { SINGLE = 1, DUAL = 2 };
enum class AddrMode : uint8_t { DIRECT = 0, OFFSET = 1, POST_UPDATE_PTR = 2, ADDR_REG = 3 };

struct RegLoadTrait {
    Reg::LoadDist dist = Reg::LoadDist::DIST_NORM;
    PostMode post = PostMode::NORMAL;
    RegWidth width = RegWidth::SINGLE;
    AddrMode addr = AddrMode::DIRECT;
};

struct RegStoreTrait {
    Reg::StoreDist dist = Reg::StoreDist::DIST_NORM;
    PostMode post = PostMode::NORMAL;
    RegWidth width = RegWidth::SINGLE;
    AddrMode addr = AddrMode::DIRECT;
};

struct BlockStrideTrait {
    PostMode post = PostMode::NORMAL;
};

struct MaskMoveTrait {
    Reg::MaskDist dist = Reg::MaskDist::DIST_NORM;
    PostMode post = PostMode::NORMAL;
};

struct UnalignTrait {
    PostMode post = PostMode::POST_UPDATE;
    AddrMode addr = AddrMode::DIRECT;
};

constexpr RegLoadTrait regLoadTraitDefault{};
constexpr RegStoreTrait regStoreTraitDefault{};

} // namespace Te
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_ARCH_VECTOR_LOAD_TRAIT_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif
