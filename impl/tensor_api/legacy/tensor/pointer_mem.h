/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_TENSOR_API_LEGACY_TENSOR_POINTER_MEM_H
#define IMPL_TENSOR_API_LEGACY_TENSOR_POINTER_MEM_H

namespace asc {
namespace te {

template <typename PtrPattern, typename Pointer>
using HardwareMemPtr = hardware_mem_ptr<PtrPattern, Pointer>;

template <typename T>
using GetMemLocation = get_mem_location<T>;

template <typename PtrPattern, typename Pointer>
constexpr bool IsHardwareMemV = is_hardware_mem_v<PtrPattern, Pointer>;

template <typename T>
constexpr bool IsHardwareMemPtrV = is_hardware_mem_ptr_v<T>;

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_LEGACY_TENSOR_POINTER_MEM_H
