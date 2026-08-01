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
    "impl/tensor_api/tensor/layout_pattern.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file layout_pattern.h
* \brief
*/
#ifndef IMPL_TENSOR_API_TENSOR_LAYOUT_PATTERN_H
#define IMPL_TENSOR_API_TENSOR_LAYOUT_PATTERN_H

#include "impl/tensor_api/tensor/layout_method.h"

namespace asc {
namespace te {

struct zn_layout_ptn {};
struct zz_layout_ptn {};
struct nn_layout_ptn {};
struct nz_layout_ptn {};
struct nd_layout_ptn {};
struct dn_layout_ptn {};
struct nd_ext_layout_ptn {};
struct dn_ext_layout_ptn {};
struct scale_a_nd_layout_ptn {};
struct scale_a_dn_layout_ptn {};
struct scale_b_nd_layout_ptn {};
struct scale_b_dn_layout_ptn {};

using ZNLayoutPtn = zn_layout_ptn;
using ZZLayoutPtn = zz_layout_ptn;
using NNLayoutPtn = nn_layout_ptn;
using NZLayoutPtn = nz_layout_ptn;
using NDLayoutPtn = nd_layout_ptn;
using DNLayoutPtn = dn_layout_ptn;
using NDExtLayoutPtn = nd_ext_layout_ptn;
using DNExtLayoutPtn = dn_ext_layout_ptn;
using ScaleANDLayoutPtn = scale_a_nd_layout_ptn;
using ScaleADNLayoutPtn = scale_a_dn_layout_ptn;
using ScaleBNDLayoutPtn = scale_b_nd_layout_ptn;
using ScaleBDNLayoutPtn = scale_b_dn_layout_ptn;

template <typename LayoutPattern, typename TraitType, typename ShapeType, typename StrideType>
__aicore__ inline constexpr auto make_pattern_layout(const ShapeType& shape, const StrideType& stride)
{
    using LayoutT = Layout<ShapeType, StrideType, Std::tuple<LayoutPattern, TraitType>>;
    return LayoutT(shape, stride);
} 

template <typename LayoutPattern, typename TraitType, typename Batch, typename LayoutType>
__aicore__ inline constexpr auto make_batch_pattern_layout(const Batch& batch, const LayoutType& layout)
{
    return make_pattern_layout<LayoutPattern, TraitType>(
        make_shape(batch, layout.shape()), make_stride(layout.capacity(), layout.stride()));
}

struct make_nz_frame_layout {
    template <typename TraitType, typename T, typename U>
    __aicore__ inline static auto make(T row, U column) {
        constexpr auto c0_ele = TraitType::C0_ELEMENT;
        auto shape = make_shape(make_shape(Std::Int<FRACTAL_FIXED>{}, Std::ceil_division(row, FRACTAL_FIXED)),
                               make_shape(c0_ele, Std::ceil_division(column, c0_ele)));
        auto stride = make_stride(make_stride(c0_ele, c0_ele * Std::Int<FRACTAL_FIXED>{}),
                                 make_stride(_1{}, c0_ele * Std::ceil_align(row, FRACTAL_FIXED)));
        using LayoutT = Layout<decltype(shape), decltype(stride), Std::tuple<NZLayoutPtn, TraitType>>;
        return LayoutT(shape, stride);
    }

    template <typename TraitType, typename Batch, typename T, typename U>
    __aicore__ inline static auto make(Batch batch, T row, U column) {
        return make_batch_pattern_layout<NZLayoutPtn, TraitType>(batch, make<TraitType>(row, column));
    }
};

struct make_nd_ext_frame_layout {
    template <typename TraitType, typename T, typename U>
    __aicore__ inline static auto make(T row, U column) {
        auto shape = make_shape(make_shape(_1{}, row), make_shape(_1{}, column));
        auto stride = make_stride(make_stride(_0{}, column), make_stride(_0{}, _1{}));
        using LayoutT = Layout<decltype(shape), decltype(stride), Std::tuple<NDExtLayoutPtn, TraitType>>;
        return LayoutT(shape, stride);
    }

    template <typename TraitType, typename Batch, typename T, typename U>
    __aicore__ inline static auto make(Batch batch, T row, U column) {
        return make_batch_pattern_layout<NDExtLayoutPtn, TraitType>(batch, make<TraitType>(row, column));
    }
};

struct make_nd_frame_layout {
    template <typename TraitType, typename T, typename U>
    __aicore__ inline static auto make(T row, U column) {
        auto shape = make_shape(row, column);
        auto stride = make_stride(column, _1{});
        using LayoutT = Layout<decltype(shape), decltype(stride), Std::tuple<NDLayoutPtn, TraitType>>;
        return LayoutT(shape, stride);
    }

    template <typename TraitType, typename Batch, typename T, typename U>
    __aicore__ inline static auto make(Batch batch, T row, U column) {
        return make_batch_pattern_layout<NDLayoutPtn, TraitType>(batch, make<TraitType>(row, column));
    }
};

struct make_zn_frame_layout {
    template <typename TraitType, typename T, typename U>
    __aicore__ inline static auto make(T row, U column) {
        constexpr auto c0_ele = TraitType::C0_ELEMENT;
        auto shape = make_shape(make_shape(c0_ele, Std::ceil_division(row, c0_ele)),
                               make_shape(Std::Int<FRACTAL_FIXED>{}, Std::ceil_division(column, FRACTAL_FIXED)));
        auto stride = make_stride(make_stride(_1{}, c0_ele * Std::ceil_align(column, FRACTAL_FIXED)),
                                 make_stride(c0_ele, c0_ele * Std::Int<FRACTAL_FIXED>{}));
        using LayoutT = Layout<decltype(shape), decltype(stride), Std::tuple<ZNLayoutPtn, TraitType>>;
        return LayoutT(shape, stride);
    }

    template <typename TraitType, typename Batch, typename T, typename U>
    __aicore__ inline static auto make(Batch batch, T row, U column) {
        return make_batch_pattern_layout<ZNLayoutPtn, TraitType>(batch, make<TraitType>(row, column));
    }
};

struct make_dn_frame_layout {
    template <typename TraitType, typename T, typename U>
    __aicore__ inline static auto make(T row, U column) {
        auto shape = make_shape(row, column);
        auto stride = make_stride(_1{}, row);
        using LayoutT = Layout<decltype(shape), decltype(stride), Std::tuple<DNLayoutPtn, TraitType>>;
        return LayoutT(shape, stride);
    }

    template <typename TraitType, typename Batch, typename T, typename U>
    __aicore__ inline static auto make(Batch batch, T row, U column) {
        return make_batch_pattern_layout<DNLayoutPtn, TraitType>(batch, make<TraitType>(row, column));
    }
};

struct make_dn_ext_frame_layout {
    template <typename TraitType, typename T, typename U>
    __aicore__ inline static auto make(T row, U column) {
        auto shape = make_shape(make_shape(_1{}, row), make_shape(_1{}, column));
        auto stride = make_stride(make_stride(_0{}, _1{}), make_stride(_0{}, row));
        using LayoutT = Layout<decltype(shape), decltype(stride), Std::tuple<DNExtLayoutPtn, TraitType>>;
        return LayoutT(shape, stride);
    }

    template <typename TraitType, typename Batch, typename T, typename U>
    __aicore__ inline static auto make(Batch batch, T row, U column) {
        return make_batch_pattern_layout<DNExtLayoutPtn, TraitType>(batch, make<TraitType>(row, column));
    }
};

struct make_zz_frame_layout {
    template <typename TraitType, typename T, typename U>
    __aicore__ inline static auto make(T row, U column) {
        constexpr auto c0_ele = TraitType::C0_ELEMENT;
        auto shape = make_shape(make_shape(Std::Int<FRACTAL_FIXED>{}, Std::ceil_division(row, FRACTAL_FIXED)),
                               make_shape(c0_ele, Std::ceil_division(column, c0_ele)));
        auto stride = make_stride(make_stride(c0_ele, FRACTAL_FIXED * Std::ceil_align(column, c0_ele)),
                                 make_stride(_1{}, c0_ele * Std::Int<FRACTAL_FIXED>{}));
        using LayoutT = Layout<decltype(shape), decltype(stride), Std::tuple<ZZLayoutPtn, TraitType>>;
        return LayoutT(shape, stride);
    }

    template <typename TraitType, typename Batch, typename T, typename U>
    __aicore__ inline static auto make(Batch batch, T row, U column) {
        return make_batch_pattern_layout<ZZLayoutPtn, TraitType>(batch, make<TraitType>(row, column));
    }
};

struct make_nn_frame_layout {
    template <typename TraitType, typename T, typename U>
    __aicore__ inline static auto make(T row, U column) {
        constexpr auto c0_ele = TraitType::C0_ELEMENT;
        static_assert(c0_ele == 2, "NnLayoutPtn only supports fp8_e8m0_t and ShapeColumn0 as 2.");
        auto shape = make_shape(make_shape(c0_ele, row / c0_ele), make_shape(Std::Int<FRACTAL_FIXED>{}, Std::ceil_division(column, FRACTAL_FIXED)));
        auto stride = make_stride(make_stride(_1{}, c0_ele * Std::Int<FRACTAL_FIXED>{}),
                                 make_stride(c0_ele, row * FRACTAL_FIXED));
        using LayoutT = Layout<decltype(shape), decltype(stride), Std::tuple<NNLayoutPtn, TraitType>>;
        return LayoutT(shape, stride);
    }

    template <typename TraitType, typename Batch, typename T, typename U>
    __aicore__ inline static auto make(Batch batch, T row, U column) {
        return make_batch_pattern_layout<NNLayoutPtn, TraitType>(batch, make<TraitType>(row, column));
    }
};

struct make_scale_a_nd_frame_layout {
    template <typename TraitType, typename T, typename U>
    __aicore__ inline static auto make(T row, U column) {
        constexpr auto c0_ele = TraitType::C0_ELEMENT;
        static_assert(c0_ele == 2, "ScaleANDLayoutPtn only supports fp8_e8m0_t and ShapeColumn0 as 2.");
        auto shape = make_shape(make_shape(_1{}, row), make_shape(_1{}, column));
        auto stride = make_stride(make_stride(_0{}, column), make_stride(_0{}, _1{}));
        using LayoutT = Layout<decltype(shape), decltype(stride), Std::tuple<ScaleANDLayoutPtn, TraitType>>;
        return LayoutT(shape, stride);
    }

    template <typename TraitType, typename Batch, typename T, typename U>
    __aicore__ inline static auto make(Batch batch, T row, U column) {
        return make_batch_pattern_layout<ScaleANDLayoutPtn, TraitType>(batch, make<TraitType>(row, column));
    }
};

struct make_scale_a_dn_frame_layout {
    template <typename TraitType, typename T, typename U>
    __aicore__ inline static auto make(T row, U column) {
        constexpr auto c0_ele = TraitType::C0_ELEMENT;
        static_assert(c0_ele == 2, "ScaleADNLayoutPtn only supports fp8_e8m0_t and ShapeColumn0 as 2.");
        auto shape = make_shape(make_shape(_1{}, row), make_shape(c0_ele, column / c0_ele));
        auto stride = make_stride(make_stride(_0{}, c0_ele),
                                 make_stride(_1{}, c0_ele * row));
        using LayoutT = Layout<decltype(shape), decltype(stride), Std::tuple<ScaleADNLayoutPtn, TraitType>>;
        return LayoutT(shape, stride);
    }

    template <typename TraitType, typename Batch, typename T, typename U>
    __aicore__ inline static auto make(Batch batch, T row, U column) {
        return make_batch_pattern_layout<ScaleADNLayoutPtn, TraitType>(batch, make<TraitType>(row, column));
    }
};

struct make_scale_b_nd_frame_layout {
    template <typename TraitType, typename T, typename U>
    __aicore__ inline static auto make(T row, U column) {
        constexpr auto c0_ele = TraitType::C0_ELEMENT;
        static_assert(c0_ele == 2, "ScaleBNDLayoutPtn only supports fp8_e8m0_t and ShapeColumn0 as 2.");
        auto shape = make_shape(make_shape(c0_ele, row / c0_ele), make_shape(_1{}, column));
        auto stride = make_stride(make_stride(_1{}, c0_ele * column),
                                 make_stride(_0{}, c0_ele));
        using LayoutT = Layout<decltype(shape), decltype(stride), Std::tuple<ScaleBNDLayoutPtn, TraitType>>;
        return LayoutT(shape, stride);
    }

    template <typename TraitType, typename Batch, typename T, typename U>
    __aicore__ inline static auto make(Batch batch, T row, U column) {
        return make_batch_pattern_layout<ScaleBNDLayoutPtn, TraitType>(batch, make<TraitType>(row, column));
    }
};

struct make_scale_b_dn_frame_layout {
    template <typename TraitType, typename T, typename U>
    __aicore__ inline static auto make(T row, U column) {
        auto shape = make_shape(make_shape(_1{}, row), make_shape(_1{}, column));
        auto stride = make_stride(make_stride(_0{}, _1{}), make_stride(_0{}, row));
        using LayoutT = Layout<decltype(shape), decltype(stride), Std::tuple<ScaleBDNLayoutPtn, TraitType>>;
        return LayoutT(shape, stride);
    }

    template <typename TraitType, typename Batch, typename T, typename U>
    __aicore__ inline static auto make(Batch batch, T row, U column) {
        return make_batch_pattern_layout<ScaleBDNLayoutPtn, TraitType>(batch, make<TraitType>(row, column));
    }
};

struct nchw_layout_ptn {};
struct nhwc_layout_ptn {};
struct nc1hwc0_layout_ptn {};

using NCHWLayoutPtn = nchw_layout_ptn;
using NHWCLayoutPtn = nhwc_layout_ptn;
using NC1HWC0LayoutPtn = nc1hwc0_layout_ptn;

struct make_nchw_frame_layout {
    template <typename TraitType, typename N, typename C, typename H, typename W>
    __aicore__ inline static auto make(N n, C c, H h, W w) {
        auto shape = make_shape(n, c, h, w);
        auto stride = make_stride(c * h * w, h * w, w, _1{});
        using LayoutT = Layout<decltype(shape), decltype(stride), Std::tuple<NCHWLayoutPtn, TraitType>>;
        return LayoutT(shape, stride);
    }
};

struct make_nhwc_frame_layout {
    template <typename TraitType, typename N, typename H, typename W, typename C>
    __aicore__ inline static auto make(N n, H h, W w, C c) {
        auto shape = make_shape(n, h, w, c);
        auto stride = make_stride(h * w * c, w * c, c, _1{});
        using LayoutT = Layout<decltype(shape), decltype(stride), Std::tuple<NHWCLayoutPtn, TraitType>>;
        return LayoutT(shape, stride);
    }
};

struct make_nc1hwc0_frame_layout {
    template <typename TraitType, typename N, typename C1, typename H, typename W, typename C0>
    __aicore__ inline static auto make(N n, C1 c1, H h, W w, C0 c0) {
        auto shape = make_shape(n, c1, h, w, c0);
        auto stride = make_stride(c1 * h * w * c0, h * w * c0, w * c0, c0, _1{});
        using LayoutT = Layout<decltype(shape), decltype(stride), Std::tuple<NC1HWC0LayoutPtn, TraitType>>;
        return LayoutT(shape, stride);
    }
};

// conv3D feature map formats. NCDHW is the row-major logical layout; NDC1HWC0 is the fractal L1
// layout (C split into C1 outer / C0 inner, with the depth axis D between N and C1).
struct ncdhw_layout_ptn {};
struct ndc1hwc0_layout_ptn {};

using NCDHWLayoutPtn = ncdhw_layout_ptn;
using NDC1HWC0LayoutPtn = ndc1hwc0_layout_ptn;

struct make_ncdhw_frame_layout { // Shape = (N, C, D, H, W)
    template <typename TraitType, typename N, typename C, typename D, typename H, typename W>
    __aicore__ inline static auto make(N n, C c, D d, H h, W w) {
        auto shape = make_shape(n, c, d, h, w);
        auto stride = make_stride(c * d * h * w, d * h * w, h * w, w, _1{});
        using LayoutT = Layout<decltype(shape), decltype(stride), Std::tuple<NCDHWLayoutPtn, TraitType>>;
        return LayoutT(shape, stride);
    }
};

struct make_ndc1hwc0_frame_layout { // Shape = (N, D, C1, H, W, C0)
    template <typename TraitType, typename N, typename D, typename C1, typename H, typename W, typename C0>
    __aicore__ inline static auto make(N n, D d, C1 c1, H h, W w, C0 c0) {
        auto shape = make_shape(n, d, c1, h, w, c0);
        auto stride = make_stride(d * c1 * h * w * c0, c1 * h * w * c0, h * w * c0, w * c0, c0, _1{});
        using LayoutT = Layout<decltype(shape), decltype(stride), Std::tuple<NDC1HWC0LayoutPtn, TraitType>>;
        return LayoutT(shape, stride);
    }
};

using MakeNzFrameLayout = make_nz_frame_layout;
using MakeNDExtFrameLayout = make_nd_ext_frame_layout;
using MakeNDFrameLayout = make_nd_frame_layout;
using MakeZnFrameLayout = make_zn_frame_layout;
using MakeDNFrameLayout = make_dn_frame_layout;
using MakeDNExtFrameLayout = make_dn_ext_frame_layout;
using MakeZzFrameLayout = make_zz_frame_layout;
using MakeNnFrameLayout = make_nn_frame_layout;
using MakeScaleANDFrameLayout = make_scale_a_nd_frame_layout;
using MakeScaleADNFrameLayout = make_scale_a_dn_frame_layout;
using MakeScaleBNDFrameLayout = make_scale_b_nd_frame_layout;
using MakeScaleBDNFrameLayout = make_scale_b_dn_frame_layout;
using MakeNCHWFrameLayout = make_nchw_frame_layout;
using MakeNHWCFrameLayout = make_nhwc_frame_layout;
using MakeNC1HWC0FrameLayout = make_nc1hwc0_frame_layout;
using MakeNCDHWFrameLayout = make_ncdhw_frame_layout;
using MakeNDC1HWC0FrameLayout = make_ndc1hwc0_frame_layout;

} // namespace te
} // namespace asc


// PascalCase compatibility wrappers
namespace asc {
namespace te {
template <typename LayoutPattern, typename TraitType, typename ShapeType, typename StrideType>
__aicore__ inline constexpr auto MakePatternLayout(const ShapeType& shape, const StrideType& stride)
{
    return make_pattern_layout<LayoutPattern, TraitType>(shape, stride);
}

template <typename LayoutPattern, typename TraitType, typename Batch, typename LayoutType>
__aicore__ inline constexpr auto MakeBatchPatternLayout(const Batch& batch, const LayoutType& layout)
{
    return make_batch_pattern_layout<LayoutPattern, TraitType>(batch, layout);
}
} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_TENSOR_LAYOUT_PATTERN_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
