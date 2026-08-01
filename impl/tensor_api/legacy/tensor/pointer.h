/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_TENSOR_API_LEGACY_TENSOR_POINTER_H
#define IMPL_TENSOR_API_LEGACY_TENSOR_POINTER_H

namespace asc {
namespace te {

template <typename PtrPattern, typename Iterator>
__aicore__ inline auto MakeLocationMemPtr(Iterator iterator)
{
    return make_location_mem_ptr<PtrPattern>(iterator);
}

template <typename PtrPattern, typename DataType, typename Addr, EnableMakePtrByTrait<PtrPattern, Addr> = 0>
__aicore__ inline auto MakeMemPtr(Addr address)
{
    return make_mem_ptr<PtrPattern, DataType>(address);
}

template <typename PtrPattern, typename Iterator, EnableMakeHardwarePtr<PtrPattern, Iterator> = 0>
__aicore__ inline constexpr auto MakeMemPtr(Iterator iterator)
{
    return make_mem_ptr<PtrPattern>(iterator);
}

template <typename Iterator, EnableMakePtrByIter<Iterator> = 0>
__aicore__ inline constexpr auto MakeMemPtr(Iterator iterator)
{
    return make_mem_ptr(iterator);
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_LEGACY_TENSOR_POINTER_H
