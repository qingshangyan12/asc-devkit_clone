/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_TENSOR_API_LEGACY_TENSOR_TENSOR_H
#define IMPL_TENSOR_API_LEGACY_TENSOR_TENSOR_H

namespace asc {
namespace te {

template <typename Iterator, typename... Args>
__aicore__ inline constexpr auto MakeTensor(const Iterator& iterator, const Args&... args)
{
    return make_tensor(iterator, args...);
}

template <typename Tensor>
__aicore__ inline constexpr auto MakeSingleBatchSubTensor(const Tensor& tensor, uint32_t batch_index)
{
    return make_single_batch_sub_tensor(tensor, batch_index);
}

template <size_t... SqueezeDims, typename T,
          typename = Std::enable_if_t<(is_layout_v<T> || is_attr_tensor_v<T>) && (sizeof...(SqueezeDims) > 0)>>
__aicore__ inline constexpr auto Squeeze(const T& value)
{
    return squeeze<SqueezeDims...>(value);
}

template <typename Pattern, typename T,
          typename = Std::enable_if_t<(is_layout_v<T> || is_attr_tensor_v<T>)
                                      && Std::is_tuple_v<Std::remove_cvref_t<Pattern>>>>
__aicore__ inline constexpr auto Squeeze(const T& value, const Pattern& pattern)
{
    return squeeze(value, pattern);
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_LEGACY_TENSOR_TENSOR_H
