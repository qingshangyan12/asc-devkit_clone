/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_TENSOR_API_LEGACY_TENSOR_TENSOR_TYPES_H
#define IMPL_TENSOR_API_LEGACY_TENSOR_TENSOR_TYPES_H

namespace AscendC {

template <typename EngineType, typename LayoutType>
using TensorAttribute = tensor_attribute<EngineType, LayoutType>;

} // namespace AscendC

namespace asc {
namespace te {

using AscendC::TensorAttribute;

template <typename LocationType, typename EngineType, typename LayoutType>
using MakeTensorResult = make_tensor_result<LocationType, EngineType, LayoutType>;

template <typename EngineType, typename LayoutType>
using BaseTensor = base_tensor<EngineType, LayoutType>;

template <typename T>
using IsAttrTensor = is_attr_tensor<T>;

template <typename T>
constexpr bool IsAttrTensorV = is_attr_tensor_v<T>;

template <typename T>
using MakeTensorBuilder = make_tensor_builder<T>;

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_LEGACY_TENSOR_TENSOR_TYPES_H
