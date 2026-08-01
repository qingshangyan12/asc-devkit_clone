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
* \file copy_op.h
* \brief
*/
#ifndef INCLUDE_TENSOR_API_ARCH_CUBE_COPY_OP_H
#define INCLUDE_TENSOR_API_ARCH_CUBE_COPY_OP_H

namespace asc {
namespace te {

struct copy_gm_to_l1;
struct copy_l1_to_ub;
struct copy_l1_to_l0b;
struct copy_l1_to_l0a;
struct copy_l1_to_l0_scale_a;
struct copy_l1_to_l0_scale_b;
struct copy_l1_to_fb;
struct copy_l1_to_bt;
struct copy_l0c_to_gm;
struct copy_l0c_to_gm_with;
struct copy_l0c_to_ub;
struct copy_l0c_to_ub_with;
struct copy_l0c_to_l1;
struct copy_l0c_to_l1_with;

struct copy_gm_to_l1_trait;
struct copy_gm_to_l1_trait_default;
struct copy_l1_to_ub_trait;
struct copy_l1_to_ub_trait_default;
struct copy_l1_to_l0a_trait;
struct copy_l1_to_l0a_trait_default;
struct copy_l1_to_l0b_trait;
struct copy_l1_to_l0b_trait_default;
struct copy_l1_to_l0_scale_a_trait;
struct copy_l1_to_l0_scale_a_trait_default;
struct copy_l1_to_l0_scale_b_trait;
struct copy_l1_to_l0_scale_b_trait_default;
struct copy_l1_to_fb_trait;
struct copy_l1_to_fb_trait_default;
struct copy_l1_to_bt_trait;
struct copy_l1_to_bt_trait_default;
struct copy_l0c_to_gm_trait;
struct copy_l0c_to_gm_trait_default;
struct copy_l0c_to_ub_trait;
struct copy_l0c_to_ub_trait_default;
struct copy_l0c_to_l1_trait;
struct copy_l0c_to_l1_trait_default;

using CopyGM2L1 = copy_gm_to_l1;
using CopyL12UB = copy_l1_to_ub;
using CopyL12L0B = copy_l1_to_l0b;
using CopyL12L0A = copy_l1_to_l0a;
using CopyL12L0ScaleA = copy_l1_to_l0_scale_a;
using CopyL12L0ScaleB = copy_l1_to_l0_scale_b;
using CopyL12FB = copy_l1_to_fb;
using CopyL12BT = copy_l1_to_bt;
using CopyL0C2GM = copy_l0c_to_gm;
using CopyL0C2GMWith = copy_l0c_to_gm_with;
using CopyL0C2UB = copy_l0c_to_ub;
using CopyL0C2UBWith = copy_l0c_to_ub_with;
using CopyL0C2L1 = copy_l0c_to_l1;
using CopyL0C2L1With = copy_l0c_to_l1_with;
using CopyGM2L1Trait = copy_gm_to_l1_trait;
using CopyGM2L1TraitDefault = copy_gm_to_l1_trait_default;
using CopyL12UBTrait = copy_l1_to_ub_trait;
using CopyL12UBTraitDefault = copy_l1_to_ub_trait_default;
using CopyL12L0ATrait = copy_l1_to_l0a_trait;
using CopyL12L0ATraitDefault = copy_l1_to_l0a_trait_default;
using CopyL12L0BTrait = copy_l1_to_l0b_trait;
using CopyL12L0BTraitDefault = copy_l1_to_l0b_trait_default;
using CopyL12L0ScaleATrait = copy_l1_to_l0_scale_a_trait;
using CopyL12L0ScaleATraitDefault = copy_l1_to_l0_scale_a_trait_default;
using CopyL12L0ScaleBTrait = copy_l1_to_l0_scale_b_trait;
using CopyL12L0ScaleBTraitDefault = copy_l1_to_l0_scale_b_trait_default;
using CopyL12FBTrait = copy_l1_to_fb_trait;
using CopyL12FBTraitDefault = copy_l1_to_fb_trait_default;
using CopyL12BTTrait = copy_l1_to_bt_trait;
using CopyL12BTTraitDefault = copy_l1_to_bt_trait_default;
using CopyL0C2GMTrait = copy_l0c_to_gm_trait;
using CopyL0C2GMTraitDefault = copy_l0c_to_gm_trait_default;
using CopyL0C2UBTrait = copy_l0c_to_ub_trait;
using CopyL0C2UBTraitDefault = copy_l0c_to_ub_trait_default;
using CopyL0C2L1Trait = copy_l0c_to_l1_trait;
using CopyL0C2L1TraitDefault = copy_l0c_to_l1_trait_default;

} // namespace te
} // namespace asc

#endif // INCLUDE_TENSOR_API_ARCH_CUBE_COPY_OP_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif
