/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_TENSOR_API_LEGACY_UTILS_LOCATION_H
#define IMPL_TENSOR_API_LEGACY_UTILS_LOCATION_H

namespace asc {
namespace te {

using LocationTypeCombo = location_type_combo;

template <typename T>
using GetAttributeLocation = get_attribute_location<T>;

template <typename T>
using GetAttributeElementType = get_attribute_element_type<T>;

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_LEGACY_UTILS_LOCATION_H
