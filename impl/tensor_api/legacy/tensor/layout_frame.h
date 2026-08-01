/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_TENSOR_API_LEGACY_TENSOR_LAYOUT_FRAME_H
#define IMPL_TENSOR_API_LEGACY_TENSOR_LAYOUT_FRAME_H

namespace asc {
namespace te {

using LayoutFormatSet = layout_format_set;

template <typename T, typename C0>
using LayoutTrait = layout_trait<T, C0>;

template <typename T = uint16_t, typename C0 = Std::Int<C0_ELEMENT<T>>>
using LayoutTraitDefault = layout_trait_default<T, C0>;

using LayoutTraitScale = layout_trait_scale;
using LayoutTraitFP4 = layout_trait_fp4;
using FormatTraitSet = format_trait_set;

template <typename T, typename Enable = void>
using IsFrameLayoutTrait = is_frame_layout_trait<T, Enable>;

template <typename T>
constexpr bool IsFrameLayoutTraitV = is_frame_layout_trait_v<T>;

template <typename LayoutPattern, typename TraitType>
using TraitConversion = trait_conversion<LayoutPattern, TraitType>;

template <typename Maker, typename Trait, typename Enable = void>
using SupportsRowColMake = supports_row_col_make<Maker, Trait, Enable>;

template <typename Maker, typename Trait>
constexpr bool SupportsRowColMakeV = supports_row_col_make_v<Maker, Trait>;

template <typename LayoutPattern, typename TraitType = layout_trait_default<>>
using FrameLayoutFormat = frame_layout_format<LayoutPattern, TraitType>;

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_LEGACY_TENSOR_LAYOUT_FRAME_H
