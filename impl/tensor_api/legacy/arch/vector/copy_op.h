/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_TENSOR_API_LEGACY_ARCH_VECTOR_COPY_OP_H
#define IMPL_TENSOR_API_LEGACY_ARCH_VECTOR_COPY_OP_H

namespace asc {
namespace te {

using CopyGM2UB = copy_gm_to_ub;
using CopyGM2UBWith = copy_gm_to_ub_with;
using CopyUB2L1 = copy_ub_to_l1;
using CopyUB2GM = copy_ub_to_gm;
using CopyUB2UB = copy_ub_to_ub;
using CopyGM2UBTrait = copy_gm_to_ub_trait;
using CopyGM2UBTraitDefault = copy_gm_to_ub_trait_default;
using CopyUB2L1Trait = copy_ub_to_l1_trait;
using CopyUB2L1TraitDefault = copy_ub_to_l1_trait_default;
using CopyUB2GMTrait = copy_ub_to_gm_trait;
using CopyUB2GMTraitDefault = copy_ub_to_gm_trait_default;
using CopyUB2UBTrait = copy_ub_to_ub_trait;
using CopyUB2UBTraitDefault = copy_ub_to_ub_trait_default;

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_LEGACY_ARCH_VECTOR_COPY_OP_H
