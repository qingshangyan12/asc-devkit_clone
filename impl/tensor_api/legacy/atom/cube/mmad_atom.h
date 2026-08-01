/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_TENSOR_API_LEGACY_ATOM_CUBE_MMAD_ATOM_H
#define IMPL_TENSOR_API_LEGACY_ATOM_CUBE_MMAD_ATOM_H

namespace asc {
namespace te {

template <typename MmadOperationType, typename... MmadOpArgs>
using MmadTraits = mmad_traits<MmadOperationType, MmadOpArgs...>;

template <typename... Args>
using MmadAtom = mmad_atom<Args...>;

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_LEGACY_ATOM_CUBE_MMAD_ATOM_H
