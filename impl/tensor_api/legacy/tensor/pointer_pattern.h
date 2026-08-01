/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_TENSOR_API_LEGACY_TENSOR_POINTER_PATTERN_H
#define IMPL_TENSOR_API_LEGACY_TENSOR_POINTER_PATTERN_H

namespace asc {
namespace te {

template <typename T = uint16_t>
using PtrTrait = ptr_trait<T>;

template <typename T>
using MemPtrTraitT = mem_ptr_trait_t<T>;

template <typename Hardware, typename Arg>
using EnableMakePtrByTrait = enable_make_ptr_by_trait<Hardware, Arg>;

template <typename Hardware, typename Arg>
using EnableMakeHardwarePtr = enable_make_hardware_ptr<Hardware, Arg>;

template <typename Iterator>
using EnableMakePtrByIter = enable_make_ptr_by_iter<Iterator>;

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_LEGACY_TENSOR_POINTER_PATTERN_H
