/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_TENSOR_API_LEGACY_TENSOR_LAYOUT_PATTERN_H
#define IMPL_TENSOR_API_LEGACY_TENSOR_LAYOUT_PATTERN_H

namespace asc {
namespace te {

using ZNLayoutPtn = zn_layout_ptn;
using ZZLayoutPtn = zz_layout_ptn;
using NNLayoutPtn = nn_layout_ptn;
using NZLayoutPtn = nz_layout_ptn;
using NDLayoutPtn = nd_layout_ptn;
using DNLayoutPtn = dn_layout_ptn;
using NDExtLayoutPtn = nd_ext_layout_ptn;
using DNExtLayoutPtn = dn_ext_layout_ptn;
using ScaleANDLayoutPtn = scalea_nd_layout_ptn;
using ScaleADNLayoutPtn = scalea_dn_layout_ptn;
using ScaleBNDLayoutPtn = scaleb_nd_layout_ptn;
using ScaleBDNLayoutPtn = scaleb_dn_layout_ptn;
using NCHWLayoutPtn = nchw_layout_ptn;
using NHWCLayoutPtn = nhwc_layout_ptn;
using NC1HWC0LayoutPtn = nc1hwc0_layout_ptn;
using NCDHWLayoutPtn = ncdhw_layout_ptn;
using NDC1HWC0LayoutPtn = ndc1hwc0_layout_ptn;

using MakeNzFrameLayout = make_nz_frame_layout;
using MakeNDExtFrameLayout = make_nd_ext_frame_layout;
using MakeNDFrameLayout = make_nd_frame_layout;
using MakeZnFrameLayout = make_zn_frame_layout;
using MakeDNFrameLayout = make_dn_frame_layout;
using MakeDNExtFrameLayout = make_dn_ext_frame_layout;
using MakeZzFrameLayout = make_zz_frame_layout;
using MakeNnFrameLayout = make_nn_frame_layout;
using MakeScaleANDFrameLayout = make_scalea_nd_frame_layout;
using MakeScaleADNFrameLayout = make_scalea_dn_frame_layout;
using MakeScaleBNDFrameLayout = make_scaleb_nd_frame_layout;
using MakeScaleBDNFrameLayout = make_scaleb_dn_frame_layout;
using MakeNCHWFrameLayout = make_nchw_frame_layout;
using MakeNHWCFrameLayout = make_nhwc_frame_layout;
using MakeNC1HWC0FrameLayout = make_nc1hwc0_frame_layout;
using MakeNCDHWFrameLayout = make_ncdhw_frame_layout;
using MakeNDC1HWC0FrameLayout = make_ndc1hwc0_frame_layout;

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_LEGACY_TENSOR_LAYOUT_PATTERN_H
