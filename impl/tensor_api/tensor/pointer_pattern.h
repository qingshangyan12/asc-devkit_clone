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
#warning                                                                                                               \
    "impl/tensor_api/tensor/pointer_pattern.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file pointer_pattern.h
* \brief
*/
#ifndef IMPL_TENSOR_API_TENSOR_POINTER_PATTERN_H
#define IMPL_TENSOR_API_TENSOR_POINTER_PATTERN_H

#include "impl/tensor_api/tensor/pointer_mem_impl.h"

namespace AscendC {
namespace Te {

template <typename T>
using GetMemLocation = typename T::iterator::ptrPattern;

template <typename PtrPattern, typename Iterator>
__aicore__ inline constexpr auto MakeMemPtr(Iterator iter)
{
    return MakeLocationMemPtr<PtrPattern, Iterator>(iter);
}

template <typename Iterator>
__aicore__ inline constexpr auto MakeMemPtr(Iterator iter)
{
    using ptrPattern = GetAttributeLocation<typename IterEle<Iterator>::type*>;
    return MakeMemPtr<ptrPattern, Iterator>(iter);
}

template <typename PtrPattern, typename DataType, typename Addrss>
__aicore__ inline constexpr auto MakeMemPtr(Addrss addr)
{
    using iterator = typename locationAttr<DataType>::locationMap::template Get<PtrPattern>;
    return MakeMemPtr<PtrPattern, iterator>(reinterpret_cast<iterator>(asc_get_phy_buf_addr(0) + arg));
}

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_TENSOR_POINTER_PATTERN_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
