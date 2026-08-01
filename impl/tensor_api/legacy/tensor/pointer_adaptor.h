/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_TENSOR_API_LEGACY_TENSOR_POINTER_ADAPTOR_H
#define IMPL_TENSOR_API_LEGACY_TENSOR_POINTER_ADAPTOR_H

namespace asc {
namespace te {

template <typename Iterator, typename DerivedType>
using IterAdaptor = iter_adaptor<Iterator, DerivedType>;

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_LEGACY_TENSOR_POINTER_ADAPTOR_H
