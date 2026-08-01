/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_TENSOR_API_LEGACY_TENSOR_ENGINE_H
#define IMPL_TENSOR_API_LEGACY_TENSOR_ENGINE_H

namespace asc {
namespace te {

template <typename Iterator>
using ViewEngine = view_engine<Iterator>;

template <typename Iterator>
using ConstViewEngine = const_view_engine<Iterator>;

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_LEGACY_TENSOR_ENGINE_H
